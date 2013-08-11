#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define POPSIZE 20             /* population size */ 
#define MAXGENS 100           /* max. number of generations */ 
#define NVARS 2                /* no. of problem variables */ 
#define PXOVER 0.25            /* probability of crossover */ 
#define PMUTATION 0.01         /* probability of mutation */ 
#define PI 3.141592654

FILE *galog;
int generation;

struct genotype /* genotype (GT), a member of the population */ {
  double gene[NVARS];           /* a string of variables */
  double fitness;               /* GT's fitness */
  double upper[NVARS];          /* GT's variables upper bound */
  double lower[NVARS];          /* GT's variables lower bound */
  double rfitness;              /* relative fitness */
  double cfitness;              /* cumulative fitness */
};

struct genotype population[POPSIZE+1];    /* population */ 
struct genotype newpopulation[POPSIZE+1];        /* new population; */

void evaluate(void) { 
    int mem; int i; double x[NVARS+1];
    for(mem=0; mem<POPSIZE; mem++){
	for(i=0; i<NVARS; i++)
	{
	    x[i+1] = population[mem].gene[i];
	   // printf("%.5f, ", x[i+1]);
	}
        population[mem].fitness = 21.5 + x[1]*sin(4*PI*x[1]) + x[2]*sin(20*PI*x[2]);
	//printf("%.5f\n", population[mem].fitness);
    }
    getchar();
}

void keep_the_best() {
    int mem; int i; int cur_best = 0; 
    /*stores the index of the best individual */

    for(mem=0; mem<POPSIZE; mem++){
	if(population[mem].fitness > population[POPSIZE].fitness){
            cur_best = mem;
            population[POPSIZE].fitness = population[mem].fitness;
        }
    }

    /* once the best member in the population is found, copy the genes*/
    for(i=0; i<NVARS; i++)
        population[POPSIZE].gene[i] = population[cur_best].gene[i];
}

void elitist() {
    int i; double best, worst;    /* best and worst fitness values */ 
    int best_mem, worst_mem;      /* indexes of the best and worst member */

    best = population[0].fitness;
    worst = population[0].fitness;

    for (i = 0; i < POPSIZE - 1; ++i){
        if(population[i].fitness > population[i+1].fitness) {
            if (population[i].fitness >= best){
                best = population[i].fitness;
                best_mem = i;}
            if (population[i+1].fitness <= worst){
                worst = population[i+1].fitness;
                worst_mem = i + 1;}}
        else {
            if (population[i].fitness <= worst) {
                worst = population[i].fitness;
                worst_mem = i;}
            if (population[i+1].fitness >= best) {
                best = population[i+1].fitness;
                best_mem = i + 1;
		}
	}
    }
    return;
}

void selection(void) {
    int mem, i, j, k;
    double sum = 0;
    double p;

    /* find total fitness of the population */
    for(mem=0; mem<POPSIZE; mem++){
        sum += population[mem].fitness;
    }

    /* calculate relative fitness */
    for(mem=0; mem<POPSIZE; mem++){
        population[mem].rfitness =  population[mem].fitness/sum;
    }
    /* calculate cumulative fitness */
    for(mem=0; mem<POPSIZE; mem++){
        population[mem].cfitness =  population[mem-1].cfitness + population[mem].rfitness;
    }
    population[0].cfitness = population[0].rfitness;
    /* finally select survivors using cumulative fitness. */
    for (i = 0; i < POPSIZE; i++){
        p = rand()%1000/1000.0;
	if(p < population[0].fitness)
            newpopulation[i] = population[0];
        else{
	    for(j=0; j<POPSIZE; j++)
	    if((p >= population[j].cfitness) && (p < population[j+1].cfitness))
                newpopulation[i] = population[j+1];
      }
  }
    /* once a new population is created, copy it back */
    for(i=0; i<POPSIZE; i++)
        population[i] = newpopulation[i];
   return;
}

void Xover(int one, int two){
    int i;
    int point;    /* crossover point */

    /* select crossover point */ 
    if(NVARS > 1){
	if(NVARS == 2)
            point = 1;
        else
            point = (rand() % (NVARS - 1)) + 1;
	for(i=0; i<point; i++)
	{
	    float aux = population[one].gene[i];
            population[one].gene[i] = population[two].gene[i];
            population[two].gene[i] = aux;
	}
    }
}

void crossover(void) { 
    int i, mem, one; int first  =  0;

    /* count of the number of members chosen */ 
    double x;
    for(mem=0; mem<POPSIZE; ++mem){
        x = rand()%1000/1000.0;
	printf("%d",x);
	if(x < PXOVER){
            ++first;
	    if(first % 2 == 0)
                Xover(one, mem);
            else
                one = mem;
        }
    }
}


double randval(double low, double high){
 double val;
 val = ((double)(rand()%1000)/1000.0)*(high-low)+low;
 return val;
}


void mutate(void) {
    int i, j;
    double lbound, hbound, x;

    for(i=0; i<POPSIZE; i++){
        for (j = 0; j < NVARS; j++){
            x = rand()%1000/1000.0;
            if (x < PMUTATION){
                /* find the bounds on the variable to be mutated */
                lbound = population[i].lower[j];
                hbound = population[i].upper[j];
                population[i].gene[j] = randval(lbound, hbound);
            }
        }
    }
}

void initialize(void)
{
    int i, j;
    for (j = 0; j < POPSIZE; j++) {
	population[j].fitness = 0;
	population[j].rfitness = 0;
	population[j].cfitness = 0;
 	    for (i = 0; i < NVARS; i++)
	    { 
 		population[j].lower[i] = 0;
 		population[j].upper[i] = 0.99;
 		population[j].gene[i] = randval(population[j].lower[i], population[j].upper[i]);
	    }
    }
    return;
}

void report(void)
{
    int i;
    double best_val;
    best_val = population[POPSIZE].fitness;
    fprintf(galog, "\n%5d, %6.3f, \n", generation, best_val);
}

int main(void) {
    int i;
    if((galog=fopen("galog.txt", "w"))==NULL){
	exit(1);
    }
    generation = 0;

    fprintf(galog, "\n generation best averagestandard \n");
    fprintf(galog, " number value fitness deviation \n");

    initialize();
    evaluate();
    keep_the_best();

    while(generation<MAXGENS){
        generation++;
	selection();
        crossover();
        mutate();
        report();
        evaluate();
        elitist();
    }

    fprintf(galog,"\n\n Completed\n"); fprintf(galog,"\n Best member: \n");
    for (i = 0; i < NVARS; i++) {
        fprintf (galog,"\n var(%d) = %3.3f",i,population[POPSIZE].gene[i]);
    }
    fprintf(galog,"\n\n Best fitness = %3.3f",population[POPSIZE].fitness);
    fclose(galog);
    printf("Success\n");
    return 0; 
}



