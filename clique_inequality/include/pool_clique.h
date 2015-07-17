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
    bool _pool_updated;

    std::priority_queue<CliqueInequality, std::vector<CliqueInequality>, cliquecomp> _cliques_queue;
    std::unordered_set<CliqueInequality> _cliques_access;

    CliqueInequality create_clique();

public:
    PoolClique(Formulation& p_f);

    bool add_clique(ConstraintLine* p_cl, int p_num_variables);
    bool add_clique(CliqueInequality& p_ci);
    void pop_clique();

    bool extend_pool();
    inline bool is_pool_updated(){return _pool_updated;};
};

#endif