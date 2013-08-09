#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<iostream>
#include <fstream>
#include <vector>

#define POPSIZE 20             /* population size */ 
#define MAXGENS 100           /* max. number of generations */ 
#define PXOVER 0.25            /* probability of crossover */ 
#define PMUTATION 0.01         /* probability of mutation */ 

//variables globales

int x[80];

//estructura 
typedef struct{
	//numero total de aviones
       int p;
	//tiempo mas temprano en intervalo de tiempo de avion i
       int bef[80];
	//tiempo esperado de aterrizaje para avion i
       int target[80];
	//tiempo mas tarde en intervalo de tiempo avion i
       int last[80];
	//penalizacion por unidad de tiempo para el avion i por aterrizar antes del tiempo esperado
       float pbef[80];
	//penalizacion por unidad de tiempo para el avion i por aterrizar luego del tiempo esperado
      	float plast[80];
       //Separacion necesaria entre aterrizaje avion i y aterrizaje avion j
       int sep[80][80];    
	//numero total de pistas
       int pista;
}airland;
airland al;

typedef struct{/* gen, miembro población */
	float y[10];           /* a string of variables */
	double fitness;               /* valor fitness */
	double unfitness; 		/* valor unfitness */
}genotype;

genotype population[POPSIZE+1]; //poblacion
genotype newpopulation[POPSIZE+1]; //nueva poblacion


unsigned int linea=0;
//lee archivo por entrada estandar, linea por linea pues los archivos entregados poseen la misma estructura
int leer_archivo(){
char *ptr;
char mje[80];
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

void iniciar(void){

    for (int j = 0; j < POPSIZE; j++) {
	population[j].fitness = 0; //inicializa en cero el valor de fitness para cada individuo 
	
 	    for (int i = 0; i < al.p; i++)
	    { 
 		population[j].y[i]= (float)rand()/(float)RAND_MAX;
		//printf("%.5f, ",population[j].y[i]);
	    }
    }
    return;
}

void evaluar(void) { 
	int mem; int i; 
	double x[al.p+1], d[al.p+1], a[al.p+1], r[al.p+1];
	float f, sum=0;
	for(mem=0; mem<POPSIZE; mem++){
		for(i=0; i<al.p; i++){
			x[i+1] = al.bef[i]+population[mem].y[i]*(al.last[i]-al.bef[i]);
			printf("%.5f, ", x[i+1]);
			d[i+1]=x[i+1]-al.target[i+1];
			if(d[i+1]<0){
				a[i+1]=-d[i+1];
				r[i+1]=0;
			}
			if(d[i+1]>0){
				r[i+1]=d[i+1];
				a[i+1]=0;
				}
			if(d[i+1]==0){
				a[i+1]=0;
				r[i+1]=0;
			}
			f=(al.pbef[i+1]*a[i+1]+al.plast[i+1]*r[i+1]);
			sum=sum+f;
		}	
		population[mem].fitness=sum[mem];	
		printf("FITNESS %.5f\n", population[mem].fitness);
	}
}

/*
int variable(float ay[]){
	for (int i=0;i<=al.p;i++){
		x[i]=al.bef[i]+ay[i]*(al.last[i]-al.bef[i]);
		printf(" x: %d ",x[i]);
	}
	return 0;
}
*/

int main(){
	leer_archivo();
	iniciar();
	evaluar(); 
	//variable(population.y);
	return 0;
}
