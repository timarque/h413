#include <stdio.h>
#include <stdlib.h>
#include <values.h>
#include <iostream>
#include <vector> 
#include "optimisation.h" 
#include "instance.h"
#include "utilities.h"
#include "neighborhoods.h"


long *checkMove(int neighborhood, long *currentSolution, int i, int j){
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
    long int *newsol;
    bool improvement = true;
    int newCost;
    int start;
    printf("Using first improvement\n");
    if (neighborhood != 1){ 
    while (improvement){
        improvement = false;
        for(int i=0; i < PSize; i++){
            for (int j = 1; j < PSize; j++){
                newsol = checkMove(neighborhood, currentSolution, i, j); 
                /* Recompute cost of solution after the exchange move */
                /* There are some more efficient way to do this, instead of recomputing everything... */
                newCost = computeCost(newsol);
                if (newCost > cost){
                    improvement = true;
                    cost = newCost;
                    j = PSize;
                    for (int k = 0; k < PSize; k++) {
                        currentSolution[k] = newsol[k];
                    }
                }
            }
             
        }
    }
    }else{ // different for transpose since need to be only adjacent and checking for transpose in other loop would cause other neighborhoods to be slower
        while (improvement){
            improvement = false;
            for(int i=0; i < PSize; i++){
                newsol = checkMove(neighborhood, currentSolution, i, i + 1);
                newCost = computeCost(newsol);
                if (newCost > cost){
                    improvement = true;
                    cost = newCost;
                    i = PSize;
                    for (int k = 0; k < PSize; k++) {
                        currentSolution[k] = newsol[k];
                    }

                }
            }
        }
    }
}

// best imrpvement algo
void bestImprovement(long *currentSolution, int neighborhood, int cost){
    int newCost;
    long int *newsol;
    long int *memsol;
    memsol = (long int *)malloc(PSize * sizeof(long int));
    bool improvement = true;
    printf("Using best improvement\n");
    if (neighborhood != 1){
        while (improvement){
            improvement = false;
            for(int i=0; i < PSize; i++){      
                for (int j = 1; j < PSize; j++){
                    newsol = checkMove(neighborhood, currentSolution, i, j);
                    newCost = computeCost(newsol);
                    if (newCost > cost){
                        improvement = true;
                        cost = newCost;
                        for (int k = 0; k < PSize; k++) {
                            memsol[k] = newsol[k];
                        }
                    }
                }
                for (int k = 0; k < PSize; k++) { 
                 currentSolution[k] = memsol[k];   
                }
            
            }
        }
    }else{
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
            for (int k = 0; k < PSize; k++) { 
                 currentSolution[k] = memsol[k];   
                }
        }
        
    }
}
    


// vnd algo
void vnd(long *currentSolution, int neighborhood, int cost){
    int neighborhoods[3];
    if (neighborhood == 3){
        int neighborhoods[3] = {1,0,2}; // check if this is correct order
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
                    // i = PSize;
                    j = PSize;
                    for (int k = 0; k < PSize; k++) {
                        currentSolution[k] = newsol[k];
                    }
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