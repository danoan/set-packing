#ifndef __PURE_LAGRANGEAN_SOLVER__
#define __PURE_LAGRANGEAN_SOLVER__

#include <cmath>

#include "subgradient_solver.h"
#include "constants.h"
#include "heuristics.h"

extern bool DEBUG;

class PureLagrangeanSolver:SubgradientSolver{
private:
    Formulation _f;
    LagrangeanFormulation _lf;

    solution_pair find_primal_solution(Formulation& f);
    solution_pair update_primal(solution_pair& p, solution_pair& d, 
                                        Formulation& f,LagrangeanFormulation& lf);
    solution_pair find_dual_solution(LagrangeanFormulation& lf, vector<double>& lbda);
    vector<double> compute_gradient(bool& all_zeros,double& sum_square_g,
                                            LagrangeanFormulation& lf, solution_pair& d);
    solution_pair subgradient(Formulation& f, LagrangeanFormulation& lf, solution_pair& p,
                                      solution_pair& d, vector<double>& lbda, int max_N);        

public:
    PureLagrangeanSolver(Formulation& p_f, LagrangeanFormulation& p_lf):_f(p_f),_lf(p_lf){};
    subgradient_solution solve(int max_N);
};

#endif