#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "optimisation.h" 
#include "instance.h"
#include "utilities.h"


void exchange(long  int *solution, int i, int j){
    int temp;   
    if (j >= PSize){
        j -= PSize;
    }
    temp = solution[i];
    solution[i] = solution[j];
    solution[j] = temp;
}

// not working correct 
void transpose(long int *solution, int i, int j){
    int temp;
    if (j >= PSize){
        j -= PSize;
    }
    if (i >= PSize){    
        i -= PSize;
    }
    temp = solution[i];
    solution[i] = solution[j];
    solution[j] = temp;

}

void insert(long int *solution, int i, int j) {
    int temp, toinsert;   
    if (j >= PSize){
        j -= PSize;
    }
    toinsert = solution[i];
    if (j < i) {
        for (int z = i; z > j; z--) {
            solution[z] = solution[z - 1];
        }
    } else {
        for (int r = i; r < j; r++) {
            solution[r] = solution[r + 1];
        }
    }
    solution[j] = toinsert;
}

void checkMove(int neighborhood, long *currentSolution, int i, int j){
    if (neighborhood == 0){
        exchange(currentSolution, i, j);
    }else if (neighborhood == 1){
        transpose(currentSolution, i, j);
    }else {
        insert(currentSolution, i, j);
    }
}

// first improvement algo
void firstImprovement(long *currentSolution, int neighborhood, int cost){ 
    int newCost;
    bool improvement = true;
    if (neighborhood != 1){
        while (improvement){
            improvement = false;
            for (int i = 0; i < PSize; i++){
                for (int j = 0; j < PSize; j++){ // this will have redundancy for exchange but should not be too bad
                    checkMove(neighborhood,currentSolution, i, j);
                    newCost = computeCost(currentSolution);
                    if (newCost > cost){
                        cost = newCost;
                        improvement = true;
                    }else {
                        checkMove(neighborhood, currentSolution, j, i); // Reset the exchange
                    }
                    
                }
            }
        }
    }else{  
        while (improvement){
            improvement = false;
            for(int i=0; i < PSize; i++){ // same as 2 loops
                transpose( currentSolution, i, i + 1);
                newCost = computeCost(currentSolution);
                if (newCost > cost){
                    improvement = true;
                    cost = newCost;
                }else{
                    transpose(currentSolution, i + 1, i);
                }
            }
        }
        
    }
}

// best imrpvement algo
void bestImprovement(long int *currentSolution, int neighborhood, int cost){
    int newCost;
    int memi, memj;
    bool improvement = true;
    printf("Using best improvement\n");
    if (neighborhood != 1){
        while (improvement){
            improvement = false;
            for(int i=0; i < PSize; i++){      
                for (int j = 0; j < PSize; j++){
                    checkMove(neighborhood, currentSolution, i, j);
                    newCost = computeCost(currentSolution);
                    if (newCost > cost){
                        improvement = true;
                        cost = newCost;
                        memi = i;
                        memj = j;
                    }
                    checkMove(neighborhood,currentSolution, j, i);
                }
            }
            if (improvement){
                checkMove(neighborhood,currentSolution, memi, memj);
            }
        }
    }else{// transpose neighborhood here since different and so code can be a bit more clear above without having to make workarounds for it to work with transpose
        while (improvement){
            improvement = false;
            for(int i=0; i < PSize; i++){
                transpose(currentSolution, i, i + 1);
                newCost = computeCost(currentSolution);
                if (newCost > cost){
                    improvement = true;
                    cost = newCost;
                    memi = i;
                }
                transpose( currentSolution, i + 1, i);
            }
            if (improvement){
                transpose(currentSolution, memi, memi + 1);
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
    bool improvement = true;
    int newCost;
    printf("Using vnd\n");
    bool cont = false;
    while (improvement){
        improvement = false;
        for(int i=0; i < PSize; i++){ 
            int j = 0;
            iterator = 0;  
            while (j < PSize){
                checkMove(neighborhoods[iterator], currentSolution, i, j);
                /* Recompute cost of solution after the exchange move */
                /* There are some more efficient way to do this, instead of recomputing everything... */                
                newCost = computeCost(currentSolution);
                if (newCost > cost){
                    improvement = true;
                    cost = newCost;
                    for (int k=0; k < PSize; k++){
                        currentSolution[k] = currentSolution[k];
                    }               
                }else{
                    checkMove(neighborhoods[iterator], currentSolution, j, i);
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