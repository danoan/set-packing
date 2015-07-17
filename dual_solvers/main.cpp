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

void single_input(InputType input_type, int max_N, double pi_factor, double max_no_improvement, Solver solver){

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

    dual_lagrangean_solution s;

    if(solver==NO_CONSTRAINTS){
        SimpleDualSolver pls(f,Config::debug);    
        s = pls.solve(max_N,pi_factor,max_no_improvement);
    }else{
        MinknapDualSolver pls(f,Config::debug);    
        s = pls.solve(max_N,pi_factor,max_no_improvement);
    }
    
    printf("FINAL ANSWER\n");
    print_solution("PRIMAL",s.p);
    print_solution("DUAL",s.d);    
}

int main(int argc, char* argv[]){
    srand(time(NULL));

    if(Config::read_input(argc,argv)==-1){
        return 0;
    }else{
        single_input(Config::input_type,Config::iterations,Config::pi_factor,Config::gap_improving,Config::solver);    
    }
    
    return 0;
}
