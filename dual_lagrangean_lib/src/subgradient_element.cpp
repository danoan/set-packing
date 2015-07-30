#include "subgradient_element.h"

double SubgradientElement::compute_gradient(const int& objective_type, Solution& d){
    ConstraintMember rm;
    double sum_a=0;
    double g;
    for(member_it it_m=_cl->begin();it_m!=_cl->end();it_m++){
        rm = (*it_m);
        sum_a+= rm.cost*d.x(rm.index);
    }    

    g = _cl->rhs() - sum_a;

    if(objective_type==MAX_TYPE){
        g=-g;
    }

    _grad = g;

    return g;  
}

bool SubgradientElement::update_lbda(double& T, int& group){
    double temp = _cl->lbda() + T*_grad;
    bool res;

    if( temp <=0){
        temp=0;
    }

    if(_cl->original()){
        group = 1; //Active
        res = false;
    }else{
        if(_cl->lbda()==0 && temp>0){
            group = 1;
            res = true;
        }else if( _cl->lbda()>0 && temp==0){
            group = 2;
            res = true;
        }else{
            res = false;
        }
    }

    _cl->lbda(temp);
    return res;
}