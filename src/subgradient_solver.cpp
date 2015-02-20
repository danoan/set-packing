#include "subgradient_solver.h"

void log_start(Formulation& f, LagrangeanFormulation& lf, vector<double>& lbda, solution_pair& p, solution_pair& d){
    printf("Begin Solver\n\n");

    printf("Original Formulation\n");
    printf("%s\n",f.to_str().c_str());

    printf("Lagrangean Formulation\n");
    printf("%s\n",lf.to_str().c_str());    

    printf("Initial Data\n");
    print_vector("LBDA",lbda);
    print_solution("PRIMAL",p);
    print_solution("DUAL",p);

    printf("\n");
}

void log_subgradient(int num_it, double pi, int N, vector<double>& G, double sum_square_g, 
                     double T, vector<double>& lbda, Formulation& f, LagrangeanFormulation& lf,
                     solution_pair& d, solution_pair& p, int no_improvement){

    printf("SUBGRADIENT ITERATION: %d\n",num_it);
    printf("PI: %.12lf (%d)\n",pi,N);
    print_vector("GRADIENT",G);
    printf("G SUM SQUARE: %.4lf\n", sum_square_g);
    printf("STEP SIZE: %.12lf\n", T);
    print_vector("LBDA", lbda);
    print_solution("PRIMAL",p);
    print_vector("LAGRANGEAN COSTS",lf.lagrangean_costs(),false);
    print_solution("DUAL",d);
    printf("NO IMPROVEMENT: %d\n",no_improvement);

    printf("\n");
}
