#include "constraint_line.h"

int ConstraintLine::_next_id = 0;

void ConstraintLine::copy_line(const ConstraintLine& p_rl){
    

    for(int i=0;i<p_rl._members.size();i++){
        ConstraintMember orm = p_rl._members[i];
        ConstraintMember nrm;

        nrm.index = orm.index;
        nrm.cost = orm.cost;

        add(nrm);
    }    

    _rhs = p_rl._rhs;
    _op = p_rl._op;    

    _id = p_rl._id;
}

ConstraintLine::ConstraintLine(const ConstraintLine& p_rl){
    copy_line(p_rl);
}

ConstraintLine& ConstraintLine::operator=(const ConstraintLine& p_rl){
    if(this != &p_rl){  //Avoi self-assignment
        _members.clear();
        copy_line(p_rl);
    }

    return *this;
}
