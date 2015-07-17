#ifndef __TEST_POOL_CLIQUE__
#define __TEST_POOL_CLIQUE__

#include "test_utils.h"

#include "formulation.h"
#include "pool_clique.h"

typedef std::priority_queue<CliqueInequality, std::vector<CliqueInequality>, cliquecomp> clique_queue;

void test_pool_clique_check_order();
void test_extend_pool();
void test_extend_pool_2();
std::vector<CliqueInequality> get_unique_cliques(PoolClique& p);

void print_current_pool(PoolClique& p);

#endif