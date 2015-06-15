#include <cmath>
#include <cstring>
#include <cstdlib>
#include <ctime>

#include "io.h"
#include "formulation.h"
#include "lagrangean_formulation.h"
#include "simple_dual_solver.h"
#include "minknap_dual_solver.h"

#define LONG_TYPE_INPUT 'L'
#define SHORT_TYPE_INPUT 'S'

bool DEBUG = true;

Formulation short_input(){        
    int objective_type;
    vector< vector<int> > A_index;
    vector< vector<double> > A_cost;
    vector<double> b;
    vector<double> c;
    vector<int> op;

    read_input(A_index,A_cost,b,c,op,objective_type);
    Formulation f(A_index,A_cost,b,c,op,objective_type);    

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

void single_input(char input_type, int max_N, char solver){

    Formulation f;

    if(input_type==SHORT_TYPE_INPUT){
        f = short_input();
    }else if(input_type==LONG_TYPE_INPUT){
        f = long_input();        
    }else{
        printf("You should specify a valid input type\n");
        exit(2);
    }

    dual_lagrangean_solution s;

    if(solver=='S'){
        SimpleDualSolver pls(f);    
        s = pls.solve(max_N);
    }else{
        MinknapDualSolver pls(f);    
        s = pls.solve(max_N);
    }
    
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
    char solver = 'S';
    if(argc>=3){
        max_N = atoi(argv[1]);
        input_type = argv[2][0];   
        if(argc>=4){
            DEBUG = argv[3][0]=='T';
        }
        if(argc==5){
            solver = argv[4][0];
        }
    }else{
        printf("Missing arguments\n");
        exit(1);
    }

    single_input(input_type,max_N,solver);
}
