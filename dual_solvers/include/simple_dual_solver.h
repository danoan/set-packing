#ifndef __SIMPLE_DUAL_SOLVER__
#define __SIMPLE_DUAL_SOLVER__

#include <cmath>
#include <utility>

#include "dual_lagrangean_method.h"
#include "constants.h"
#include "heuristics.h"
#include "subgradient_method.h"

#include "pool_clique.h"

class SimpleDualSolver:DualLagrangeanMethod{
private:
    bool _debug;
    Formulation _f;
    LagrangeanFormulation _lf;

    vector<benefit_cost_pair> _primal_bc;
    vector<benefit_cost_pair> _lagrangean_bc;

    ConstraintLine _primal_backpack_restriction;
    ConstraintLine get_backpack_restriction(Formulation& f);

    /*Implemented virtual functions*/
    void find_primal_solution(Solution& p);
    void update_primal(bool p_use_lagrangean_costs);
    void find_dual_solution(Solution& d);
    void solve_lagrangean_subproblem(Formulation& f, LagrangeanFormulation& lf, int p_max_N, 
                                      double p_pi_factor, double p_max_no_improvement,
                                      bool p_use_lagrangean_costs);

    void fixing();

public:
    SimpleDualSolver(Formulation& p_f, bool p_debug);
    std::pair<Solution,Solution> solve(int p_max_N, double p_pi_factor, double p_max_no_improvement,
                             bool p_use_lagrangean_costs);
};

#endif