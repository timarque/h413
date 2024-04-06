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
                for (int j = 0; j < PSize; j++){// do not remove redundancy since when we get to j i it may no longer be the same components as when it  was i j since solution updated
                    checkMove(neighborhood,currentSolution, i, j);
                    newCost = computeCost(currentSolution);
                    if (newCost > cost){
                        cost = newCost;
                        improvement = true;
                    }else {
                        checkMove(neighborhood, currentSolution, j, i); // Reset last move done if was not improving
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
    int j;
    printf("Using best improvement\n");
    if (neighborhood != 1){
        while (improvement){
            improvement = false;
            for(int i = 0; i < PSize; i++){      
                if (neighborhood == 0){ // since we do not modify the solution until exploring full neihgborhood, start j at i + 1 to remove redundant exchanges
                    j = i+1;
                }else{
                    j = 0;
                }
                for (j; j < PSize; j++){
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
    printf("Using vnd\n");
    int newCost = cost;
    while (iterator < 3){
        firstImprovement(currentSolution, neighborhoods[iterator], cost);
        newCost = computeCost(currentSolution);
        if (newCost > cost){
            cost = newCost;
            iterator = 0;
        }else{
            iterator ++;
        }
        }       
}