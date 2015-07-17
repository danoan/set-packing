#ifndef __TEST_CLIQUE_INEQUALITY__
#define __TEST_CLIQUE_INEQUALITY__

#include "test_utils.h"
#include "formulation.h"
#include "clique_inequality.h"

#include <vector>
#include <bitset>

typedef struct _ci_expected_data{
    bool maximal_clique;
    int total_active_variables;
    int index_sum_active_variables;

    std::vector<bit_cluster> variables_in_use;
} ci_expected_data;

ci_expected_data create_data(bool maximal_clique,int total_active_variables,
                             int index_sum_active_variables,const int* r,
                             int total_variables_form);

void test_correctness();
void test_dominated();
void test_hash();

#endif