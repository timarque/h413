#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "optimisation.h" 
#include "instance.h"
#include "utilities.h"
#include <chrono> 
#include <cmath>
#include <random> 
#include <algorithm> 

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
// this modifies solution itself so needs to be called again with inverted indices to reset solution
void checkMove(int neighborhood, long *currentSolution, int i, int j){ 
    if (neighborhood == 0){
        exchange(currentSolution, i, j);
    }else if (neighborhood == 1){
        transpose(currentSolution, i, j);
    }else{
        insert(currentSolution, i, j);
    }
}

// first improvement algo
void firstImprovement(long *currentSolution, int neighborhood, int cost, int algo){ 
    int newCost;
    if ( algo !=  0 ){
        std::cout << "SA" << std::endl;
        float temp = 10000000.0; // 1M seems to be the best for first 2 instances.
        float alpha = 0.99; // geometric cooling
        float min_temp = 0.0;
        float accept;
        int delta;
        int x, y;
        bool stop = false;
        std::uniform_real_distribution<> dis(0.0, 1.0);
        std::chrono::seconds algo_duration(800);
        //std::chrono::seconds print_times(60);
        std::chrono::high_resolution_clock::time_point algo_time = std::chrono::high_resolution_clock::now(); // Start timer
        std::chrono::high_resolution_clock::time_point passed_time = std::chrono::high_resolution_clock::now();
        std::chrono::seconds duration = std::chrono::duration_cast<std::chrono::seconds>(passed_time - algo_time);
        bool improvement = false; 
        while (!stop){
            std::vector<int> i_used(PSize);
            for (int r = 0; r < PSize; ++r) {
                i_used[r] = r;
            }
            for (int i = 0; i < PSize; i++){
                std::vector<int> j_used(PSize);
                for (int z = 0; z < PSize; ++z) {
                    j_used[z] = z;
                }
                x = randInt(0, i_used.size() - 1);
                i_used.erase(i_used.begin() + x); 
                for (int j = 0; j < PSize; j++){// do not remove redundancy since when we get to j i it may no longer be the same components as when it  was i j since solution updated
                    y = randInt(0, j_used.size() - 1);
                    j_used.erase(j_used.begin() + y );
                    checkMove(neighborhood,currentSolution, x, y);
                    newCost = computeCost(currentSolution);
                    if (newCost > cost){
                        cost = newCost;
                        improvement = true;
                        //break;
                    }else{
                        accept = (float)(ran01(&Seed)); 
                        delta = cost - newCost;
                        if (exp(-delta/temp) < accept){
                             checkMove(neighborhood, currentSolution, y, x); // Reset last move done 
                        }else{ // accept solution if >= to random number "accept"
                            cost = newCost;
                        }
                    }
                }
            }
            temp = temp * alpha;
            passed_time = std::chrono::high_resolution_clock::now();
            // Calculate elapsed time in seconds
            duration = std::chrono::duration_cast<std::chrono::seconds>(passed_time - algo_time);
            if (algo == 1){
                if (duration > algo_duration){
                    stop = true;
                    std::cout << delta <<" " <<exp(-delta/temp) << " " << accept << std::endl;
                }
                // if (duration >= print_times){
                //     std::cout << cost << std::endl;
                //     print_times += std::chrono::seconds(60);
                // }
            }else{
                if (!improvement){
                    stop = true;
                }
            }
        }
    }else{
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
                            //break;
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
    int newCost = cost;
    while (iterator < 3){
        firstImprovement(currentSolution, neighborhoods[iterator], cost, 0);
        newCost = computeCost(currentSolution);
        if (newCost > cost){
            cost = newCost;
            iterator = 0;
        }else{
            iterator ++;
        }
    }       
}


void perturb(long int* currentSolution, int perturb_used, int perturb_size){
    int x,y;
    
    for (int i = 0; i < perturb_size; i++){
        x = randInt(0,PSize - 1);
        y = randInt(0,PSize - 1);
        if (perturb_used == 0){
            exchange(currentSolution, x, y);
        }else if (perturb_used == 2){
            insert(currentSolution, x, y);
        }else{
            if (i > perturb_size / 2){
                exchange(currentSolution, x, y);
            }else{
                insert(currentSolution, x, y);
            }
        }
    }
}

 // ILS
void ILS(long int *currentSolution, int neighborhood, int cost, int ls, int perturb_used, int perturb_size){
    bool stop = false;
    std::chrono::seconds algo_duration(800);
    //std::chrono::seconds print_times(60);
    std::chrono::high_resolution_clock::time_point algo_time = std::chrono::high_resolution_clock::now(); // Start timer
    std::chrono::high_resolution_clock::time_point passed_time = std::chrono::high_resolution_clock::now();
    std::chrono::seconds duration = std::chrono::duration_cast<std::chrono::seconds>(passed_time - algo_time); 
    int best_so_far_cost = cost;
    long int *best_so_far = (long int *)malloc(PSize * sizeof(long int));
    for (int i = 0; i < PSize; i++){// resetting to best_solution found so far
        best_so_far[i] = currentSolution[i]; 
     }
    std::cout << " ILS " << std::endl;
    while(!stop){
        perturb(currentSolution, perturb_used, perturb_size); 
        if (ls == 0){
            firstImprovement(currentSolution, neighborhood, cost, 0);
        }else if (ls == 1){
            bestImprovement(currentSolution, neighborhood, cost);
        }else{
            vnd(currentSolution, neighborhood, cost);
        }
        cost = computeCost(currentSolution);
        if (cost < best_so_far_cost){ // reject if not better after perturb + ls 
            for (int i = 0; i < PSize; i++){// resetting to best_solution found so far
                currentSolution[i] = best_so_far[i]; 
            }
        }else{
            for (int i = 0; i < PSize; i++){// resetting to best_solution found so far
                best_so_far[i] = currentSolution[i]; 
            }
            
            best_so_far_cost = cost;
        }
        passed_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::seconds>(passed_time - algo_time); 
        if (duration > algo_duration){
            stop = true;
        }
        // if (duration >= print_times){
        //     std::cout << best_so_far_cost << std::endl;
        //     print_times += std::chrono::seconds(60);
        // }
    }
    free(best_so_far);
 }