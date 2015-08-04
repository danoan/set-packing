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

#include "pool_clique.h"

class MinknapDualSolver:DualLagrangeanMethod{
private:
    bool _debug;

    Formulation _f;
    LagrangeanFormulation _lf;

    vector<BenefitCostPair> _primal_kc;
    vector<BenefitCostPair> _lagrangean_kc;

    ConstraintLine* _primal_knapsack_restriction;
    ConstraintLine* get_knapsack_constraint(Formulation& f);
    int active_constraints_for_vars(vector<int>& Ix, const int& index_variable,const int& fixed_value);

    /*Implemented virtual functions*/
    void find_primal_solution(Solution& p);
    bool update_primal(bool p_use_lagrangean_costs);
    void find_dual_solution(Solution& d){ find_dual_solution(d,-1,-1); }
    void find_dual_solution(Solution& d, const int& fixed_variable, const int& fixed_value);
    void solve_lagrangean_subproblem(Formulation& f, LagrangeanFormulation& lf,
                                      int p_max_N, double p_pi_factor, double p_max_no_improvement,
                                      bool p_use_lagrangean_costs);

    void fixing();

public:
    MinknapDualSolver(Formulation& p_f, bool p_debug);
    std::pair<Solution,Solution> solve(int p_max_N, double p_pi_factor, double p_max_no_improvement,
                             bool p_use_lagrangean_costs);
};

#endif