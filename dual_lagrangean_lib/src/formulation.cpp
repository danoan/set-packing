#include "formulation.h"

Formulation::Formulation(vector< vector<double> >& p_A, vector<double>& p_b, vector<double>& p_c, 
            vector<int>& p_op, int p_objective_type):_c(p_c), _objective_type(p_objective_type){
    if( !( (p_A.size()==p_b.size()) &&
           (p_A.size()==p_op.size()) )   ) {
        //ERROR
    }

    restriction_line_member rlm;
    for(int i=0;i<p_A.size();i++){
        restriction_line rl;
        for(int j=0;j<p_A[i].size();j++){
            if(p_A[i][j]==0){
                continue;
            }

            rlm.index = j;
            rlm.cost = p_A[i][j];
            rl.members.push_back(rlm);            
        }
        rl.rhs = p_b[i];
        rl.op = p_op[i];
        _restrictions.push_back(rl);
    }
}

Formulation::Formulation(vector< vector<int> >& p_A_index, vector< vector<double> >& p_A_cost, vector<double>& p_b, 
            vector<double>& p_c, vector<int>& p_op, int p_objective_type ):_c(p_c), 
            _objective_type(p_objective_type){

    if( !( (p_A_index.size()==p_A_cost.size()) && 
           (p_A_index.size()==p_b.size()) &&
           (p_A_index.size()==p_op.size()) )   ) {
        //ERROR
    }

    restriction_line_member rlm;
    for(int i=0;i<p_A_index.size();i++){
        restriction_line rl;
        for(int j=0;j<p_A_index[i].size();j++){
            rlm.index = p_A_index[i][j];
            rlm.cost = p_A_cost[i][j];
            rl.members.push_back(rlm);            
        }
        rl.rhs = p_b[i];
        rl.op = p_op[i];
        _restrictions.push_back(rl);
    }
}

Formulation::Formulation(Formulation& f){
    _restrictions.resize(f._restrictions.size());
    _c.resize(f._c.size());

    _restrictions = f._restrictions;
    _c = f._c;
    _objective_type = f._objective_type;
}

double Formulation::compute(vector<double> x){
    double s = 0;
    for(int j=0;j<_c.size();j++){
        s+=x[j]*_c[j];
    }

    return s;
}

bool Formulation::check_restriction(restriction_line& rl,vector<double>& x){
    double sum = 0;
    
    for(member_it it_m=rl.members.begin();it_m!=rl.members.end();it_m++){
        restriction_line_member rlm = (*it_m);
        sum+= x[ rlm.index ]*rlm.cost;
    }

    switch(rl.op){
        case EQUAL:
            if( !(sum==rl.rhs) ) return false;
            break;
        case GREATER:
            if( !(sum>rl.rhs) ) return false;
            break;            
        case GREATER_EQUAL:
            if( !(sum>=rl.rhs) ) return false;
            break;    
        case LESSER:
            if( !(sum<rl.rhs) ) return false;
            break;    
        case LESSER_EQUAL:
            if( !(sum<=rl.rhs) ) return false;
            break;                                    
    }

    return true;
}

bool Formulation::check_restrictions(vector<double> x){    
    restriction_line rl;
    for(line_it it_r=_restrictions.begin();it_r!=_restrictions.end();it_r++){
        rl = (*it_r);
        if(check_restriction( rl,x )==false) return false;
    }

    return true;
}

string Formulation::to_str(){
    ostringstream s;
    if(_objective_type==MIN_TYPE){
        s << "MIN ";
    }else{
        s << "MAX ";
    }

    for(int j=0;j<_c.size();j++){
        if(_c[j]>=0) s << " +";
        s << _c[j] << "x" << j+1;
    }
    s<<"\t subject to\n";

    for(line_it it_r=_restrictions.begin();it_r!=_restrictions.end();it_r++){
        restriction_line rl = (*it_r);
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
