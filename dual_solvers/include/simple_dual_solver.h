#ifndef __SIMPLE_DUAL_SOLVER__
#define __SIMPLE_DUAL_SOLVER__

#include <cmath>

#include "dual_lagrangean_method.h"
#include "constants.h"
#include "heuristics.h"

extern bool DEBUG;

class SimpleDualSolver:DualLagrangeanMethod{
private:
    Formulation _f;
    LagrangeanFormulation _lf;

    vector<benefit_cost_pair> _primal_bc;
    vector<benefit_cost_pair> _lagrangean_bc;

    restriction_line _primal_backpack_restriction;
    restriction_line get_backpack_restriction(Formulation& f);

    vector<double> compute_gradient(bool& all_zeros,double& sum_square_g,LagrangeanFormulation& lf, solution_pair& d);

    /*Implemented virtual functions*/
    solution_pair find_primal_solution();
    solution_pair update_primal(solution_pair& p, solution_pair& d);
    solution_pair find_dual_solution(vector<double>& lbda);
    solution_pair solve_lagrangean_subproblem(Formulation& f, LagrangeanFormulation& lf, solution_pair& p,
                                      solution_pair& d, vector<double>& lbda, int max_N);

public:
    SimpleDualSolver(Formulation& p_f, LagrangeanFormulation& p_lf);;
    dual_lagrangean_solution solve(int max_N);
};

class EX_ALL_GRADIENTS_ZERO:public exception{
    virtual const char* what() const throw(){
        return "All gradients are equal to zero, no evolution on subgradient method from here.";
    }
};

void log_subgradient(int num_it, double pi, int N, vector<double>& G, double sum_square_g, 
                    double T, vector<double>& lbda, Formulation& f, LagrangeanFormulation& lf,
                    solution_pair& d, solution_pair& p, int no_improvement);

#endif