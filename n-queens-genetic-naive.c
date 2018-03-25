//
//  main.c
//  queens
//
//  Created by Mateus Nunes de B Magalhaes on 3/22/18.
//  Copyright © 2018 mateusnbm. All rights reserved.
//

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define N 8
#define INDIVIDUAL_SIZE 24
#define POPULATION_SIZE 100
#define MAXIMUM_CHILDS_PER_CYCLE 40
#define MUTATION_PROBABILITY 40
#define RECOMBINATION_PROBABILITY 90
#define MAXIMUM_FITNESS_EVALUATIONS 10000

int childsCount = 0;
int fitnessCount = 0;
int parenting[POPULATION_SIZE];
int parents[POPULATION_SIZE/5][2];
int fitness[POPULATION_SIZE + MAXIMUM_CHILDS_PER_CYCLE];
char *population[POPULATION_SIZE + MAXIMUM_CHILDS_PER_CYCLE];

int binaryToDecimal(char arr[], int i, int length) {
    
    int num = 0;
    int max = i + length;
    
    for (; i < max; i++) {
        
        num = num << 1;
        num += (arr[i] - '0');
        
    }
    
    return num;
    
}

void printArray(int arr[], int size) {
    
    int i;
    
    for (i = 0; i < size; i++) {
        
        printf("%i\n", arr[i]);
        
    }
    
    printf("\n");
    
}

void printPopulation() {
    
    int i, j;
    
    for (i = 0; i < POPULATION_SIZE; i++) {
        
        for (j = 0; j < INDIVIDUAL_SIZE; j+=3) {
            
            printf("%i ", binaryToDecimal(population[i], j, 3));
            
        }
        
        printf("(%i)\n", fitness[i]);
        
    }
    
    printf("\n");
    
}

int partition(int arr[], int i0, int iX) {
    
    int aux;
    char *aux_individual;
    
    int originalPivotIndex = i0;
    int pivot = arr[i0];
    char *pivot_individual = population[i0];
    
    while (i0 < iX) {
        
        // Move the left index to the right until a number smaller than the pivot is found.
        
        for (; arr[i0] >= pivot && i0 < iX; i0++);
        
        // Move the right index to the left until a number bigger than the pivot is found.
        
        for (; arr[iX] < pivot; iX--);
        
        // While the two indexes don't come across, keep swapping the numbers
        // in order to store the ones bigger than the pivot to the left and the
        // ones smaller than the pivot to the right.
        
        if (i0 < iX) {
            
            aux = arr[i0];
            arr[i0] = arr[iX];
            arr[iX] = aux;
            
            // Swap individuals (addition to this specific problem).
            
            aux_individual = population[i0];
            population[i0] = population[iX];
            population[iX] = aux_individual;
            
        }
        
    }
    
    arr[originalPivotIndex] = arr[iX];
    arr[iX] = pivot;
    
    // Swap individuals (addition to this specific problem).
    
    population[originalPivotIndex] = population[iX];
    population[iX] = pivot_individual;
    
    return iX;
    
}

void quicksort(int arr[], int i0, int iX) {
    
    if (i0 < iX) {
        
        int pivotIndex = partition(arr, i0, iX);
        
        quicksort(arr, i0, (pivotIndex - 1));
        quicksort(arr, (pivotIndex + 1), iX);
        
    }
    
}

char *randomIndividual() {
    
    int i;
    char *individual = malloc(INDIVIDUAL_SIZE * sizeof(char));
    
    for (i = 0; i < INDIVIDUAL_SIZE; i++) {
        
        individual[i] = '0' + (rand() % 2);
        
    }
    
    return individual;
    
}

int computeIndividualFitness(char *individual) {
    
    int j, jx;
    int f = 0;
    
    fitnessCount++;
    
    // The fitness is equal to the number of queens in valid spots.
    // Loop through each queen position (row assignment).
    
    for (j = 0; j < N; j++) {
        
        int collisions = 0;
        int baseQueenRow = binaryToDecimal(individual, j*3, 3);
        
        // Look for queens on the same row or diagonal.
        
        for (jx = 0; jx < N; jx++) {
            
            int endQueenRow = binaryToDecimal(individual, jx*3, 3);
            int sameRow = (baseQueenRow == endQueenRow);
            int onDiagonal = (abs(baseQueenRow-endQueenRow) == abs(j-jx));
            
            collisions += ((sameRow == 1 || onDiagonal == 1) && jx != j);
            
        }
        
        f += (collisions == 0);
        
    }
    
    return f;
    
}

void selectParents() {
    
    int i, j;
    
    // Parents selection.
    // Select 5 individuals randomically and breed the top two.
    
    for (i = 0; i < POPULATION_SIZE; i++) {
        
        parenting[i] = 0;
        
    }
    
    for (i = 0; i < (POPULATION_SIZE/5); i++) {
        
        int index;
        int a = -1, b = -1;
        
        for (j = 0; j < 5; j++) {
            
            index = (rand() % POPULATION_SIZE);
            
            while (parenting[index] == 1) {
                
                index = (++index >= POPULATION_SIZE ? 0 : index);
                
            }
            
            parenting[index] = 1;
            
            if (a == -1) {
                
                a = index;
                
            }else if (fitness[index] > fitness[a]) {
                
                b = a;
                a = index;
                
            }else if (b == -1) {
                
                b = index;
                
            }else if (fitness[index] > fitness[b]) {
                
                b = index;
                
            }
            
        }
        
        parents[i][0] = a;
        parents[i][1] = b;
        
    }
    
}

void recombine() {
    
    int i, j;
    
    childsCount = 0;
    
    for (i = 0; i < (POPULATION_SIZE/5); i++) {
        
        int p = (rand() % INDIVIDUAL_SIZE);
        
        if (rand() % 100 < RECOMBINATION_PROBABILITY) {
            
            int k = childsCount + POPULATION_SIZE;
            
            char *parentA = population[parents[i][0]];
            char *parentB = population[parents[i][1]];
            
            population[k] = malloc(INDIVIDUAL_SIZE * sizeof(char));
            population[k+1] = malloc(INDIVIDUAL_SIZE * sizeof(char));
            
            for (j = 0; j < INDIVIDUAL_SIZE; j++) {
                
                if (j < p) {
                    
                    population[k][j] = parentA[j];
                    population[k+1][j] = parentB[j];
                    
                }else{
                    
                    population[k][j] = parentB[j];
                    population[k+1][j] = parentA[j];
                    
                }
                
            }
            
            fitness[k] = computeIndividualFitness(population[k]);
            fitness[k+1] = computeIndividualFitness(population[k+1]);
            childsCount += 2;
            
        }
        
    }
    
}

void mutate() {
    
    int i, p;
    
    for (i = 0; i < (POPULATION_SIZE+childsCount); i++) {
        
        if (rand() % 100 < MUTATION_PROBABILITY) {
            
            p = rand() % INDIVIDUAL_SIZE;
            population[i][p] = (population[i][p] == '0' ? '1' : '0');
            fitness[i] = computeIndividualFitness(population[i]);
            
        }
        
    }
    
}

void initialize() {
    
    int i;
    
    // Generate individuals randomically.
    
    for (i = 0; i < POPULATION_SIZE; i++) {
        
        population[i] = randomIndividual();
        
    }
    
}

void computePopulationFitness() {
    
    int i;
    
    // Compute population fitness.
    // Loop through each solution (individual).
    
    for (i = 0; i < POPULATION_SIZE; i++) {
        
        fitness[i] = computeIndividualFitness(population[i]);
        
    }
    
}

void cleanup(int i, int length) {
    
    for (; i < length; i++) {
        
        free(population[i]);
        
    }
    
}

void evolve() {
    
    while (fitnessCount < MAXIMUM_FITNESS_EVALUATIONS) {
        
        selectParents();
        recombine();
        mutate();
        quicksort(fitness, 0, (POPULATION_SIZE+childsCount-1));
        cleanup(POPULATION_SIZE, (POPULATION_SIZE+childsCount));
        
    }
    
    printPopulation();
    
}

int main(int argc, const char * argv[]) {
    
    srand((unsigned int) time(NULL));
    
    initialize();
    computePopulationFitness();
    evolve();
    cleanup(0, POPULATION_SIZE);
    
    return 0;
    
}
