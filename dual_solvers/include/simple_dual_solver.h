#ifndef __SIMPLE_DUAL_SOLVER__
#define __SIMPLE_DUAL_SOLVER__

#include <cmath>

#include "dual_lagrangean_method.h"
#include "constants.h"
#include "heuristics.h"
#include "subgradient_method.h"

extern bool DEBUG;

class SimpleDualSolver:DualLagrangeanMethod{
private:
    Formulation _f;
    LagrangeanFormulation _lf;

    vector<benefit_cost_pair> _primal_bc;
    vector<benefit_cost_pair> _lagrangean_bc;

    ConstraintLine _primal_backpack_restriction;
    ConstraintLine get_backpack_restriction(Formulation& f);

    /*Implemented virtual functions*/
    solution_pair find_primal_solution();
    solution_pair update_primal(solution_pair& p, solution_pair& d);
    solution_pair find_dual_solution(vector<double>& lbda);
    solution_pair solve_lagrangean_subproblem(Formulation& f, LagrangeanFormulation& lf, solution_pair& p,
                                      solution_pair& d, vector<double>& lbda, int max_N);

public:
    SimpleDualSolver(Formulation& p_f);
    dual_lagrangean_solution solve(int max_N);
};

#endif