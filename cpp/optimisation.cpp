/*  Heuristic Optimization assignment, 2015.
    Adapted by Jérémie Dubois-Lacoste from the ILSLOP implementation
    of Tommaso Schiavinotto:
    ---
    ILSLOP Iterated Lcaol Search Algorithm for Linear Ordering Problem
    Copyright (C) 2004  Tommaso Schiavinotto (tommaso.schiavinotto@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <values.h>
#include <iostream>
#include <vector> 
#include "optimisation.h" 
#include "instance.h"
#include "utilities.h"

#ifdef __MINGW32__
#include <float.h>
#define MAX_FLOAT FLT_MAX
#else
#define MAX_FLOAT MAXFLOAT
#endif


long int **CostMat;


long long int computeCost(long int *s) {
    int h,k;
    long long int sum;
    for (sum = 0, h = 0; h < PSize; h++ )
	for ( k = h + 1; k < PSize; k++ )
	    sum += CostMat[s[h]][s[k]];
    return sum;
}


long long int computeAtract(int row) { // attractiveness of a row for chenery watanabe ( sum of all values of the row )
    long long int sum = 0;
    for (int j = 0; j < PSize; j++){
        sum += CostMat[row][j];
    }
    return sum;
}

void createRandomSolution(long int *s) {
    int j; 
    long int *random;

    random = generate_random_vector(PSize);
    for ( j = 0 ; j < PSize ; j++ ) {
      s[j] = random[j];
    }
    free ( random );
}

// check if this is correct
void cheneryWatanabe(long int *s){
    long int* usable;
    long int* empty;
    int cost;
    empty = (long int *)malloc(PSize * sizeof(long int));
    usable = (long int *)malloc(PSize * sizeof(long int));
    for ( int r = 0 ; r < PSize; r++)
        usable[r] = r;
    for (int i=0; i < PSize; i++){
        int ind = 0, max = 0;
        for (int j=0; j < PSize; j++){
            if (usable[j] != -1){
                empty[i] = usable[j];
                cost = computeAtract(empty[i]);
                if (cost > max){
                    max = cost;
                    ind = j;
                
                }
            }
        }
        empty[i] = usable[ind];
        s[i] = empty[i];
        usable[ind] = -1;
    }
    free (empty);
    free (usable);
}

