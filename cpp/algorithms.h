#ifndef _ALGORITHMS_H_
#define _ALGORITHMS_H_

long *checkMove(int neighborhood, long *currentSolution, int i, int j);
void firstImprovement(long int *currentSolution, int neighborhood, int cost);
void bestImprovement(long int *currentSolution, int neighborhood, int cost);
void vnd(long int *currentSolution, int neighborhood, int cost);

#endif
