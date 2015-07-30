#ifndef __RESTRICTION_LINE__
#define __RESTRICTION_LINE__

#include <vector>
#include <unordered_map>
#include <stdio.h>

#define CONSTR_HASH_MOD 1<<15

typedef struct _restriction_member{
    int index;
    double cost;
} ConstraintMember;

class ConstraintLine{
private:
    static int _next_id;

    std::vector< ConstraintMember > _members;
    double _rhs;
    int _op;
    int _id;
    double _lbda;
    bool _original;
    

    void copy_line(ConstraintLine* p_rl);

public:
    ConstraintLine(bool p_original=false):_original(p_original){ _id = _next_id++;};
    ConstraintLine(ConstraintLine* p_rl);
    ConstraintLine(std::vector< ConstraintMember > &p_members, double p_rhs, int p_op, bool p_original=false);

    inline double rhs(){return _rhs;}
    inline void rhs(double p_rhs){ _rhs=p_rhs;}

    inline int op(){return _op;}
    inline void op(int p_op){_op=p_op;}

    inline double lbda(){ return _lbda; };
    inline void lbda(double lbda){ _lbda=lbda; };

    inline int index() const{return _id;}
    inline bool original() const{ return _original; }

    inline void add(ConstraintMember& p_rlm){ _members.push_back(p_rlm); }
    inline size_t size(){return _members.size();};

    inline void replace(std::vector< ConstraintMember >& p_member){ _members=p_member; }

    inline std::vector< ConstraintMember >::iterator begin(){ return _members.begin(); }
    inline std::vector< ConstraintMember >::iterator end(){ return _members.end(); }

    inline bool operator==(const ConstraintLine& p_ci) const{ return p_ci.index()==_id;};
};

typedef std::unordered_map< int, ConstraintLine* >::iterator line_it;
typedef std::vector< ConstraintMember >::iterator member_it;

#endif