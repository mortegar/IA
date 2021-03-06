#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<iostream>
#include <fstream>
#include <vector>

#define POPSIZE 20            /* tamanio de la poblacion */ 
#define MAXGENS 80          /* maximo numero de poblacion */ 
#define PXOVER 0.90            /* probabilidad de cruzamiento */ 
#define PMUTATION 0.00         /* probabilidad de mutacion */ 
#define ACJ 50            /* tamanio de arreglos */ 


//estructura 
typedef struct{
	//numero total de aviones
       int p;
	//tiempo mas temprano en intervalo de tiempo de avion i
       int bef[ACJ];
	//tiempo esperado de aterrizaje para avion i
       int target[ACJ];
	//tiempo mas tarde en intervalo de tiempo avion i
       int last[ACJ];
	//penalizacion por unidad de tiempo para el avion i por aterrizar antes del tiempo esperado
       float pbef[ACJ];
	//penalizacion por unidad de tiempo para el avion i por aterrizar luego del tiempo esperado
      	float plast[ACJ];
       //Separacion necesaria entre aterrizaje avion i y aterrizaje avion j
       int sep[ACJ][ACJ];    
	//numero total de pistas
       int pista;
}airland;
airland al;

typedef struct{/* gen, miembro población */
	float y[ACJ];           /* a string of variables */
	double fitness;               /* valor fitness */
	double unfitness; 		/* valor unfitness */
	int pista[ACJ]; 
	int cuadrante;
}genotype;

genotype poblacion[POPSIZE+1]; //poblacion
//genotype newpoblacion[POPSIZE+1]; //nueva poblacion
genotype hijo;
//genotype h;

unsigned int linea=0;

/*Lee archivo por entrada estandar, linea por linea pues los archivos entregados poseen la misma estructura*/
int leer_archivo(){
char *ptr;
char mje[ACJ];
int j,n;
int i=0,k=0;

    while(fgets(mje,50,stdin) != NULL){
	n=linea%2;
	if (linea==0){
		ptr = strtok(mje, " ");
        	al.p=atoi(mje);
	}else if(n!=0){
		j=0;
             	ptr = strtok(mje, " ");
                         while(ptr != NULL){
				if(j==0)
					al.bef[i]=atoi(ptr);
				if(j==1)
					al.target[i]=atoi(ptr);
				if(j==2)
					al.last[i]=atoi(ptr);
				if(j==3)
					al.pbef[i]=atoi(ptr);
				if(j==4)
					al.plast[i]=atoi(ptr);
					
                               ptr = strtok(NULL, " ");
                               j++;
			}    
		i++; 
	} else if(n==0){
		j=0;
		ptr = strtok(mje, " ");
                         while(ptr != NULL){
				al.sep[j][k]=atoi(ptr);   
				
                               ptr = strtok(NULL, " ");
                               j++;
			}     k++; 
	}
    linea++; 	   
    }		
    return 0;
}

int clasificar();
void reemplazar();
void evaluarm(char* archivo);
using namespace std;
ofstream rc("ruteo.txt");
//ofstream rt("ruteo2.txt");
//ofstream pb("pb.txt");
//ofstream pbn("pbn.txt");

/*funcion que da comienzo a la primera generacion de la poblacion*/
void iniciar(void){ 

    for (int j = 0; j < POPSIZE; j++) {
	poblacion[j].fitness = 0; //inicializa en cero el valor de fitness para cada individuo 
 	    for (int i = 0; i < al.p; i++)
	    { 
 		poblacion[j].y[i]= (float)rand()/(float)RAND_MAX;
		poblacion[j].pista[i] = 1; //inicializa a todos los aviones que aterricen en la pista 1 
		//rc << "| y= "<<(poblacion[j].y[i]);
		//printf("y: %.5f, ",poblacion[j].y[i]);
	    }//rc <<endl;
    }
    return;
}

/*funcion que evalua el fitness y unfitness de cada miembro de la poblacion */
void evaluar(void) { 
	int mem; int i,s=0,j; 
	double x[al.p+1], d[al.p+1], a[al.p+1], r[al.p+1];
	float f=0,uf=0, sumf[ACJ], sumu[ACJ];
	for(mem=0; mem<POPSIZE; mem++){ // sumatoria comienza en 0
		sumf[mem]=0;
		sumu[mem]=0;
	}
	for(mem=0; mem<POPSIZE; mem++){ //calculo de fitness
		//pb << "| mem= "<<(mem);
		for(i=0; i<al.p; i++){
			x[i] = al.bef[i]+poblacion[mem].y[i]*(al.last[i]-al.bef[i]); //convertir de y a x
			//printf("%.5f, ", x[i]);
			//pb << "| y= "<<(poblacion[mem].y[i]);			
			d[i]=x[i]-al.target[i]; // desviacion
			if(d[i]<0){ // aterriza antes target time
				a[i]=-d[i];
				r[i]=0;
			}
			if(d[i]>0){ // aterriza despues target time
				r[i]=d[i];
				a[i]=0;
				}
			if(d[i]==0){ // aterriza justo en target time 
				a[i]=0;
				r[i]=0;
			}
			f=(al.pbef[i]*a[i]+al.plast[i]*r[i]);
			sumf[mem]=sumf[mem]+f;	
		}//rc <<endl;	
		for(i=0; i<al.p; i++){// calculo unfitness		
			for(j=0; j<al.p; j++){
				if(poblacion[mem].pista[j]==poblacion[mem].pista[i]){
					s=al.sep[i][j]-abs(x[j]-x[i]); //mayor unfitness menos factible
					if(s>0&&j>i){ 
						uf=s;
						if(poblacion[mem].pista[j]<al.pista){ 
							poblacion[mem].pista[j]=poblacion[mem].pista[j]+1;
							//rc << "| entre a cambio pista ";
							uf=0;
						}
					}else uf=0;
				} 				
			}sumu[mem]=sumu[mem]+uf;//valor distinto de 0 la separacion fue violada
		}
		poblacion[mem].fitness=sumf[mem];
		poblacion[mem].unfitness=sumu[mem];
		//printf("FITNESS %.5f\n", poblacion[mem].fitness);
		//pb << "| FITNESS= "<<(poblacion[mem].fitness);
		//rc <<endl;//printf("UNFITNESS %.5f\n", poblacion[mem].unfitness);
		//rc << "| UNFITNESS= "<<(poblacion[mem].unfitness);
		//rc <<endl;
	}
}

/*funcion que evalua el fitness y unfitness del hijo de la poblacion */
void evaluarh() { 
	int i,s,j; 
	double x[al.p+1], d[al.p+1], a[al.p+1], r[al.p+1];
	float f=0,uf=0, sumf, sumu;
	// sumatoria comienza en 0
	sumf=0;
	sumu=0;
	//calculo de fitness
	for(i=0; i<al.p; i++){
		x[i] = al.bef[i]+hijo.y[i]*(al.last[i]-al.bef[i]); //convertir de y a x
		//rc << "| x= "<<(x[i]);
		d[i]=x[i]-al.target[i]; // desviacion
		if(d[i]<0){ // aterriza antes target time
			a[i]=-d[i];
			r[i]=0;
		}
		if(d[i]>0){ // aterriza despues target time
			r[i]=d[i];
			a[i]=0;
		}
		if(d[i]==0){ // aterriza justo en target time 
			a[i]=0;
			r[i]=0;
		}
		f=(al.pbef[i]*a[i]+al.plast[i]*r[i]);
		sumf=sumf+f;	
	}//rc <<endl;	
	
	for(i=0; i<al.p; i++){// calculo unfitness		
		for(j=0; j<al.p; j++){
			if(hijo.pista[j]==hijo.pista[i]){
				s=al.sep[i][j]-abs(x[j]-x[i]); //mayor unfitness menos factible
				if(s>0&&j>i){ 
					uf=s;
					if(hijo.pista[j]<al.pista){ 
						hijo.pista[j]=hijo.pista[j]+1;
						//pbn << "|cta| ";
						uf=0;
					}
				}else uf=0;
			} 				
		}sumu=sumu+uf;//valor distinto de 0 la separacion fue violada
	}
	hijo.fitness=sumf;
	hijo.unfitness=sumu;
	
	//rc << "| FITNESS mejor= "<<(hijo.fitness);
	//rc <<endl;
	//rc << "| UNFITNESS= "<<(hijo.unfitness);
	//rc <<endl;
}

/*void cambia_pista(genotype np){
	np.pista

}*/
/*cruzamiento uniforme 2 padres->1 hijo,toma cada valor al azar de los dos padres*/
void Xover(int one, int two){
    int i;
    float point;    /* crossover point */
	for(i=0; i<al.p; i++){
		point = rand()%1000/1000.0;
		//rc << "|punto= "<<(point);
		//rc << "|i= "<<(i);
		
		if (point>=0.5){
			hijo.y[i]=poblacion[one].y[i];
	    	}else{
			hijo.y[i]=poblacion[two].y[i];
		}//rc << "| hijo= "<<(hijo.y[i]); 
	} 
	clasificar();	
	//rc <<endl;
}
/* seleciona los padres a cruzar*/
void crossover(void) { 
	
    /*  torneo binario*/

	int uno,dos;
	int one,two;
	int cant;
	//double x;
	cant = 2*POPSIZE;
	for(int mem=0; mem<cant; ++mem){
		// x = rand()%1000/1000.0;
		//if(x < PXOVER){
		uno = rand()%POPSIZE;
		dos = rand()%POPSIZE;
		if(poblacion[uno].fitness<=poblacion[dos].fitness){
			one=uno;
		}else one=dos;
	
		uno = rand()%POPSIZE;
		dos = rand()%POPSIZE;
		if(poblacion[uno].fitness<=poblacion[dos].fitness){
			two=uno;
		}else two=dos;
		Xover(one, two);
	//	}
        }
    

	
/*al azar con probabilidad */
  /*  int mem, one; 
    int first  =  0;

  // contar el número de miembros elegidos
    double x;
    for(mem=0; mem<POPSIZE; ++mem){
        x = rand()%1000/1000.0;
	//printf("%f",x);
	if(x < PXOVER){
            ++first;
	    if(first % 2 == 0)
               Xover(one, mem);
            else
                one = mem;
        }
    }*/
}

void muta(void) { 
    int mem, point; 
    double x;
    for(mem=0; mem<POPSIZE; ++mem){
        x = rand()%1000/1000.0;
	//printf("%f",x);
	//point = rand()%POPSIZE;
	point = rand()%al.p;
	//rc <<" punto" << point;
	//rc <<endl;
	if(x < PMUTATION){
	rc << "muto ";
          poblacion[mem].y[point] =(float)rand()/(float)RAND_MAX;
	  //poblacion[mem].y[point] = al.target[point];
        }
    }
}
void guardar_mejor() {
	int mem; int i; //int mejor = 0; 
 	
	for(mem=0; mem<POPSIZE; mem++){
		if(poblacion[mem].fitness < poblacion[POPSIZE].fitness){ //busca el mejor miembro
			//mejor = mem;
			poblacion[POPSIZE].fitness = poblacion[mem].fitness; 
		  	/*Se copia el mejor miembro*/
			for(i=0; i<al.p; i++){//el mejor miembro se guarda al final de la poblacion
				poblacion[POPSIZE].y[i] = poblacion[mem].y[i];
				//rc << "| ent2= ";
			}
					//poblacion[POPSIZE].fitness = -10; 
					//rc << "| ent1= ";
				}
	}
  
	//rc << "| popz= "<<(POPSIZE); 
	//rc << "| fitmejor= "<<(poblacion[POPSIZE].fitness); 
	//evaluarh(poblacion[POPSIZE]);
}

/*clasifica la poblacion en cuadrantes para reemplazar a los peores*/
int clasificar(){
	int mem; 
	evaluarh();
	for(mem=0; mem<POPSIZE; ++mem){
		if(poblacion[mem].fitness<=hijo.fitness){
			if(poblacion[mem].unfitness<=hijo.unfitness){
				poblacion[mem].cuadrante=4;
				//rc << "| mem= "<<(mem);
				//rc << "| cua= "<<(poblacion[mem].cuadrante);
			}
			if(poblacion[mem].unfitness>hijo.unfitness){			
				poblacion[mem].cuadrante=2; 
				//rc << "| mem= "<<(mem);
				//rc << "| cua= "<<(poblacion[mem].cuadrante);
				//rc <<endl;
			}
		}else{
			if(poblacion[mem].unfitness<=hijo.unfitness){
				poblacion[mem].cuadrante=3;
				//rc << "| mem= "<<(mem);
				//rc << "| cua= "<<(poblacion[mem].cuadrante);
			}
			if(poblacion[mem].unfitness>hijo.unfitness){			
				poblacion[mem].cuadrante=1;
				//rc << "| mem= "<<(mem);
				//rc << "| cua= "<<(poblacion[mem].cuadrante);
			}
		}//rc <<endl;
        }
	reemplazar();
	return 0;
}



void reemplazar(){
	int cont=0, mem,ne=0,t=0;
	//int t;
	vector<int> numeros;
	vector<int>  ::iterator my_iterator;
	
    // Llenar el vector con los numeros desde 0 al tamaño de poblacion
	for (int i = 0; i < POPSIZE; i++){
		numeros.push_back(i);
	}
	//rt <<  "---------entre----------- " << endl;

 //reemplaza un miembro en del 1er cuadrante
	while (cont!=1 && numeros.size() > 0){
		//Genera una posicion en el vector de numeros y coge el numero de aquella posicion 
            	int indice = rand() % numeros.size();
		//rt << "| indice= "<<(indice); 
		if (poblacion[indice].cuadrante==1){
			for(mem=0; mem<al.p; mem++){
				poblacion[indice].y[mem] = hijo.y[mem];
				//rc << "| hijoCopia= "<<(poblacion[indice].y[mem]); 
				ne=1;//encuentra un miembro en el primer cuadrante ne=1 sino sigue siendo 0
			}
			cont=1;	
		}
		//cout << "Numero aleatorio: " << numeros[indice] << endl;
		//rt << " cont: " << cont << endl;
		//borra el numero para que no vuelva a aparecer
            	numeros.erase(numeros.begin() + indice);	
				
		/*for(my_iterator = numeros.begin(); my_iterator < numeros.end(); my_iterator++){
			rt << "| " << (*my_iterator) ;					
		}rt << endl;*/
	}

	if(ne==0){
		for(int i = 0; i < POPSIZE&&t==0; i++){
			if (poblacion[i].cuadrante==1){
				for(mem=0; mem<al.p; mem++){
					
					poblacion[i].y[mem] = hijo.y[mem];
					//rc << "| hijoCopia= "<<(poblacion[i].y[mem]); 
					t=1;
				}
				ne=1;
			}
		}	
	}

//segundo cuadrante
	for (int i = 0; i < POPSIZE; i++){
		numeros.push_back(i);
	}
	if(ne==0){
		while (cont!=1 && numeros.size() > 0){
			//Genera una posicion en el vector de numeros y coge el numero de aquella posicion 
		    	int indice = rand() % numeros.size();
			//rt << "| indice= "<<(indice); 
			if (poblacion[indice].cuadrante==2){
				for(mem=0; mem<al.p; mem++){
					poblacion[indice].y[mem] = hijo.y[mem];
					//rc << "| hijoCopia= "<<(poblacion[indice].y[mem]); 
					ne=1;
				}
				cont=1;	
			}
			//cout << "Numero aleatorio: " << numeros[indice] << endl;
			//rt << " cont: " << cont << endl;
			//borra el numero para que no vuelva a aparecer
		    	numeros.erase(numeros.begin() + indice);	
				
			/*for(my_iterator = numeros.begin(); my_iterator < numeros.end(); my_iterator++){
				rt << "| " << (*my_iterator) ;					
			}rt << endl;*/
		}
	}
	if(ne==0){
		for(int i = 0; i < POPSIZE&&t==0; i++){
			if (poblacion[i].cuadrante==2){
				for(mem=0; mem<al.p; mem++){
					poblacion[i].y[mem] = hijo.y[mem];
					//rc << "| hijoCopia= "<<(poblacion[i].y[mem]); 
					t=1;
				}
				ne=1;		
			}
		}	
	}
//tercer cuadrante
	for (int i = 0; i < POPSIZE; i++){
		numeros.push_back(i);
	}
	if(ne==0){
		while (cont!=1 && numeros.size() > 0){
			//Genera una posicion en el vector de numeros y coge el numero de aquella posicion 
		    	int indice = rand() % numeros.size();
			//rt << "| indice= "<<(indice); 
			if (poblacion[indice].cuadrante==3){
				for(mem=0; mem<al.p; mem++){
					poblacion[indice].y[mem] = hijo.y[mem];
					//rc << "| hijoCopia= "<<(poblacion[indice].y[mem]); 
					ne=1;
				}
				cont=1;	
			}
			//cout << "Numero aleatorio: " << numeros[indice] << endl;
			//rt << " cont: " << cont << endl;
			//borra el numero para que no vuelva a aparecer
		    	numeros.erase(numeros.begin() + indice);	
				
		/*	for(my_iterator = numeros.begin(); my_iterator < numeros.end(); my_iterator++){
				rt << "| " << (*my_iterator) ;					
			}rt << endl;*/
		}
	}
	if(ne==0){
		for(int i = 0; i < POPSIZE&&t==0; i++){
			if (poblacion[i].cuadrante==3){
				for(mem=0; mem<al.p; mem++){
					poblacion[i].y[mem] = hijo.y[mem];
					//rc << "| hijoCopia= "<<(poblacion[i].y[mem]); 
					t=1;
				}
				ne=1;		
			}
		}	
	}
//cuarto cuadrante 
	for (int i = 0; i < POPSIZE; i++){
		numeros.push_back(i);
	}
	if(ne==0){
		while (cont!=1 && numeros.size() > 0){
			//Genera una posicion en el vector de numeros y coge el numero de aquella posicion 
		    	int indice = rand() % numeros.size();
			//rt << "| indice= "<<(indice); 
			if (poblacion[indice].cuadrante==4){
				for(mem=0; mem<al.p; mem++){
					poblacion[indice].y[mem] = hijo.y[mem];
					//rc << "| hijoCopia= "<<(poblacion[indice].y[mem]); 
					ne=1;
				}
				cont=1;	
			}
			//cout << "Numero aleatorio: " << numeros[indice] << endl;
			//rt << " cont: " << cont << endl;
			//borra el numero para que no vuelva a aparecer
		    	numeros.erase(numeros.begin() + indice);	
				
			/*for(my_iterator = numeros.begin(); my_iterator < numeros.end(); my_iterator++){
				rt << "| " << (*my_iterator) ;					
			}rt << endl;*/
		}
	}
	if(ne==0){
		for(int i = 0; i < POPSIZE&&t==0; i++){
			if (poblacion[i].cuadrante==4){
				for(mem=0; mem<al.p; mem++){
					poblacion[i].y[mem] = hijo.y[mem];
					//rc << "| hijoCopia= "<<(poblacion[i].y[mem]); 
					t=1;
				}
				ne=1;		
			}
		}	
	}


}




int main(int argc, char *argv[]){
	char* archivo; 
	char* out;
	//srand(time(NULL));
	al.pista = atoi(argv[1]);

	archivo = argv[2];
	out=".out"; 
	strcat( archivo, out );	
	//printf("archivo: %s",archivo);
	
	
	leer_archivo();
	iniciar();
	guardar_mejor();
	poblacion[POPSIZE].fitness=99999999;
	for(int i=0;i<MAXGENS;i++){ 
		evaluar();
		guardar_mejor();
		crossover();	
		muta();
	}
	/*imprimir nuevapoblacion
		for(int pop=0; pop<POPSIZE; pop++){
			for(int mem=0; mem<al.p; mem++){
				pbn << "|x= "<<(poblacion[pop].y[mem]); 
				
			}pbn << "|mem= "<<(pop); 
			pbn << endl;
		}*/
	/*double ximp[al.p+1];
	for(int asd=0; asd<al.p; asd++){
		//poblacion[POPSIZE].y[i] = poblacion[mejor].y[i];
		ximp[asd] = al.bef[asd]+poblacion[POPSIZE].y[asd]*(al.last[asd]-al.bef[asd]);
		pbn << "| mejor= "<<(ximp[asd]); 
	}*/

	evaluarm(archivo);
	
	
	//variable(poblacion.y);
	return 0;
}

void evaluarm(char* archivo) { 
	int i,s,j; 
	double x[al.p+1];
	//double d[al.p+1], a[al.p+1], r[al.p+1];
	float uf=0, sumf, sumu;
	ofstream outp(archivo);
	// sumatoria comienza en 0
	sumf=0;
	sumu=0;
	//calculo de fitness
	/*for(i=0; i<al.p; i++){
		x[i] = al.bef[i]+poblacion[POPSIZE].y[i]*(al.last[i]-al.bef[i]); //convertir de y a x
		//rc << "| x= "<<(x[i]);
		d[i]=x[i]-al.target[i]; // desviacion

		if(d[i]<0){ // aterriza antes target time
			a[i]=-d[i];
			r[i]=0;

		}
		if(d[i]>0){ // aterriza dint leer_archivo(){
char *ptr;
char mje[ACJ];
int j,n;
int i=0,k=0;

    while(fgets(mje,50,stdin) != NULL){
	n=linea%2;
	if (linea==0){
		ptr = strtok(mje, " ");
        	al.p=atoi(mje);
	}else if(n!=0){
		j=0;
             	ptr = strtok(mje, " ");
                         while(ptr != NULL){
				if(j==0){
					al.bef[i]=atoi(ptr);					
					rc<<"|bef"<<(al.bef[i]);}
				if(j==1){
					al.target[i]=atoi(ptr);					
					rc<<"|t"<<(al.target[i]);}
				if(j==2){
					al.last[i]=atoi(ptr);					
					rc<<"|last"<<(al.last[i]);}
				if(j==3){
					al.pbef[i]=atoi(ptr);
					rc<<"pbef"<<(al.pbef[i]);}
				if(j==4){
					al.plast[i]=atoi(ptr);
					rc<<"pla"<<(al.plast[i])<<endl;}
                               ptr = strtok(NULL, " ");
                               j++;
			}    
		i++; 
	} else if(n==0){
		j=0;
		ptr = strtok(mje, " ");
                         while(ptr != NULL){
				al.sep[j][k]=atoi(ptr);   
				
                               ptr = strtok(NULL, " ");
                               j++;
			}     k++; 
	}
    linea++; 	   
    }		
    return 0;
}espues target time
			r[i]=d[i];
			a[i]=0;

		}
		if(d[i]==0){ // aterriza justo en target time 
			a[i]=0;

			r[i]=0;
		}
		f=(al.pbef[i]*a[i]+al.plast[i]*r[i]);
		sumf=sumf+f;	

	}//rc <<endl;	*/
	for(i=0; i<al.p; i++){// calculo unfitness		
		for(j=0; j<al.p; j++){
			if(poblacion[POPSIZE].pista[j]==poblacion[POPSIZE].pista[i]){
				s=al.sep[i][j]-abs(x[j]-x[i]); //mayor unfitness menos factible
				if(s>0&&j>i){ 
					uf=s;
					if(poblacion[POPSIZE].pista[j]<al.pista){ 
						poblacion[POPSIZE].pista[j]=poblacion[POPSIZE].pista[j]+1;
						//pbn << "|cta| ";
						uf=0;
					}
				}else uf=0;
			} 				
		}sumu=sumu+uf;//valor distinto de 0 la separacion fue violada
	}
	//poblacion[POPSIZE].fitness=sumf;
	poblacion[POPSIZE].unfitness=sumu;

	outp <<(poblacion[POPSIZE].fitness); 
	outp <<endl;
	//pbn << "| UNFITNESS= "<<(hijo.unfitness);
	//pbn <<endl;

	double ximp[al.p+1];
	for(int b=0; b<al.p; b++){
		ximp[b] = al.bef[b]+poblacion[POPSIZE].y[b]*(al.last[b]-al.bef[b]);
		outp << (ximp[b]) <<" " ; 
		outp << (poblacion[POPSIZE].pista[b])<<endl; 
	}
	
}



