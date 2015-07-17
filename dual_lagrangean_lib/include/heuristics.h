#ifndef __HEURISTICS__

#define __HEURISTICS__

#include <vector>
#include <algorithm>

#include "types.h"
#include "formulation.h"
#include "lagrangean_formulation.h"
#include "constraint_line.h"

using namespace std;

typedef struct _benefit_cost_pair{
    double value;
    int index;
} benefit_cost_pair;
bool compare_benefit_cost(benefit_cost_pair a, benefit_cost_pair b);

vector<benefit_cost_pair> compute_benefit_cost(ConstraintLine& br, vector<double>& cost_vector);

solution_pair find_primal_int_solution_by_benefit_cost_heuristic(Formulation& f, vector<benefit_cost_pair>& bc_vector);
solution_pair find_primal_int_feasible_solution_from_dual(solution_pair& d, Formulation& f,vector<benefit_cost_pair>& bc_vector);

solution_pair find_int_optimal_solution_lagrangean_subproblem(LagrangeanFormulation& lf);


#endif