#ifndef __DUAL_LAGRANGEAN_METHOD__
#define __DUAL_LAGRANGEAN_METHOD__

#include <vector>
#include <cstdio>
#include <exception>
#include <utility>

using namespace std;

#include "types.h"
#include "io.h"
#include "formulation.h"
#include "lagrangean_formulation.h"
#include "subgradient_method.h"
#include "solution.h"

class DualLagrangeanMethod{
private:
    virtual void find_primal_solution(Solution& p) = 0;
    virtual bool update_primal(bool p_use_lagrangean_costs) = 0;
    virtual void find_dual_solution(Solution& d) = 0;
    virtual void solve_lagrangean_subproblem(Formulation& f, LagrangeanFormulation& lf, int p_max_N, 
                                      double p_pi_factor, double p_max_no_improvement,
                                      bool p_use_lagrangean_costs) = 0;

    std::vector<int> _previous_active_constraints;    
protected:
    Solution _dual;
    Solution _primal;
    
public:
    DualLagrangeanMethod(const int& num_components, const int& objective_type):
                        _primal( Solution(num_components,objective_type==MAX_TYPE?LOWER_BOUND:UPPER_BOUND) ),
                        _dual( Solution(num_components, objective_type==MAX_TYPE?UPPER_BOUND:LOWER_BOUND) ){};

    virtual std::pair<Solution,Solution> solve(int p_max_N, double p_pi_factor, double p_max_no_improvement, bool p_use_lagrangean_costs) = 0;
    void print_clique(ostringstream& os,ConstraintLine* cl,LagrangeanFormulation& lf);
    void log(SubgradientMethod& sm, LagrangeanFormulation& lf);
};

void log_start(Formulation& f, LagrangeanFormulation& lf, Solution& p, Solution& d);


#endif