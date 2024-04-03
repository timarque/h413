#include <stdio.h>
#include <stdlib.h>
#include <values.h>
#include <iostream>
#include <vector> 
#include "optimisation.h" 
#include "instance.h"
#include "utilities.h"


long *exchange(long *solution, int i, int j){
    int temp;   
    long* currentSolution = (long *)malloc(PSize * sizeof(long)); 
    for (int k = 0; k < PSize; k++) {
        currentSolution[k] = solution[k];
    }
    if (j >= PSize)
    j -= PSize;
    temp = currentSolution[i];
    currentSolution[i] = currentSolution[j];
    currentSolution[j] = temp;
    for (int z=0; z < PSize; z++) 
    printf(" %ld", currentSolution[z]);
    printf("\n");
    return currentSolution;
}

// not working correct 
long *transpose(long *solution, int i, int j){
    int temp;
    if (j >= PSize)
    j -= PSize;
    long * currentSolution = (long *)malloc(PSize * sizeof(long)); 

    for (int k = 0; k < PSize; k++) {
        currentSolution[k] = solution[k];
    }    
    temp = currentSolution[i];
    currentSolution[i] = currentSolution[j];
    currentSolution[j] = temp;

    return currentSolution;
}

long* insert(long *solution, int i, int j) {
    int temp, toinsert;   
    if (j >= PSize)
    j -= PSize;
    long* currentSolution = (long *)malloc(PSize * sizeof(long)); 
    if (currentSolution == NULL) {
        // Handle allocation failure
        return NULL;
    }
    for (int k = 0; k < PSize; k++) {
        currentSolution[k] = solution[k];
    }
    toinsert = currentSolution[i];
    if (j < i) {
        for (int z = i; z > j; z--) {
            currentSolution[z] = currentSolution[z - 1];
        }
    } else {
        for (int z = i; z < j; z++) {
            currentSolution[z] = currentSolution[z + 1];
        }
    }

    currentSolution[j] = toinsert;
    return currentSolution;
}
