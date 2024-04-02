#ifndef _ALGORITHMS_H_
#define _ALGORITHMS_H_

long *checkMove(int neighborhood, long *currentSolution);
void firstImprovement(long *currentSolution, int neighborhood, int cost);
void bestImprovement(long *currentSolution, int neighborhood, int cost);
void vnd(long *currentSolution, int neighborhood, int cost);

#endif
