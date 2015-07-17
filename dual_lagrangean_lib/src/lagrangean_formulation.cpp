#include "lagrangean_formulation.h"

LagrangeanFormulation::LagrangeanFormulation(Formulation& f):Formulation(f){
    init();
}

LagrangeanFormulation::LagrangeanFormulation(Formulation& f, vector< int >& p_dual_mask,
                                             vector< ConstraintLine* > extra_primal, 
                                             vector< ConstraintLine* > extra_dual ): Formulation(f),
                                             _dual_mask(p_dual_mask){
    init();
    for(int i=0;i<extra_primal.size();i++){
        _primal_constraints.push_back(extra_primal[i]);
    }

    for(int i=0;i<extra_dual.size();i++){
        _dual_constraints.push_back(extra_dual[i]);
        for(member_it mt= extra_dual[i]->begin(); mt!=extra_dual[i]->end();mt++){
            _restr_var_appears[ (*mt).index ].push_back( extra_dual[i]->index() );
        }        
    }    
}

LagrangeanFormulation::LagrangeanFormulation(const LagrangeanFormulation& p_lf):Formulation(p_lf){
    _lbda = p_lf._lbda;
    _lagrangean_costs = p_lf._lagrangean_costs;
    _dual_mask = p_lf._dual_mask;

    select_restrictions();
}

LagrangeanFormulation& LagrangeanFormulation::operator=(const LagrangeanFormulation& p_lf){
    if(this!=&p_lf){        
        Formulation::operator=(p_lf);
        _lbda = p_lf._lbda;
        _lagrangean_costs = p_lf._lagrangean_costs;
        _dual_mask = p_lf._dual_mask;        

        init(); //Constructor without parameters doesn't call this method, so I have to call it here
                //And it should be after _dual_mask has been setted, because it's used at select_restrictions
    }

    return *this;
}

void LagrangeanFormulation::select_restrictions(){
    if(_dual_mask.size()==0){
        for(line_it it=_constraints.begin();it!=_constraints.end();it++){
            _dual_constraints.push_back(*it);
            for(member_it mt= (*it)->begin(); mt!=(*it)->end();mt++){
                _restr_var_appears[ (*mt).index ].push_back( (*it)->index() );
            }
        }
    }else{
        for(int i=0;i<_dual_mask.size();i++){
            if(_dual_mask[i]==1){
                _dual_constraints.push_back(_constraints[i]);
                for(member_it mt= _constraints[i]->begin(); mt!=_constraints[i]->end();mt++){
                    _restr_var_appears[ (*mt).index ].push_back( _constraints[i]->index() );
                }                
            }else{
                _primal_constraints.push_back(_constraints[i]);
            }
        }
    }
}

void LagrangeanFormulation::init(){
    _restr_var_appears.resize( c().size());

    select_restrictions();

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
    ConstraintLine rl;
    ConstraintMember rm;
    for(i=0,it_r=_dual_constraints.begin();it_r!=_dual_constraints.end();it_r++,i++){
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
    ConstraintLine rl;
    for(i=0, it_r=_dual_constraints.begin();it_r!=_dual_constraints.end();it_r++,i++){
        rl = *(*it_r);
        s+=_lbda[i]*rl.rhs();
    }
    
    return s;
}

bool LagrangeanFormulation::check_constraints(vector<double> x){
    double sum = 0;

    ConstraintLine nlr;
    /*
        no_lagrangean_constraints are the restrictions not passed to the objective function.
        Here it`s been tested the restrictions that are not passed to the objective function.
    */
    for(int k=0;k<_primal_constraints.size();k++){
        nlr = *(_primal_constraints[k]);

        if(check_constraint( nlr,x)==false) return false;
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

    // vector<int> lagrangean_constraints;    
    // for(int i=0;i<_constraints.size();i++){
    //     lagrangean_constraints.push_back(i);
    // }
    // sort(_no_lagrangean_constraints.begin(),_no_lagrangean_constraints.end());    
    // for(int k=_no_lagrangean_constraints.size()-1;k>=0;k--){
    //     lagrangean_constraints.erase( lagrangean_constraints.begin() + _no_lagrangean_constraints[k] );
    // }

    
    // for(int j=0;j<_c.size();j++){
    //     s << "[" << _c[j] << " - (";    

    //     for(int i=0;i<lagrangean_constraints.size();i++){
    //         restriction_line rl = _constraints[lagrangean_constraints[i]];
            
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

    // for(int k=0;k<_no_lagrangean_constraints.size();k++){
    //     restriction_line rl = _constraints[ _no_lagrangean_constraints[k] ];

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