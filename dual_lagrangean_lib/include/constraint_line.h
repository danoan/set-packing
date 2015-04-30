#ifndef __RESTRICTION_LINE__
#define __RESTRICTION_LINE__

#include <vector>

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
    

    void copy_line(const ConstraintLine& p_rl);

public:
    ConstraintLine(){ _id = _next_id++;};
    ConstraintLine(const ConstraintLine& p_rl);
    ConstraintLine& operator=(const ConstraintLine& p_rl);

    inline double rhs(){return _rhs;}
    inline void rhs(double p_rhs){ _rhs=p_rhs;}

    inline int op(){return _op;}
    inline void op(int p_op){_op=p_op;}

    inline int index(){return _id;}

    inline void add(ConstraintMember& p_rlm){ _members.push_back(p_rlm); }

    inline std::vector< ConstraintMember >::iterator begin(){ return _members.begin(); }
    inline std::vector< ConstraintMember >::iterator end(){ return _members.end(); }
};

typedef std::vector< ConstraintLine* >::iterator line_it;
typedef std::vector< ConstraintMember >::iterator member_it;

#endif