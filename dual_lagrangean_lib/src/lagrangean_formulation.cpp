#include "lagrangean_formulation.h"

LagrangeanFormulation::LagrangeanFormulation(Formulation& f):Formulation(f){
    init();
}

LagrangeanFormulation::LagrangeanFormulation(Formulation& f, vector< int >& p_primal_mask,
                                             const vector< ConstraintLine* >& extra_primal, 
                                             const vector< ConstraintLine* >& extra_dual ): Formulation(f),
                                             _primal_mask(p_primal_mask){
    init();
    for(int i=0;i<extra_primal.size();i++){
        _primal_constraints[ extra_primal[i]->index() ] = extra_primal[i];
    }

    for(int i=0;i<extra_dual.size();i++){
        _dual_constraints[ extra_dual[i]->index() ]=extra_dual[i];
        for(member_it mt= extra_dual[i]->begin(); mt!=extra_dual[i]->end();mt++){
            _restr_var_appears[ (*mt).index ].insert( extra_dual[i]->index() );
        }        
    }    
}

LagrangeanFormulation::LagrangeanFormulation(const LagrangeanFormulation& p_lf):Formulation(p_lf){
    _lbda = p_lf._lbda;
    _lagrangean_costs = p_lf._lagrangean_costs;
    _primal_mask = p_lf._primal_mask;

    select_restrictions();
}

LagrangeanFormulation& LagrangeanFormulation::operator=(const LagrangeanFormulation& p_lf){
    if(this!=&p_lf){        
        Formulation::operator=(p_lf);
        _lbda = p_lf._lbda;
        _lagrangean_costs = p_lf._lagrangean_costs;
        _primal_mask = p_lf._primal_mask;        

        init(); //Constructor without parameters doesn't call this method, so I have to call it here
                //And it should be after _primal_mask has been setted, because it's used at select_restrictions
    }

    return *this;
}

void LagrangeanFormulation::select_restrictions(){
    ConstraintLine* cl;

    //At first all the constraints are dualized
    for(line_it it=_constraints.begin();it!=_constraints.end();it++){
        cl = (*it).second;
        _dual_constraints[cl->index()] = cl;
    }

    //After we took of the ones in the primal_mask
    for(int i=0;i<_primal_mask.size();i++){            
        cl = _constraints[ _primal_mask[i] ];    
        _dual_constraints.erase( cl->index() );

        _primal_constraints[ cl->index() ] = cl;
    }    

    //The remaining constraints at _dual_constraints have theis _restr_var_appears updated.
    for(line_it it=_dual_constraints.begin();it!=_dual_constraints.end();it++){
        cl = (*it).second;
        for(member_it mt= cl->begin(); mt!=cl->end();mt++){
            _restr_var_appears[ (*mt).index ].insert( cl->index() );
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
    ConstraintLine* rl;
    ConstraintMember rm;
    for(i=0,it_r=_dual_constraints.begin();it_r!=_dual_constraints.end();it_r++,i++){
        rl = (*it_r).second;

        for(member_it it_m=rl->begin(); it_m!=rl->end();it_m++){
            rm = (*it_m);
            _lagrangean_costs[rm.index]+= -(_lbda[i]*rm.cost);
        }
    }

}

ConstraintLine* LagrangeanFormulation::replace_constraint(vector<ConstraintMember>& vec_cm, ConstraintLine* cl){
    //It's supposed that the new constraint dominates the current one. So, the restr_var_appears vector
    //can be modified wisely.

    int _debug_var_before_count = 0;
    int _debug_var_after_count = 0;

    int var_index;
    unordered_set< int >::iterator restr_index_it;

    for(member_it m_it=cl->begin();m_it!=cl->end();m_it++){
        var_index = m_it->index;        

        restr_index_it = _restr_var_appears[var_index].find( cl->index() );
        if( restr_index_it!=_restr_var_appears[var_index].end() ){
            _restr_var_appears[var_index].erase( restr_index_it );
            _debug_var_before_count+=1;
        }
    }

    for(int i=0;i<vec_cm.size();i++){
        var_index = vec_cm[i].index;
        _restr_var_appears[var_index].insert( cl->index() );
        _debug_var_after_count+=1;
    }

    printf("***%d %d***\n",_debug_var_before_count,_debug_var_after_count);

    return Formulation::replace_constraint(vec_cm,cl);
}

double LagrangeanFormulation::compute(vector<double> p_x){
    check_lbda_setted();

    double s = 0;
    for(int j=0;j<_c.size();j++){
        s+=p_x[j]*_lagrangean_costs[j];
    }

    int i;
    line_it it_r;
    ConstraintLine* rl;
    for(i=0, it_r=_dual_constraints.begin();it_r!=_dual_constraints.end();it_r++,i++){
        rl = (*it_r).second;
        s+=_lbda[i]*rl->rhs();
    }
    
    return s;
}

bool LagrangeanFormulation::check_constraints(vector<double> x){
    double sum = 0;

    ConstraintLine* nlr;
    /*
        no_lagrangean_constraints are the restrictions not passed to the objective function.
        Here it`s been tested the restrictions that are not passed to the objective function.
    */
    for(line_it it=_primal_constraints.begin();it!=_primal_constraints.end();it++){
        nlr = (*it).second;

        if(check_constraint( *nlr,x)==false) return false;
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