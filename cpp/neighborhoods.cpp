#include <stdio.h>
#include <stdlib.h>
#include <values.h>
#include <iostream>
#include <vector> 
#include "optimisation.h" 
#include "instance.h"
#include "utilities.h"


long *exchange(long *solution){
    int temp, firstRandomPosition, secondRandomPosition;
    firstRandomPosition = randInt(0,PSize-1);
    secondRandomPosition = firstRandomPosition + randInt(1,(PSize-2));       
    long* currentSolution = (long *)malloc(PSize * sizeof(long)); 
    for (int k = 0; k < PSize; k++) {
        currentSolution[k] = solution[k];
    }
    if (secondRandomPosition >= PSize)
    secondRandomPosition -= PSize;
    temp = currentSolution[firstRandomPosition];
    currentSolution[firstRandomPosition] = currentSolution[secondRandomPosition];
    currentSolution[secondRandomPosition] = temp;
    return currentSolution;
}


long *transpose(long *solution){
    int temp, firstRandomPosition, secondRandomPosition;
    firstRandomPosition = randInt(0,PSize-1);
    secondRandomPosition = firstRandomPosition + randInt(1,(PSize-2));      
    if (secondRandomPosition >= PSize)
    secondRandomPosition -= PSize;
    long * currentSolution = (long *)malloc(PSize * sizeof(long)); 
    for (int k = 0; k < PSize; k++) {
        currentSolution[k] = solution[k];
    }    
    temp = currentSolution[firstRandomPosition];
    currentSolution[firstRandomPosition] = currentSolution[secondRandomPosition];
    currentSolution[secondRandomPosition] = temp;
    return currentSolution;
}

long* insert(long *solution) {
    int temp, toinsert, firstRandomPosition, secondRandomPosition;   
    firstRandomPosition = randInt(0,PSize-1);
    secondRandomPosition = firstRandomPosition + randInt(1,(PSize-2));
    if (secondRandomPosition >= PSize)
    secondRandomPosition -= PSize;
    long* currentSolution = (long *)malloc(PSize * sizeof(long)); 
    if (currentSolution == NULL) {
        // Handle allocation failure
        return NULL;
    }
    for (int k = 0; k < PSize; k++) {
        currentSolution[k] = solution[k];
    }
    toinsert = currentSolution[firstRandomPosition];
    if (secondRandomPosition < firstRandomPosition) {
        for (int z = firstRandomPosition; z > secondRandomPosition; z--) {
            currentSolution[z] = currentSolution[z - 1];
        }
    } else {
        for (int z = firstRandomPosition; z < secondRandomPosition; z++) {
            currentSolution[z] = currentSolution[z + 1];
        }
    }

    currentSolution[secondRandomPosition] = toinsert;

    return currentSolution;
}
