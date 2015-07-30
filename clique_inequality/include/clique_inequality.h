#ifndef __CLIQUE_INEQUALITY__
#define __CLIQUE_INEQUALITY__

#ifndef __DEBUG__ 
    #define ACCESS_MODE private:
#else
    #define ACCESS_MODE public:
#endif

#include <cmath>

#include <bitset>
#include <vector>

#include "constraint_line.h"
#include "types.h"

class CliqueInequality{
ACCESS_MODE
    bool _maximal_clique;
    int _total_active_variables;
    int _index_sum_active_variables;
    int _total_variables;
    int _group_number;

    std::vector<bit_cluster> _variables_in_use;    //Which variable belongs to this constraint

    ConstraintLine* _cl;    //CliqueInequality never creates a pointer. It owns a pointer created elsewhere
public:
    CliqueInequality(ConstraintLine* p_cl, int p_total_variables);
    CliqueInequality();

    inline int active_variables() const{return _total_active_variables;}
    inline int index_sum_active() const{return _index_sum_active_variables;}
    inline int set_as_maximal(){ _maximal_clique=true; };
    inline int total_variables() const{ return _total_variables;};

    inline int group_number() const{ return _group_number; };
    inline void set_group(int group_number){ _group_number = group_number; };

    inline bool is_maximal() const{ return _maximal_clique; }

    inline ConstraintLine* constraint() const{return _cl;};

    inline std::vector< ConstraintMember >::iterator begin(){ return _cl->begin(); }
    inline std::vector< ConstraintMember >::iterator end(){ return _cl->end(); }

    inline void clear(){ delete _cl;};

    bool operator==(const CliqueInequality& p_ci) const;
    //This dominates the parameter
    bool operator>(const CliqueInequality& p_ci) const;
    //This is dominated by the parameter
    bool operator<(const CliqueInequality& p_ci) const;
};

namespace std{
    template<>
    struct hash<CliqueInequality>{
        size_t operator()(const CliqueInequality& p_ci) const{
            return p_ci.active_variables()*p_ci.index_sum_active();
        }
    };
}

#endif

