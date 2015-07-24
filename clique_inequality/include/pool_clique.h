#ifndef __POOL_CLIQUE__
#define __POOL_CLIQUE__

#ifndef __DEBUG__ 
    #define ACCESS_MODE private:
#else
    #define ACCESS_MODE public:
#endif

#include <queue>
#include <unordered_set>

#include "colision_graph.h"
#include "clique_inequality.h"
#include "constraint_line.h"
#include "formulation.h"

struct cliquecomp{
    bool operator()(const CliqueInequality& a, const CliqueInequality& b) const{
        return (a.is_maximal()?-1:1)*a.active_variables()>b.active_variables();
    }
};


class PoolClique{
ACCESS_MODE
    Formulation& _f;
    ColisionGraph _cg;
    int _replaced_constraints;
    int _new_constraints;

    std::unordered_set<CliqueInequality> _cliques_access;

    CliqueInequality create_clique_inequality(ConstraintLine** cl, std::vector<int>& vertices_in_clique);
    std::vector<int> find_clique(int seed, ColisionGraph temp_cg, std::vector<bool> vertice_marked);

public:
    PoolClique(Formulation& p_f);

    bool add_clique(ConstraintLine* p_cl, int p_num_variables);
    bool add_clique(CliqueInequality& p_ci);

    int extend_pool(solution_pair& xk);

    inline int replaced_constraints(){return _replaced_constraints;};
    inline int new_constraints(){return _new_constraints;};
};

#endif