#ifndef __CONFIG__
#define __CONFIG__

#include <string>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>

enum PrimalHeuristic{
    CARDINALITY,
    LAGRANGEAN_COST    
};

enum Solver{
    NO_CONSTRAINTS,
    KNAPSACK
};

enum InputType{
    LONG,
    SHORT
};

class Config{
public:
    static double gap_improving;
    static double pi_factor;
    static PrimalHeuristic primal_heuristic;
    static int iterations;
    static bool debug;
    static Solver solver;
    static InputType input_type;

    static int read_input(int argc, char* argv[]);
};

#endif