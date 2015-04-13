#include "lagrangean_formulation.h"

LagrangeanFormulation::LagrangeanFormulation(Formulation& f):Formulation(f){
    init();
    _no_lagrangean_restrictions = vector<int>();   
}

LagrangeanFormulation::LagrangeanFormulation(Formulation& f,
                                             vector<int>& p_no_lagrangean_restrictions):Formulation(f){
    
    init();
    _no_lagrangean_restrictions = p_no_lagrangean_restrictions;
}

void LagrangeanFormulation::init(){
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
    for(i=0,it_r=_restrictions.begin();it_r!=_restrictions.end();it_r++,i++){
        restriction_line rl = (*it_r);

        for(member_it it_m=rl.members.begin(); it_m!=rl.members.end();it_m++){
            restriction_line_member rlm = (*it_m);
            _lagrangean_costs[rlm.index]+= -(_lbda[i]*rlm.cost);
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
    for(i=0, it_r=_restrictions.begin();it_r!=_restrictions.end();it_r++,i++){
        s+=_lbda[i]*(*it_r).rhs;
    }
    
    return s;
}

bool LagrangeanFormulation::check_restrictions(vector<double> x){
    double sum = 0;

    restriction_line nlr;
    /*
        no_lagrangean_restrictions are the restrictions not passed to the objective function.
        Here it`s been tested the restrictions that are not passed to the objective function.
    */
    for(int k=0;k<_no_lagrangean_restrictions.size();k++){
        nlr = _restrictions[ _no_lagrangean_restrictions[k] ];

        if(check_restriction( nlr,x)==false) return false;
    }

    return true;
}

string LagrangeanFormulation::to_str(){
    ostringstream s;
    if(_objective_type==MIN_TYPE){
        s << "MAX ";
    }else{
        s << "MIN ";
    }

    vector<int> lagrangean_restrictions;    
    for(int i=0;i<_restrictions.size();i++){
        lagrangean_restrictions.push_back(i);
    }
    sort(_no_lagrangean_restrictions.begin(),_no_lagrangean_restrictions.end());    
    for(int k=_no_lagrangean_restrictions.size()-1;k>=0;k--){
        lagrangean_restrictions.erase( lagrangean_restrictions.begin() + _no_lagrangean_restrictions[k] );
    }

    
    for(int j=0;j<_c.size();j++){
        s << "[" << _c[j] << " - (";    

        for(int i=0;i<lagrangean_restrictions.size();i++){
            restriction_line rl = _restrictions[lagrangean_restrictions[i]];
            
            for(member_it it_m=rl.members.begin();it_m!=rl.members.end();it_m++){
                restriction_line_member rlm = (*it_m);
                if(rlm.index==j){
                    if(rlm.cost>=0) s << "+";
                    s << rlm.cost << "l" << i+1; 
                }
            }
        }

        s << ") ]x" << j+1;
        if(j<_c.size()-1){ 
            s << " + ";
        }
    }

    s<<"\t subject to\n";

    for(int k=0;k<_no_lagrangean_restrictions.size();k++){
        restriction_line rl = _restrictions[ _no_lagrangean_restrictions[k] ];

        for(member_it it_m=rl.members.begin();it_m!=rl.members.end();it_m++){
            restriction_line_member rlm = (*it_m);

            if(rlm.cost>=0) s << " +";
            s << rlm.cost << "x" << rlm.index+1;
        }

        if(rl.op==EQUAL) s << " = ";
        if(rl.op==GREATER) s << " > ";
        if(rl.op==GREATER_EQUAL) s << " >= ";
        if(rl.op==LESSER) s << " < ";
        if(rl.op==LESSER_EQUAL) s << " <= ";

        s << rl.rhs;
        s << "\n";
    }
    s << "\n";

    return s.str();
}