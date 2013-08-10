#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<iostream>
#include <fstream>
#include <vector>

//estructura de personal
typedef struct{
	//Largo de la planificacion
       int w;
	//Numero de empleados
       int n;
       //Numero de turnos mas dia libre
       unsigned int m;
	//vector de largo m donde se indican los diferentes tipos de turnos el tipo de turno
       char A[80];
       //Matriz de Requerimientos de (m - 1) x w
       int R[80][80];
	//Vector de largo m donde por cada turno se indica el maxima de turnos o dias libres consecutivos permitidos
       int MAXS[80];      
	//Vector de largo m donde por cada turno se indica el manimo de turnos o dias libres consecutivos permitidos
       int MINS[80];
	//Numero maximo de dias consecutivos trabajados
       int MAXW[3];
       //Numero minimo de dias consecutivos trabajados
       int MINW[3];
       //Numero de secuencias de turnos no permitidas de largo 2
       int NSLength2;
       //Numero de secuencuas de turnos no permitidos largo 3
       int NSLength3;
       //Matriz secuencias de turnos no permitidos
       char C2[80][80];
}workforce;
workforce wk;
unsigned int linea=0;
unsigned int i=0;
unsigned int j=0;
unsigned int contador1=0;
unsigned int contador2=0;
unsigned int contador3=0;
unsigned int contador4=0;
unsigned int contador5=0;
int tr=0;
int cmin=0;
int cmax=7;
int a[7]={0,0,0,0,0,0,0},n[7]={0,0,0,0,0,0,0},d[7]={0,0,0,0,0,0,0};

//lee archivo por entrada estandar, linea por linea pues los archivos entregados poseen la misma estructura
int leer_archivo(){
    char mensaje[80];
    char temp[3];
    char var[2];
    char *ptr;
    unsigned int linea2=0;
    unsigned int linea3=0;
    unsigned int linea4=0;
    unsigned int linea5=0;
    unsigned int linea6=0;

    while(fgets(mensaje,50,stdin) != NULL){
             if(linea==1){
                wk.w=atoi(mensaje);
             }else if(linea==4){
                      wk.n=atoi(mensaje);
             }else if(linea==7){
                      wk.m=atoi(mensaje);
             }else if(linea==10+i){
                      if(i<(wk.m)-1){
                         ptr = strtok(mensaje, " ");
                         while(ptr != NULL){
                               strcpy(temp,ptr);
                               wk.R[i][j]=atoi(temp);
                               ptr = strtok(NULL, " ");
                               j++;
                         }
                         i++;
                         j=0;
                      }else{
                            linea2=linea;
                      }
             }else if(linea==linea2+contador1+3 && linea!=0 && linea!=3){
                      if(contador1<(wk.m)-1){
                         ptr = strtok(mensaje, " ");
                         if(ptr != NULL){
                            strcpy(temp,ptr);
                            wk.A[contador1]=*temp;
                            ptr = strtok(NULL, " ");
                            ptr = strtok(NULL, " ");
                            ptr = strtok(NULL, " ");
                            strcpy(temp,ptr);
                            wk.MINS[contador1]=atoi(temp);
                            ptr = strtok(NULL, " ");
                            strcpy(temp,ptr);
                            wk.MAXS[contador1]=atoi(temp);
                         }
                          contador1++;
                      }else{
                            linea3=linea;
                      }
            }else if(linea==linea3+3 && linea!=0 && linea!=3){
                    strcpy(var,"-");
                    wk.A[contador1]=*var;
                    ptr = strtok(mensaje, " ");
                    strcpy(temp,ptr);
                    wk.MINS[contador1]=atoi(temp);
                    ptr = strtok(NULL, " ");
                    strcpy(temp,ptr);
                    wk.MAXS[contador1]=atoi(temp);
                    linea4=linea;
            }else if(linea==linea4+3 && linea!=0 && linea!=3){
                    ptr = strtok(mensaje, " ");
                    strcpy(temp,ptr);
                    wk.MINW[contador2]=atoi(temp);
                    ptr = strtok(NULL, " ");
                    strcpy(temp,ptr);
                    wk.MAXW[contador2+1]=atoi(temp);
                    linea5=linea;
            }else if(linea==linea5+4 && linea!=0 && linea!=4){
                    ptr = strtok(mensaje, " ");
                    strcpy(temp,ptr);
                    wk.NSLength2=atoi(temp);
                    ptr = strtok(NULL, " ");
                    strcpy(temp,ptr);
                    wk.NSLength3=atoi(temp);
                    linea6=linea;
             }else if(linea==linea6+contador4+3 && linea!=0 && linea!=3){
                     if(contador4 < strlen(mensaje)){
                        ptr = strtok(mensaje, " ");
                        while(ptr != NULL){
                              strcpy(temp,ptr);
                              wk.C2[contador4][contador5]=*temp;
                              ptr = strtok(NULL, " ");
                              contador5++;
                        }
                        contador4++;
                        contador5=0;
                     }
             }else{

             }
      linea++;
 }
    return 0;
}

using namespace std;
int mforward(vector <int>  &my_vector, int &tr, int &cmin, int &cmax, int a[],int n[], int d[]);
vector<int> my_vector;

int dominio1(){
	int i,j,k,m,n,f,g;
	
	int s[] = {1,2,3,4}; //vector A con 1=D, 2=A, 3=N, 4=-
	/*Lleno un vector*/

	for (i=0; i<4; i++) { // 1 loop
		for (j=0; j<4; j++) { // 2 loop
			for (k=0; k<4; k++) { // 3 loop
				for (m=0; m<4; m++) { // 4 loop
					for (n=0; n<4; n++) { // 5 loop
						for (f=0; f<4; f++) { // 6 loop
							for (g=0; g<4; g++) { // 7 loop
								my_vector.push_back(s[i]);
								my_vector.push_back(s[j]);
								my_vector.push_back(s[k]);
								my_vector.push_back(s[m]);
								my_vector.push_back(s[n]);
								my_vector.push_back(s[f]);
								my_vector.push_back(s[g]);
								
							}
						}
					}
				}
			}
		}
	}
							
	
	return 0;
}

int nodoc(vector <int>  &my_vector){
//MINS
	int cont=0, cont2=0 , mins1=0, mins2=0, mins3=0, mins4=0;
	ofstream fs("archivo.txt");
/*Declaro un iterador para recorrer el vector anterior y borro los que no cumplan con mins del turno D*/
	vector<int>  ::iterator my_iterator;		
	for(my_iterator = my_vector.begin(); my_iterator < my_vector.end(); my_iterator++){
		if(*my_iterator==1){
			mins1++;	
		}
		if(cont==7){
			if(mins1<wk.MINS[0]){
				while(cont2<7){
					my_vector.erase(my_iterator);			
					my_iterator--;
					cont2++;
				}
			}
			mins1=0;
			cont=0;	
			cont2=0;	
		}
		cont++;	
			
	}

/*borro los que no cumplan con mins del turno A */
	cont=0; cont2=0;
	for(my_iterator = my_vector.begin(); my_iterator < my_vector.end(); my_iterator++){
		if(*my_iterator==2){
			mins2++;	
		}
		if(cont==7){
			if(mins2<wk.MINS[1]){
				while(cont2<7){
					my_vector.erase(my_iterator);			
					my_iterator--;
					cont2++;
				}
			}
			mins2=0;
			cont=0;	
			cont2=0;	
		}
		cont++;	
			
	}
		
/*borro los que no cumplan con mins del turno N */
	cont=0; cont2=0;
	for(my_iterator = my_vector.begin(); my_iterator < my_vector.end(); my_iterator++){
		if(*my_iterator==3){
			mins3++;	
		}
		if(cont==7){
			if(mins3<wk.MINS[2]){
				while(cont2<7){
					my_vector.erase(my_iterator);			
					my_iterator--;
					cont2++;
				}
			}
			mins3=0;
			cont=0;	
			cont2=0;	
		}
		cont++;	
			
	}

/*borro los que no cumplan con mins del turno libre */
	cont=0;	cont2=0;
	for(my_iterator = my_vector.begin(); my_iterator < my_vector.end(); my_iterator++){
		if(*my_iterator==4){
			mins4++;	
		}
		if(cont==7){
			if(mins4<wk.MINS[3]){
				while(cont2<7){
					my_vector.erase(my_iterator);			
					my_iterator--;
					cont2++;
				}
			}
			mins4=0;
			cont=0;	
			cont2=0;	
		}
		cont++;		
	}
//MAXS
	int maxs1=0, maxs2=0, maxs3=0, maxs4=0;

/*borro los que no cumplan con maxs del turno D*/
	cont=0;	cont2=0;
	for(my_iterator = my_vector.begin(); my_iterator < my_vector.end(); my_iterator++){
		if(*my_iterator==1){
			maxs1++;	
		}
		if(cont==7){
			if(maxs1>wk.MAXS[0]){
				while(cont2<7){
					my_vector.erase(my_iterator);			
					my_iterator--;
					cont2++;
				}
			}
			maxs1=0;
			cont=0;	
			cont2=0;	
		}
		cont++;			
	}

/*borro los que no cumplan con maxs del turno A */
	cont=0; cont2=0;
	for(my_iterator = my_vector.begin(); my_iterator < my_vector.end(); my_iterator++){
		if(*my_iterator==2){
			maxs2++;	
		}
		if(cont==7){
			if(maxs2>wk.MAXS[1]){
				while(cont2<7){
					my_vector.erase(my_iterator);			
					my_iterator--;
					cont2++;
				}
			}
			maxs2=0;
			cont=0;	
			cont2=0;	
		}
		cont++;			
	}
/*borro los que no cumplan con maxs del turno N */
	cont=0; cont2=0;
	for(my_iterator = my_vector.begin(); my_iterator < my_vector.end(); my_iterator++){
		if(*my_iterator==3){
			maxs3++;	
		}
		if(cont==7){
			if(maxs3>wk.MAXS[2]){
				while(cont2<7){
					my_vector.erase(my_iterator);			
					my_iterator--;
					cont2++;
				}
			}
			maxs3=0;
			cont=0;	
			cont2=0;	
		}
		cont++;	
			
	}
/*borro los que no cumplan con maxs del turno libre */
	cont=0;	cont2=0;
	for(my_iterator = my_vector.begin(); my_iterator < my_vector.end(); my_iterator++){
		if(*my_iterator==4){
			maxs4++;	
		}
		if(cont==7){
			if(maxs4>wk.MAXS[3]){
				while(cont2<7){
					my_vector.erase(my_iterator);			
					my_iterator--;
					cont2++;
				}
			}
			maxs4=0;
			cont=0;	
			cont2=0;	
		}
		cont++;			
	}
//MINW
	int minw=0;
	cont=0;	cont2=0;
	for(my_iterator = my_vector.begin(); my_iterator < my_vector.end(); my_iterator++){
		if(*my_iterator!=4){
			minw++;	
		}
		if(cont==7){
			if(minw<wk.MINW[0]){
				while(cont2<7){
					my_vector.erase(my_iterator);			
					my_iterator--;
					cont2++;
				}
			}
			minw=0;
			cont=0;	
			cont2=0;	
		}
		cont++;			
	}
			
//MAXW
	int maxw=0;
	cont=0;	cont2=0;
	for(my_iterator = my_vector.begin(); my_iterator < my_vector.end(); my_iterator++){
		if(*my_iterator!=4){
			maxw++;		
		}
		if(cont==7){
			if(maxw>wk.MAXW[1]){
				while(cont2<7){
					my_vector.erase(my_iterator);			
					my_iterator--;
					cont2++;
				}
			}
			maxw=0;
			cont=0;	
			cont2=0;	
		}
		cont++;	
			
	}	
		
	for(my_iterator = my_vector.begin(); my_iterator < my_vector.end(); my_iterator++){
		if(cont==7){	
			fs << endl;
			//fs << " cont:" <<(cont);
			cont=0;	
		}		
			fs << (*my_iterator) ;					
			cont++;
	}
	
	return 0;
}
int x[9][7];

/*int contar(int tr, int a[],int n[], int d[], int x[][]){
	a[7]={0,0,0,0,0,0,0};
	n[7]={0,0,0,0,0,0,0};
	d[7]={0,0,0,0,0,0,0};
	int j,h;	
	for(h=0;h<=tr;h++){
             for(j=0;j<7;j++){
		if(x[h][j]==1){
			d[j]=1+d[j];
			 //rc << "d=" <<(d[j]);
		}
		if(x[h][j]==2){
			a[j]=1+a[j];
			//rc << "a=" << (a[j]);
		}	
		if(x[h][j]==3){
			n[j]=1+n[j];
			//rc << "n=" << (n[j]);		
		}
	    }	
	}
	mforward(my_vector,tr,cmin,cmax,a,n,d);
	return 0;
}*/
ofstream rc("ruteo2.txt");
ofstream rt("ruteo.txt");
ofstream sol("solucion.txt");
ofstream fc("archivo2.txt");

int asignar(vector <int>  &my_vector, int tr, int &cmin, int &cmax,int a[],int n[], int d[]){
	int j,h,m;
	//cout << "trasignar="<< (tr) <<endl;
	for(h=cmin,j=0;h<cmax,j<7;h++,j++){
		x[tr][j]=my_vector[h];
		rc << "j="<<(j)<<endl;
		rc << "h="<<(h)<<endl;
		rc << "cmax="<<(cmax)<<endl;
		rc << "cmin="<<(cmin)<<endl;
		rc << "tr" <<(tr)<<endl;
	}
	for(j=0;j<7;j++){
		a[j]=0;
		n[j]=0;
		d[j]=0;
	}	
	for(m=0;m<=tr;m++){
             for(j=0;j<7;j++){
		if(x[m][j]==1){
			d[j]=1+d[j];
			rt << "d=" <<(d[j]);
		}
		if(x[m][j]==2){
			a[j]=1+a[j];
			rt << "a=" << (a[j]);
		}	
		if(x[m][j]==3){
			n[j]=1+n[j];
			rt << "n=" << (n[j]);		
		}
	    }rt<<endl;	
	}
	for(int i=0;i<9;i++){
		for(int j=0;j<7;j++){
			sol << (x[i][j]);
		}
		sol << endl;
	}
	mforward(my_vector,tr,cmin,cmax,a,n,d);
	//contar(tr,a,n,d,x);	
	return 0;
}


int mforward(vector <int>  &my_vector, int &tr, int &cmin, int &cmax, int a[],int n[], int d[]){
	
	int j=0,cont=0;
	//cumplir con R
	if(tr<9){
	    for(j=0;j<7;j++){
		if(wk.R[0][j]>=d[j]){
			cont++;
		}
		if(wk.R[1][j]>=a[j]){
			cont++;
		}
		if(wk.R[2][j]>=n[j]){
			cont++;	
		}
	    }	rt << "cont1="<<(cont) <<endl;
		if(cont==21){
			tr++;
			//rc << "trif20="<< (tr) <<endl;	
			cmin=0;
			cmax=7;
			cont=0;
			asignar(my_vector, tr, cmin, cmax,a,n,d);
		}
		else {
			
			if(cmax<16384){	
			cmin=cmin+7;
			cmax=cmax+7;	
			cont=0;
			asignar(my_vector, tr, cmin, cmax,a,n,d);
			}
			//si se acaba el vector saltar		
		}
	}
	/*else{
		for(int i=0;i<9;i++){
			for(int j=0;j<7;j++){
				sol << (wk.x[i][j]);
			}
			sol << endl;
		}
	}*/

	/*for(j=0;j<7;j++){
		cout << " d=" <<(d[j]);
		cout << " a=" << (a[j]);		
		cout << " n=" << (n[j]);			
		
	}*/
return 0;
}

int main(){
	leer_archivo();
	dominio1();
	nodoc(my_vector);	
	asignar(my_vector,tr,cmin,cmax,a,n,d);
	//mforward(my_vector,tr,cmin,cmax,a,n,d);
	
/*for(my_iterator = my_vector.begin(); my_iterator < my_vector.end(); my_iterator++){
		if(cont==7){	
			fc << endl;
			//fs << " cont:" <<(cont);
			cont=0;	
		}		
			fc << (*my_iterator) ;					
			cont++;
	}*/
/*for(int i=0;i<3;i++){
for(int j=0;j<7;j++){
cout << (wk.R[i][j]);}
cout << endl;}
*/
	return 0;
}
