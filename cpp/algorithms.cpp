#include <stdio.h>
#include <stdlib.h>
#include <values.h>
#include <iostream>
#include <vector> 
#include "optimisation.h" 
#include "instance.h"
#include "utilities.h"


// neighborhoods also here as it reduces run time compared to newfile
long *exchange(long  int *solution, int i, int j){
    int temp;   
    long int* currentSolution = (long int *)malloc(PSize * sizeof(long int)); 
    for (int k = 0; k < PSize; k++) {
        currentSolution[k] = solution[k];
    }
    if (j >= PSize)
    j -= PSize;
    temp = currentSolution[i];
    currentSolution[i] = currentSolution[j];
    currentSolution[j] = temp;
    return currentSolution;
}

// not working correct 
long *transpose(long int *solution, int i, int j){
    int temp;
    if (j >= PSize)
    j -= PSize;
    long int* currentSolution = (long int *)malloc(PSize * sizeof(long int)); 

    for (int k = 0; k < PSize; k++) {
        currentSolution[k] = solution[k];
    }    
    temp = currentSolution[i];
    currentSolution[i] = currentSolution[j];
    currentSolution[j] = temp;

    return currentSolution;
}

long* insert(long int *solution, int i, int j) {
    int temp, toinsert;   
    if (j >= PSize)
        j -= PSize;
    long int* currentSolution = (long int *)malloc(PSize * sizeof(long int)); 
    for (int k = 0; k < PSize; k++) {
        currentSolution[k] = solution[k];
    }
    toinsert = currentSolution[i];
    if (j < i) {
        for (int z = i; z > j; z--) {
            currentSolution[z] = currentSolution[z - 1];
        }
    } else {
        for (int r = i; r < j; r++) {
            currentSolution[r] = currentSolution[r + 1];
        }
    }
    currentSolution[j] = toinsert;
    return currentSolution;
}



long *checkMove(int neighborhood, long int *currentSolution, int i, int j){
    long int *newsol;
    if (neighborhood == 0){ // first imrpovement exchange neighborhood
        newsol = exchange(currentSolution, i, j);
    }else if (neighborhood == 1){
        newsol = transpose(currentSolution, i, j);
    }
    else if (neighborhood == 2){
        newsol = insert(currentSolution, i, j);
    }
    return newsol;
}

// first improvement algo
void firstImprovement(long *currentSolution, int neighborhood, int cost){
    int newCost;
    long int *newsol;
    bool improvement = true;
    if (neighborhood != 1){
        while (improvement){
            improvement = false;
            for (int i = 0; i < PSize; i++){
                for (int j = 0; j < PSize; j++){
                    newsol = checkMove(neighborhood, currentSolution, i, j);
                    newCost = computeCost(newsol);
                    if (newCost > cost){
                        cost = newCost;
                        improvement = true;
                        for (int k=0; k < PSize; k++){
                            currentSolution[k] = newsol[k];
                        }
                        break; // break out of the loop since first improvement ( if break out of both loops it just goes on infinetly)
                    }
                }
            }
        }
    }else{ // transpose neighborhood here since different and so code can be a bit more clear above without having to make workarounds for it to work with transpose
        while (improvement){
            improvement = false;
            for(int i=0; i < PSize; i++){
                newsol = checkMove(neighborhood, currentSolution, i, i + 1);
                newCost = computeCost(newsol);
                if (newCost > cost){
                    improvement = true;
                    cost = newCost;
                    for (int k=0; k < PSize; k++){
                        currentSolution[k] = newsol[k];
                    }
                    break;
                }
            }
        }
        
    }
}

// best imrpvement algo
void bestImprovement(long int *currentSolution, int neighborhood, int cost){
    int newCost;
    long int *newsol;
    long int *memsol;
    memsol = (long int *)malloc(PSize * sizeof(long int));
    bool improvement = true;
    printf("Using best improvement\n");
    if (neighborhood != 1){
        while (improvement){
            improvement = false;
            int k = 0, l = 0;
            for(int i=0; i < PSize; i++){      
                for (int j = 0; j < PSize; j++){
                    newsol = checkMove(neighborhood, currentSolution, i, j);
                    newCost = computeCost(newsol);
                    if (newCost > cost){
                        improvement = true;
                        cost = newCost;

                        for (int k = 0; k < PSize; k++) {
                            memsol[k] = newsol[k]; // memorising best solution found so far
                        }
                    }
                }
            }
            if (improvement){
                for (int k=0; k < PSize; k++){
                    currentSolution[k] = memsol[k];
                }
            }
        }
    }else{// transpose neighborhood here since different and so code can be a bit more clear above without having to make workarounds for it to work with transpose
        while (improvement){
            improvement = false;
            for(int i=0; i < PSize; i++){
                newsol = checkMove(neighborhood, currentSolution, i, i + 1);
                newCost = computeCost(newsol);
                if (newCost > cost){
                    improvement = true;
                    cost = newCost;
                    for (int k = 0; k < PSize; k++) {
                        memsol[k] = newsol[k];
                    }
                }
            }
            if (improvement){
                for (int k=0; k < PSize; k++){
                    currentSolution[k] = memsol[k];
                }
            }
        }
        
    }
}
    

// vnd algo
void vnd(long int *currentSolution, int neighborhood, int cost){

    int neighborhoods[3];
    if (neighborhood == 3){
        int neighborhoods[3] = {1,0,2};
    }else{
         int neighborhoods[3] = {1,2,0};
    }
    int iterator = 0;
    long int *newsol;
    bool improvement = true;
    int newCost;
    printf("Using vnd\n");
    bool cont = false;
    while (improvement){
        improvement = false;
        for(int i=0; i < PSize; i++){ 
            int j = 1;
            iterator = 0;  
            while (j < PSize){
                newsol = checkMove(neighborhoods[iterator], currentSolution, i, j);
                /* Recompute cost of solution after the exchange move */
                /* There are some more efficient way to do this, instead of recomputing everything... */                
                newCost = computeCost(newsol);
                if (newCost > cost){
                    improvement = true;
                    cost = newCost;
                    for (int k=0; k < PSize; k++){
                        currentSolution[k] = newsol[k];
                    }
                    j = PSize;                    
                }else{
                    if (iterator == 2){ // skip transpose since already tested for this index i (its the first tested in  both cases)
                        iterator = 1;
                        j++;
                    }else{
                        iterator++;
                    }
                }
            }
        }
    }
}