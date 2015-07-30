#include "dual_lagrangean_method.h"

void log_start(Formulation& f, LagrangeanFormulation& lf, Solution& p, Solution& d){
    printf("Begin Solver\n\n");

    printf("Original Formulation\n");
    printf("%s\n",f.to_str().c_str());

    printf("Lagrangean Formulation\n");
    printf("%s\n",lf.to_str().c_str());    

    printf("Initial Data\n");
    // print_vector("LBDA",lbda);
    print_solution("PRIMAL",p);
    print_solution("DUAL",d);

    printf("\n");
}
