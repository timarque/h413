**** Heuristic Optimization assignment.

*** To compile/clean:
make
make clean



*** To run on one instance:
./lop -i instances/instance_name algo neighborhood initial_solution_method
algo can be sa -- ils first/best/vnd  -- first/best/vnd
if you are using ils, provide random neighborhood move perturbation at the end of the file as such : neighborhood size : with size an int which will be the number of random moves applied 
examples : ./lop -i instances/N-be75eec_150 first exchange cw
./lop -i instances/N-be75eec_150 ils first exchange cw insertion 5
./lop -i instances/N-be75eec_150 sa insertion cw 
If you wish to see the current cost at different times, uncomment everything in algorithms.cpp associated with "print_times" variable
This will print the current cost of the solution every minute.


*** Best known solutions:
The best known solutions are in best_known/best_known.txt

*** The solutions for each combination are in files:
lop_algo_neighborhood_initialsolmethod.dat 
the times for each instance are in files:
lop_algo_neighborhood_initialsolmethod_times.dat 
example:
lop_best_insertion_random.dat
lop_best_insertion_random_times.dat

*** statistical test:
used code shown in assignement slides with solution files above.
best.known <- read.table ("full_path/best_known/best_known.dat")
a.cost <- read.table("full_path/instances/lop-best-ex-rand.dat")$V2 since we v1 is instance names
a.cost <- 100*(a.cost - best.known) / best.known
b.cost <- read.table("full_path/instances/lop-best-ins-rand.dat")$V2
b.cost <- 100*(b.cost - best.known) / best.known
t.test (a.cost, b.cost, paired=T)$p.value
wilcox.test (a.cost, b.cost, paired=T)$p.value
