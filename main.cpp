#include <cmath>
#include <cstring>
#include <cstdlib>
#include <ctime>

#include "util.h"
#include "formulation.h"
#include "lagrangean_formulation.h"
#include "pure_lagrangean_solver.h"

#define LONG_TYPE_INPUT 'L'
#define SHORT_TYPE_INPUT 'S'

bool DEBUG = true;

void single_input(char input_type, int max_N){
    vector< vector<int> > A_index;
    vector< vector<double> > A_cost;
    vector<double> b;
    vector<double> c;
    vector<int> op;
    int objective_type;

    Formulation f;
    LagrangeanFormulation lf;

    if(input_type==LONG_TYPE_INPUT){
        read_input(A_cost,b,c,op,objective_type);
        f = Formulation(A_cost,b,c,op,objective_type);
        lf = LagrangeanFormulation(A_cost,b,c,op,objective_type);
    }else if(input_type==SHORT_TYPE_INPUT){
        read_input(A_index,A_cost,b,c,op,objective_type);
        f = Formulation(A_index,A_cost,b,c,op,objective_type);
        lf = LagrangeanFormulation(A_index,A_cost,b,c,op,objective_type);
    }else{
        printf("You should specify a valid input type\n");
        exit(2);
    }

    PureLagrangeanSolver pls(f,lf);
    subgradient_solution s = pls.solve(max_N);

    printf("FINAL ANSWER\n");
    print_solution("PRIMAL",s.p);
    print_solution("DUAL",s.d);    
}

void batch_input(char input_type, int max_N){

}

int main(int argc, char* argv[]){
    srand(time(NULL));
    int max_N=30;
    char input_type;
    if(argc>=3){
        max_N = atoi(argv[1]);
        input_type = argv[2][0];   
        if(argc==4){
            DEBUG = argv[3][0]=='T';
        }
    }else{
        printf("Missing arguments\n");
        exit(1);
    }

    single_input(input_type,max_N);
}
