#ifndef __MINKNAP_DUAL_SOLVER__
#define __MINKNAP_DUAL_SOLVER__

#include <cmath>
#include <cstdio>
#include <map>

#include "dual_lagrangean_method.h"
#include "constants.h"
#include "heuristics.h"
#include "subgradient_method.h"
#include "pisinger.h"
#include "real_to_int.h"
#include "util.h"

extern bool DEBUG;

class MinknapDualSolver:DualLagrangeanMethod{
private:
    Formulation _f;
    LagrangeanFormulation _lf;

    vector<benefit_cost_pair> _primal_kc;
    vector<benefit_cost_pair> _lagrangean_kc;

    ConstraintLine* get_knapsack_constraint(Formulation& f);
    int active_constraints_for_vars(vector<int>& Ix);

    /*Implemented virtual functions*/
    solution_pair find_primal_solution();
    solution_pair update_primal(solution_pair& p, solution_pair& d);
    solution_pair find_dual_solution(vector<double>& lbda);
    solution_pair solve_lagrangean_subproblem(Formulation& f, LagrangeanFormulation& lf, solution_pair& p,
                                      solution_pair& d, vector<double>& lbda, int max_N);

public:
    MinknapDualSolver(Formulation& p_f);
    dual_lagrangean_solution solve(int max_N);
};

#endif