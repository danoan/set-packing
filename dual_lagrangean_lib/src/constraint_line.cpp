#include "constraint_line.h"

int ConstraintLine::_next_id = 0;

void ConstraintLine::copy_line(ConstraintLine* p_rl){
    

    for(int i=0;i<p_rl->_members.size();i++){
        ConstraintMember orm = p_rl->_members[i];
        ConstraintMember nrm;

        nrm.index = orm.index;
        nrm.cost = orm.cost;

        add(nrm);
    }    

    _rhs = p_rl->_rhs;
    _op = p_rl->_op;    

    _id = p_rl->_id;
}

ConstraintLine::ConstraintLine(std::vector< ConstraintMember > &p_members, double p_rhs, int p_op){
    _id = _next_id++;
    _members = p_members;
    _rhs = p_rhs;
    _op = p_op;
}

ConstraintLine::ConstraintLine(ConstraintLine* p_rl){
    copy_line(p_rl);
}
