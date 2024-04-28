#include <chrono> 
#include <iostream>
#include <getopt.h>
#include <string.h>
#include <cstring> 
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

#include "instance.h"
#include "optimisation.h"
#include "utilities.h"
#include "algorithms.h"

char *FileName;

void readOpts(int argc, char **argv) {
    char opt;
    FileName = NULL;
    while ( (opt = getopt(argc, argv, "i:")) > 0 )  
        switch (opt) {
	    case 'i': /* Instance file */
	        FileName = (char *)malloc(strlen(optarg)+1);
	        strncpy(FileName, optarg, strlen(optarg));
	        break;
	    default:
	        fprintf(stderr, "Option %c not managed.\n", opt);
        }
    
    if ( !FileName ) {
        printf("No instance file provided (use -i <instance_name>). Exiting.\n");
        exit(1);
    } 

}

int main(int argc, char **argv){
    long int i,j;
    long int *currentSolution;
    int cost;
    std::string fileoutname, fileoutnametime, constr_heuristic,neighborhood_used, algo_used, ls_used;
    int neighborhood, ls, perturb_size, perturb_used;


    /* Do not buffer output */
    setbuf(stdout,NULL);
    setbuf(stderr,NULL);
    
    if (argc < 2) {
    printf("No instance file provided (use -i <instance_name>). Exiting.\n");
    exit(1);
    }

    readOpts(argc, argv);

    /* Read instance file */
    CostMat = readInstance(FileName);
    std::cout << "File name : " << FileName << std::endl;
    printf("Data have been read from instance file. Size of instance = %ld.\n\n", PSize);

    /* initialize random number generator, deterministically based on instance.
    * To do this we simply set the seed to the sum of elements in the matrix, so it is constant per-instance,
    but (most likely) varies between instances */
    Seed = (long int) 0;
    for (i=0; i < PSize; ++i)
      for (j=0; j < PSize; ++j)
        Seed += (long int) CostMat[i][j];
    printf("Seed used to initialize RNG: %ld.\n\n", Seed);

    // start time 
    std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now(); // Start timer
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));
    std::cout << "Start time: " << buffer << std::endl;
    
    currentSolution = (long int *)malloc(PSize * sizeof(long int));

    // do something so that is usable just with first instead of ils with 0 perturb
    if (strcmp(argv[6], "random") == 0){
        printf("Creating random initial solution \n");
        constr_heuristic = "random";
        createRandomSolution(currentSolution);
    }else if (strcmp(argv[6], "CW") == 0 || strcmp(argv[6], "cw") == 0 ){
        printf("Creating initial solution using CW heuristic \n");
        constr_heuristic = "cw";
        cheneryWatanabe(currentSolution);
    }
    printf("Initial solution:\n");
    for (int z=0; z < PSize; z++) 
    printf(" %ld", currentSolution[z]);
    printf("\n");
     /* Compute cost of solution and print it */
    cost = computeCost(currentSolution);
    printf("Cost of this initial solution: %d\n\n", cost);
    //
    // neighborhoods, 0 = exchange, 1 = transpose, 2 = insert;
    if (strcmp(argv[5],"exchange") == 0){
        neighborhood = 0;
        neighborhood_used = "ex_";
        printf("Using exchange neighborhood \n");
    }else if (strcmp(argv[5],"transpose") == 0){
        neighborhood = 1;
        neighborhood_used = "tr_";
        printf("Using transpose neighborhood \n");
    }else if (strcmp(argv[5],"insertion") == 0){
        neighborhood = 2;
        neighborhood_used = "in_";
        printf("Using insertion neighborhood \n");
    }else if (strcmp(argv[5], "tei") == 0){
        neighborhood = 3;
        neighborhood_used = "tei_";
        printf("Using transpose, exchange, insertion neighborhood ordering \n");
    }else if (strcmp(argv[5], "tie") == 0){
        neighborhood = 4;
        neighborhood_used = "tie_";
        printf("Using transpose, insertion, exchange neighborhood ordering \n");
    }else{
        fprintf(stderr, "Neighborhood provided does not exist.\n");
        return 0;
    }

    if (strcmp(argv[3], "sa") == 0){
        algo_used = "sa_";
        firstImprovement(currentSolution, neighborhood, cost, 1);
    }else if (strcmp(argv[3], "ils") == 0){
        algo_used = "ils_";
        if (strcmp(argv[4], "first") == 0){
            ls_used = "first_";
            ls = 0;
        }else if (strcmp(argv[4], "best") == 0){
            ls_used = "best_";
            ls = 1;
        }else if (strcmp(argv[4], "vnd") == 0){
            ls_used = "vnd_";
            ls = 2;
        }else{
            fprintf(stderr, "Local search provided does not exist.\n");
            return 0;
        }
        if (strcmp(argv[7], "insertion") == 0){
            perturb_used = 2;
        }else if (strcmp(argv[7], "exchange") == 0){
            perturb_used = 0;
        }else if (strcmp(argv[7], "ie") == 0){
            perturb_used = 1;
        }else{
            fprintf(stderr, "Perturbation provided does not exist.\n");
            return 0;
        }
        char *endptr;
        perturb_size = strtol(argv[8], &endptr, 10);
        ILS(currentSolution, neighborhood, cost, ls, perturb_used, perturb_size);
    }else{
        if (strcmp(argv[3], "first") == 0){
            firstImprovement(currentSolution, neighborhood, cost, 0);
            std::cout << " here " << std::endl;
        }else if (strcmp(argv[3], "best") == 0){
            bestImprovement(currentSolution, neighborhood, cost);
        }else if (strcmp(argv[3], "vnd") == 0){
            vnd(currentSolution, neighborhood, cost);
        }else{fprintf(stderr, "Algorithm provided does not exist.\n");
            return 0;    
        }
    }
    // algorithms; 


    // print here elapsed time
    std::chrono::high_resolution_clock::time_point end_time = std::chrono::high_resolution_clock::now(); // End timer
    // Calculate elapsed time in seconds
    std::chrono::seconds duration = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time); 
    // change seconds to miliseconds here for transpose if you wish to see exact time


    printf("new solution \n");
    for (j=0; j < PSize; j++) 
    printf(" %ld", currentSolution[j]);
    printf("\n");
    cost = computeCost(currentSolution);
    printf("Cost of this new solution: %d\n\n", cost);

    fileoutname = "best_known/lop_" + algo_used + neighborhood_used + constr_heuristic + ".dat";
    fileoutnametime = "best_known/lop_" + algo_used + neighborhood_used + constr_heuristic + "_times.dat";
    std::string file_name = std::string(FileName);
    file_name.erase(0,10);
    std::string write = file_name + "       " + std::to_string(cost);
    std::string write_time = file_name + " time : " + std::to_string(duration.count()) + " s"; // change here to ms if you run in milliseconds
    std::cout << write_time << std::endl;
    bool found = false;
    std::vector<std::string> lines; 
    std::ifstream file(fileoutname);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (line.find(file_name) != std::string::npos) {
                lines.push_back(write);
                found = true;
            } else {
                lines.push_back(line);
            }
        }
        file.close();
    }

    if (!found) {
        lines.push_back(write);
    }
    std::ofstream outfile(fileoutname);
    if (outfile.is_open()) {
        for (const auto& line : lines) {
            outfile << line << std::endl;
        }
        outfile.close();
    } 


    bool foundtime = false;
    std::vector<std::string> linestime;
    std::ifstream filetime(fileoutnametime);
    if (filetime.is_open()) {
        std::string line;
        while (std::getline(filetime, line)) {
            if (line.find(file_name) != std::string::npos) {
                linestime.push_back(write_time);
                foundtime = true;
            } else {
                linestime.push_back(line);
            }
        }
        filetime.close();
    }

    if (!foundtime) {
        linestime.push_back(write_time);
    }
    
    std::ofstream outfiletime(fileoutnametime);
    if (outfiletime.is_open()) {
        for (const auto& line : linestime) {
            outfiletime << line << std::endl;
        }
        outfiletime.close();
    } 
    free(currentSolution);

    return 0;
}

