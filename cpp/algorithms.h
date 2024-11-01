#ifndef _ALGORITHMS_H_
#define _ALGORITHMS_H_



void exchange(long int *solution, int i, int j);
void transpose(long int *solution, int i, int j);
void insert(long int *solution, int i, int j);
void checkMove(int neighborhood, long *currentSolution, int i, int j);
void firstImprovement(long int *currentSolution, int neighborhood, int cost);
void bestImprovement(long int *currentSolution, int neighborhood, int cost);
void vnd(long int *currentSolution, int neighborhood, int cost);

#endif
