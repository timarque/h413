#ifndef _ALGORITHMS_H_
#define _ALGORITHMS_H_



long *exchange(long int *solution, int i, int j);
long *transpose(long int *solution, int i, int j);
long *insert(long int *solution, int i, int j);
long *checkMove(int neighborhood, long *currentSolution, int i, int j);
void firstImprovement(long int *currentSolution, int neighborhood, int cost);
void bestImprovement(long int *currentSolution, int neighborhood, int cost);
void vnd(long int *currentSolution, int neighborhood, int cost);

#endif
