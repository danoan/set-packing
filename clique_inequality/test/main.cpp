#include "test_clique_inequality.h"
#include "test_pool_clique.h"
#include "test_dyn_bit_cluster.h"
#include "test_colision_graph.h"

int main(){
    test_correctness();
    test_dominated();
    test_hash();    

    test_dyn_stress();
    test_dyn_reduce();

    test_colision_graph_creation();
    test_colision_graph_reduce();

    test_pool_clique_check_order();
    test_extend_pool();

    test_extend_pool_2();

    return 0;
}