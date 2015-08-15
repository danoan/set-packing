#ifndef __HEURISTICS__

#define __HEURISTICS__

#include <vector>
#include <algorithm>

#include "types.h"
#include "formulation.h"
#include "solution.h"
#include "lagrangean_formulation.h"
#include "constraint_line.h"

using namespace std;

bool compare_benefit_cost(BenefitCostPair a, BenefitCostPair b);

vector<BenefitCostPair> compute_benefit_cost(ConstraintLine& br, vector<double>& cost_vector);

void find_primal_int_solution_by_benefit_cost_heuristic(Formulation& f, vector<BenefitCostPair>& bc_vector, Solution& d);
void find_primal_int_feasible_solution_from_dual(Solution& d, Formulation& f,vector<BenefitCostPair>& bc_vector, Solution& p);

void find_int_optimal_solution_lagrangean_subproblem(LagrangeanFormulation& lf, Solution& d,  const int fixed_variable=-1, const int fixed_value=-1);


bool local_search(Formulation& lf, Solution& p, int deep);
void local_search_it(Formulation& lf, Solution& p, Solution& best_solution, const int& deep, int curr_deep);

#endif