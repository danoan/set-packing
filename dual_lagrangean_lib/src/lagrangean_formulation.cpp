#include "lagrangean_formulation.h"

LagrangeanFormulation::LagrangeanFormulation(Formulation& f,
                                             bool ALL_CONST_ARE_DUAL,
                                             int* dual_mask):Formulation(f){
    init(ALL_CONST_ARE_DUAL,dual_mask);
}

LagrangeanFormulation::LagrangeanFormulation(Formulation& f, bool ALL_CONST_ARE_DUAL, 
                                             int* dual_mask, vector< RestrictionLine* > extra_primal, 
                                             vector< RestrictionLine* > extra_dual ): Formulation(f){
    init(ALL_CONST_ARE_DUAL,dual_mask);
    for(int i=0;i<extra_primal.size();i++){
        _primal_restrictions.push_back(extra_primal[i]);
    }

    for(int i=0;i<extra_dual.size();i++){
        _dual_restrictions.push_back(extra_dual[i]);
    }    
}

void LagrangeanFormulation::init(bool ALL_CONST_ARE_DUAL, int* dual_mask){
    if(ALL_CONST_ARE_DUAL){
        for(line_it it=_restrictions.begin();it!=_restrictions.end();it++){
            _dual_restrictions.push_back(*it);
        }
    }else{
        for(int i=0;i<_restrictions.size();i++){
            if(dual_mask[i]==1){
                _dual_restrictions.push_back(_restrictions[i]);
            }else{
                _primal_restrictions.push_back(_restrictions[i]);
            }
        }
    }

    _lagrangean_costs = vector<double>();
    _lagrangean_costs.resize(_c.size());         
}

void LagrangeanFormulation::check_lbda_setted(){
    if(_lbda.size()==0){
        printf("Error: No lambda was setted");
        exit(-1);
    }
}

void LagrangeanFormulation::update_lagrangean_costs(){
    for(int j=0;j<_c.size();j++){
        _lagrangean_costs[j] = _c[j];
    }

    int i;
    line_it it_r;
    RestrictionLine rl;
    RestrictionMember rm;
    for(i=0,it_r=_dual_restrictions.begin();it_r!=_dual_restrictions.end();it_r++,i++){
        rl = *(*it_r);

        for(member_it it_m=rl.begin(); it_m!=rl.end();it_m++){
            rm = (*it_m);
            _lagrangean_costs[rm.index]+= -(_lbda[i]*rm.cost);
        }
    }

}

double LagrangeanFormulation::compute(vector<double> p_x){
    check_lbda_setted();

    double s = 0;
    for(int j=0;j<_c.size();j++){
        s+=p_x[j]*_lagrangean_costs[j];
    }

    int i;
    line_it it_r;
    RestrictionLine rl;
    for(i=0, it_r=_dual_restrictions.begin();it_r!=_dual_restrictions.end();it_r++,i++){
        rl = *(*it_r);
        s+=_lbda[i]*rl.rhs();
    }
    
    return s;
}

bool LagrangeanFormulation::check_restrictions(vector<double> x){
    double sum = 0;

    RestrictionLine nlr;
    /*
        no_lagrangean_restrictions are the restrictions not passed to the objective function.
        Here it`s been tested the restrictions that are not passed to the objective function.
    */
    for(int k=0;k<_primal_restrictions.size();k++){
        nlr = *(_primal_restrictions[k]);

        if(check_restriction( nlr,x)==false) return false;
    }

    return true;
}

string LagrangeanFormulation::to_str(){
    ostringstream s;
    // if(_objective_type==MIN_TYPE){
    //     s << "MAX ";
    // }else{
    //     s << "MIN ";
    // }

    // vector<int> lagrangean_restrictions;    
    // for(int i=0;i<_restrictions.size();i++){
    //     lagrangean_restrictions.push_back(i);
    // }
    // sort(_no_lagrangean_restrictions.begin(),_no_lagrangean_restrictions.end());    
    // for(int k=_no_lagrangean_restrictions.size()-1;k>=0;k--){
    //     lagrangean_restrictions.erase( lagrangean_restrictions.begin() + _no_lagrangean_restrictions[k] );
    // }

    
    // for(int j=0;j<_c.size();j++){
    //     s << "[" << _c[j] << " - (";    

    //     for(int i=0;i<lagrangean_restrictions.size();i++){
    //         restriction_line rl = _restrictions[lagrangean_restrictions[i]];
            
    //         for(member_it it_m=rl.members.begin();it_m!=rl.members.end();it_m++){
    //             restriction_line_member rlm = (*it_m);
    //             if(rlm.index==j){
    //                 if(rlm.cost>=0) s << "+";
    //                 s << rlm.cost << "l" << i+1; 
    //             }
    //         }
    //     }

    //     s << ") ]x" << j+1;
    //     if(j<_c.size()-1){ 
    //         s << " + ";
    //     }
    // }

    // s<<"\t subject to\n";

    // for(int k=0;k<_no_lagrangean_restrictions.size();k++){
    //     restriction_line rl = _restrictions[ _no_lagrangean_restrictions[k] ];

    //     for(member_it it_m=rl.members.begin();it_m!=rl.members.end();it_m++){
    //         restriction_line_member rlm = (*it_m);

    //         if(rlm.cost>=0) s << " +";
    //         s << rlm.cost << "x" << rlm.index+1;
    //     }

    //     if(rl.op==EQUAL) s << " = ";
    //     if(rl.op==GREATER) s << " > ";
    //     if(rl.op==GREATER_EQUAL) s << " >= ";
    //     if(rl.op==LESSER) s << " < ";
    //     if(rl.op==LESSER_EQUAL) s << " <= ";

    //     s << rl.rhs;
    //     s << "\n";
    // }
    // s << "\n";

    return s.str();
}