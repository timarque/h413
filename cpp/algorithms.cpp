#include <stdio.h>
#include <stdlib.h>
#include <values.h>
#include <iostream>
#include <vector> 
#include "optimisation.h" 
#include "instance.h"
#include "utilities.h"
#include "neighborhoods.h"


long *checkMove(int neighborhood, long *currentSolution){
    long int *newsol;
    if (neighborhood == 0){ // first imrpovement exchange neighborhood
        newsol = exchange(currentSolution);
    }else if (neighborhood == 1){
        newsol = transpose(currentSolution);
    }else if (neighborhood == 2){
        newsol = insert(currentSolution);
    }
    return newsol;
}

// first improvement algo
void firstImprovement(long *currentSolution, int neighborhood, int cost){
    long int *newsol;
    bool improvement = true;
    int newCost;

    printf("Using first improvement\n");
    while (improvement){
        improvement = false;
        for(int i=0; i < PSize; i++){             
            for (int j = 0; j < PSize; j++){
                newsol = checkMove(neighborhood, currentSolution); // check if there is a way to make it so vnd does not need a function 
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
    while (improvement){
        improvement = false;
        for(int i=0; i < PSize; i++){             
            for (int j = 0; j < PSize; j++){
                /* There are some more efficient way to do this, instead of recomputing cost of everything... */
                newsol = checkMove(neighborhood, currentSolution); 
                newCost = computeCost(newsol);
                if (newCost > cost){
                    improvement = true;
                    cost = newCost;
                    for (int k = 0; k < PSize; k++) {
                        memsol[k] = newsol[k]; // im not sure this is correct, do i just put current sol here or no ? 
                    }
                }
            }
            for (int k = 0; k < PSize; k++) { // this or instant change solution each time ?  maybe this but dk
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
            int j = 0;      
            while (j < PSize){
                newsol = checkMove(neighborhoods[iterator], currentSolution);
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
                    iterator = 0;
                }else{
                    if (iterator == 2){
                        iterator = 0;
                        j++;
                    }else{
                        iterator++;
                    }
                }
            }
        }
    }
}