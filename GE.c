#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<iostream>
#include <fstream>
#include <vector>

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
unsigned int linea=0;


//lee archivo por entrada estandar, linea por linea pues los archivos entregados poseen la misma estructura
int leer_archivo(){
char *ptr;
char mje[80];
int j,n;
int i=0;

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
	} else if(n!=0){
		j=0;
             	ptr = strtok(mje, " ");
                         while(ptr != NULL){
				al.sep[j][i]=atoi(ptr);   
					
                               ptr = strtok(NULL, " ");
                               j++;
			    
			}     i++; 
	}
    
    linea++; 
	//printf("n:%d\n",n);	
	//printf("l:%d\n",linea);	   
    }	
	printf( "%d\n", al.p);
	printf( "%d\n", al.bef[9]);
	printf( "%d\n", al.target[9]);
        printf( "%d\n", al.last[9]);
	printf( "%f\n", al.pbef[9]);
	printf( "%f\n", al.plast[9]);
	  
	
    return 0;
}
int main(){
	leer_archivo();
	return 0;
}
