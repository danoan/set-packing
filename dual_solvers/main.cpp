#include <cmath>
#include <cstring>
#include <cstdlib>
#include <ctime>

#include "io.h"
#include "config.h"
#include "formulation.h"
#include "lagrangean_formulation.h"
#include "simple_dual_solver.h"
#include "minknap_dual_solver.h"

Formulation short_input(){        
    int objective_type;
    vector< vector<int> > A_index;
    vector< vector<double> > A_cost;
    vector<double> b;
    vector<double> c;
    vector<int> op;

    read_input(A_index,A_cost,b,c,op,objective_type);
    Formulation f(A_index,A_cost,b,c,op,objective_type);    

    // printf("AFTER CONSTRUCTOR\n");
    return f;
}

Formulation long_input(){
    int objective_type;
    vector< vector<double> > A_cost;
    vector<double> b;
    vector<double> c;
    vector<int> op;

    read_input(A_cost,b,c,op,objective_type);
    Formulation f(A_cost,b,c,op,objective_type);

    return f;
}

void single_input(InputType input_type, int max_N, double pi_factor, double max_no_improvement, Solver solver, PrimalHeuristic ph){

    // printf("DECLARING\n");
    Formulation f;

    if(input_type==SHORT){
        // printf("CALLING\n");
        f = short_input();
        // printf("RETURN\n");
    }else if(input_type==LONG){
        f = long_input();        
    }else{
        printf("You should specify a valid input type\n");
        exit(2);
    }

    std::pair<Solution,Solution> s;

    if(solver==NO_CONSTRAINTS){
        SimpleDualSolver pls(f,Config::debug);    
        s = pls.solve(max_N,pi_factor,max_no_improvement,ph==CARDINALITY?false:true);
    }else{
        MinknapDualSolver pls(f,Config::debug);    
        s = pls.solve(max_N,pi_factor,max_no_improvement,ph==CARDINALITY?false:true);
    }
    
    printf("FINAL ANSWER\n");
    print_vector("PRIMAL",s.first.best_solution());
    printf("\nVALUE: %.4lf", s.first.best_value());
    printf("\n");
    
    print_vector("DUAL",s.second.best_solution());
    printf("\nVALUE: %.4lf", s.second.best_value());    
    printf("\n");
}

int main(int argc, char* argv[]){
    srand(time(NULL));

    if(Config::read_input(argc,argv)==-1){
        return 0;
    }else{
        single_input(Config::input_type,Config::iterations,Config::pi_factor,Config::gap_improving,Config::solver,Config::primal_heuristic);    
    }
    
    return 0;
}
