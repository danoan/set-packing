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
#include "formulation.h"
#include "dyn_bit_cluster.h"
#include "constants.h"
#include "clique_inequality.h"


class ColisionGraph{
ACCESS_MODE
    Formulation& _f;    //Used to create the Ultimate ColisionGraph for this formulation

    std::vector<DynamicBitCluster> _vertice_links;

    void create_all_edges(std::vector<int>& vars_indexes);

public:
    ColisionGraph(Formulation& p_f);    
    void reduce(int p_vertice_index);
    inline int num_vertices(){return _f.c().size();};

    int max_degree(int& max_degree, std::vector<bool>& vertice_marked);

    CliqueInequality create_clique_and_constraint(const CliqueInequality& ci, std::vector<bool>& vertice_marked);
    CliqueInequality check_new_clique(const CliqueInequality& ci, std::vector<bool>& vertice_marked);
};

#endif