#include "restriction_line.h"

void RestrictionLine::copy_line(const RestrictionLine& p_rl){
    

    for(int i=0;i<p_rl._members.size();i++){
        RestrictionMember orm = p_rl._members[i];
        RestrictionMember nrm;

        nrm.index = orm.index;
        nrm.cost = orm.cost;

        add(nrm);
    }    

    _rhs = p_rl._rhs;
    _op = p_rl._op;    
}

RestrictionLine::RestrictionLine(const RestrictionLine& p_rl){
    copy_line(p_rl);
}

RestrictionLine& RestrictionLine::operator=(const RestrictionLine& p_rl){
    if(this != &p_rl){  //Avoi self-assignment
        _members.clear();
        copy_line(p_rl);
    }

    return *this;
}
