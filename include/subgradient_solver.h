#ifndef __SUBGRADIENT_SOLVER__
#define __SUBGRADIENT_SOLVER__

#include <vector>
#include <cstdio>
#include <exception>

using namespace std;

#include "types.h"
#include "util.h"
#include "formulation.h"
#include "lagrangean_formulation.h"

typedef struct _subgradient_solution{
    solution_pair p;
    solution_pair d;
    vector<double> lbda;
} subgradient_solution;

class SubgradientSolver{
private:
    virtual solution_pair find_primal_solution(Formulation& f) = 0;
    virtual solution_pair update_primal(solution_pair& p, solution_pair& d, 
                                        Formulation& f,LagrangeanFormulation& lf) = 0;
    virtual solution_pair find_dual_solution(LagrangeanFormulation& lf, vector<double>& lbda) = 0;
    virtual vector<double> compute_gradient(bool& all_zeros,double& sum_square_g,
                                            LagrangeanFormulation& lf, solution_pair& d) = 0;
    virtual solution_pair subgradient(Formulation& f, LagrangeanFormulation& lf, solution_pair& p,
                                      solution_pair& d, vector<double>& lbda, int max_N) = 0;
public:
    virtual subgradient_solution solve(int max_N) = 0;
};

class EX_ALL_GRADIENTS_ZERO:public exception{
    virtual const char* what() const throw(){
        return "All gradients are equal to zero, no evolution on subgradient method from here.";
    }
};

class EX_NO_LBDA_IMPROVE_SOLUTION:public exception{
    virtual const char* what() const throw(){
        return "There is no lbda that improves the current best solution.";
    }
};

void log_start(Formulation& f, LagrangeanFormulation& lf, vector<double>& lbda, solution_pair& p, solution_pair& d);

void log_subgradient(int num_it, double pi, int N, vector<double>& G, double sum_square_g, 
                     double T, vector<double>& lbda, Formulation& f, LagrangeanFormulation& lf,
                     solution_pair& d, solution_pair& p, int no_improvement);

#endif