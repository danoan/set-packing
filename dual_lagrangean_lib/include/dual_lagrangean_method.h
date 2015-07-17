#ifndef __DUAL_LAGRANGEAN_METHOD__
#define __DUAL_LAGRANGEAN_METHOD__

#include <vector>
#include <cstdio>
#include <exception>

using namespace std;

#include "types.h"
#include "io.h"
#include "formulation.h"
#include "lagrangean_formulation.h"

typedef struct _dual_lagrangean_solution{
    solution_pair p;
    solution_pair d;
    vector<double> lbda;
} dual_lagrangean_solution;

class DualLagrangeanMethod{
private:
    virtual solution_pair find_primal_solution() = 0;
    virtual solution_pair update_primal(solution_pair& p, solution_pair& d) = 0;
    virtual solution_pair find_dual_solution(vector<double>& lbda) = 0;
    virtual solution_pair solve_lagrangean_subproblem(Formulation& f, LagrangeanFormulation& lf, solution_pair& p,
                                      solution_pair& d, vector<double>& lbda, int p_max_N, 
                                      double p_pi_factor, double p_max_no_improvement) = 0;
public:
    virtual dual_lagrangean_solution solve(int p_max_N, double p_pi_factor, double p_max_no_improvement) = 0;
};

void log_start(Formulation& f, LagrangeanFormulation& lf, vector<double>& lbda, solution_pair& p, solution_pair& d);


#endif