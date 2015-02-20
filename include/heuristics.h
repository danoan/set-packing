#ifndef __HEURISTCS__

#define __HEURISTICS__

#include <vector>
#include <algorithm>

#include "types.h"
#include "formulation.h"
#include "lagrangean_formulation.h"

using namespace std;

typedef struct _benefit_cost_pair{
    double value;
    int index;
} benefit_cost_pair;

vector<benefit_cost_pair> compute_benefit_cost(Formulation& f);
bool compare_benefit_cost(benefit_cost_pair a, benefit_cost_pair b);

solution_pair find_int_solution_by_benefit_cost_heuristic(Formulation& f);

solution_pair find_int_feasible_solution_from_dual(solution_pair& d, Formulation& f);
solution_pair find_int_solution_by_lagrangean_heuristic(LagrangeanFormulation& lf);

#endif