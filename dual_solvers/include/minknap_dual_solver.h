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
#include "io.h"
#include "util.h"

class MinknapDualSolver:DualLagrangeanMethod{
private:
    bool _debug;
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
                                      solution_pair& d, vector<double>& lbda, int p_max_N, 
                                      double p_pi_factor, double p_max_no_improvement);

public:
    MinknapDualSolver(Formulation& p_f, bool p_debug);
    dual_lagrangean_solution solve(int p_max_N, double p_pi_factor, double p_max_no_improvement);
};

#endif