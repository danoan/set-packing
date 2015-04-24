#ifndef __RESTRICTION_LINE__
#define __RESTRICTION_LINE__

#include <vector>

typedef struct _restriction_member{
    int index;
    double cost;
} RestrictionMember;

class RestrictionLine{
private:
    std::vector< RestrictionMember > _members;

    double _rhs;
    int _op;

    void copy_line(const RestrictionLine& p_rl);

public:
    RestrictionLine(){};
    RestrictionLine(const RestrictionLine& p_rl);
    RestrictionLine& operator=(const RestrictionLine& p_rl);

    inline double rhs(){return _rhs;}
    inline void rhs(double p_rhs){ _rhs=p_rhs;}

    inline int op(){return _op;}
    inline void op(int p_op){_op=p_op;}

    inline void add(RestrictionMember& p_rlm){ _members.push_back(p_rlm); }

    inline std::vector< RestrictionMember >::iterator begin(){ return _members.begin(); }
    inline std::vector< RestrictionMember >::iterator end(){ return _members.end(); }
};

typedef std::vector< RestrictionLine* >::iterator line_it;
typedef std::vector< RestrictionMember >::iterator member_it;

#endif