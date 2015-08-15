#ifndef __COLISION_GRAPH__
#define __COLISION_GRAPH__

#ifndef __DEBUG__ 
    #define ACCESS_MODE private:
#else
    #define ACCESS_MODE public:
#endif

#include <vector>
#include <stdio.h>

#include "types.h"
#include "lagrangean_formulation.h"
#include "solution.h"
#include "dyn_bit_cluster.h"
#include "constants.h"
#include "clique_inequality.h"


class ColisionGraph{
ACCESS_MODE
    LagrangeanFormulation& _f;    //Used to create the Ultimate ColisionGraph for this formulation

    void create_all_edges(std::vector<int>& vars_indexes);
    std::vector<DynamicBitCluster> _vertice_links;     

public:
    ColisionGraph(LagrangeanFormulation& p_f);    
    void reduce(const int& p_vertice_index);
    void subgraph(Solution& xk, const DynamicBitCluster& bit_subgraph);
    void remove(const std::vector<int>& vertices_in_clique);

    inline int num_vertices(){return _f.c().size();};

    int max_degree(int& max_degree, std::vector<bool>& vertice_marked);
    int max_degree(int& max_degree, DynamicBitCluster& bit_subgraph);

    CliqueInequality create_clique_and_constraint(const CliqueInequality& ci, std::vector<bool>& vertice_marked);
    CliqueInequality replace_constraint(const CliqueInequality& new_ci, const CliqueInequality& previous_ci);
    CliqueInequality check_new_clique(const CliqueInequality& ci, std::vector<bool>& vertice_marked);
};

#endif