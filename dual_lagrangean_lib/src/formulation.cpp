#include "formulation.h"

Formulation::Formulation(vector< vector<double> >& p_A, vector<double>& p_b, vector<double>& p_c, 
            vector<int>& p_op, int p_objective_type):_c(p_c), _objective_type(p_objective_type),
            _initialized_flag(true){
    // printf("NORMAL CONSTRUCTOR\n");

    if( !( (p_A.size()==p_b.size()) &&
           (p_A.size()==p_op.size()) )   ) {
        //ERROR
    }
    
    ConstraintLine* rl;
    for(int i=0;i<p_A.size();i++){
        rl = new ConstraintLine();

        for(int j=0;j<p_A[i].size();j++){
            ConstraintMember rm;

            if(p_A[i][j]==0){
                continue;
            }

            rm.index = j;
            rm.cost = p_A[i][j];
            rl->add(rm);
        }
        rl->rhs( p_b[i] );
        rl->op( p_op[i] );
        _constraints[rl->index()] = rl;
    }
}

Formulation::Formulation(vector< vector<int> >& p_A_index, vector< vector<double> >& p_A_cost, vector<double>& p_b, 
            vector<double>& p_c, vector<int>& p_op, int p_objective_type ):_c(p_c), 
            _objective_type(p_objective_type),_initialized_flag(true){

    // printf("NORMAL CONSTRUCTOR\n");

    if( !( (p_A_index.size()==p_A_cost.size()) && 
           (p_A_index.size()==p_b.size()) &&
           (p_A_index.size()==p_op.size()) )   ) {
        //ERROR
    }
    
    ConstraintLine* rl;
    for(int i=0;i<p_A_index.size();i++){
        rl = new ConstraintLine();
        for(int j=0;j<p_A_index[i].size();j++){
            ConstraintMember rm;

            rm.index = p_A_index[i][j];
            rm.cost = p_A_cost[i][j];
            rl->add(rm);            
        }
        rl->rhs( p_b[i] );
        rl->op( p_op[i] );
        _constraints[rl->index()] = rl;
    }
}

Formulation::Formulation(const Formulation& f):_initialized_flag(true){
    // printf("COPY CONSTRUCTOR\n");
    copy_formulation(f);
}

Formulation& Formulation::operator=(const Formulation& p_f){
    // printf("ASSIGNMENT OPERATOR\n");
    if(this!=&p_f){        
        // printf("COPY\n");
        _initialized_flag=true;
        copy_formulation(p_f);
    }
    return *this;
}

Formulation::~Formulation(){
    // printf("DELETE %p %s\n",this,_initialized_flag?"EXECUTE":"SKIP");
    if(_initialized_flag){
        for(line_it it=_constraints.begin();it!=_constraints.end();it++){
            delete (*it).second;
        }
    }
}

void Formulation::copy_formulation(const Formulation& p_f){
    _c.resize(p_f._c.size());

    ConstraintLine *cl,*temp;
    int i=0;
    for(auto it=p_f._constraints.begin();it!=p_f._constraints.end();it++){
        cl = (*it).second;
        temp = new ConstraintLine( cl );
        _constraints[temp->index()] = temp;    
    }
    
    _c = p_f._c;
    _objective_type = p_f._objective_type;      
}

ConstraintLine* Formulation::replace_constraint(vector<ConstraintMember>& vec_cm, ConstraintLine* cl){
    cl->replace(vec_cm);
    return cl;
}

double Formulation::compute(vector<double> x){
    double s = 0;
    for(int j=0;j<_c.size();j++){
        s+=x[j]*_c[j];
    }

    return s;
}

bool Formulation::check_constraint(ConstraintLine& rl,vector<double>& x){
    double sum = 0;
    
    ConstraintMember rm;
    for(member_it it_m=rl.begin();it_m!=rl.end();it_m++){
        rm = (*it_m);
        sum+= x[ rm.index ]*rm.cost;
    }

    switch(rl.op()){
        case EQUAL:
            if( !(sum==rl.rhs()) ) return false;
            break;
        case GREATER:
            if( !(sum>rl.rhs()) ) return false;
            break;            
        case GREATER_EQUAL:
            if( !(sum>=rl.rhs()) ) return false;
            break;    
        case LESSER:
            if( !(sum<rl.rhs()) ) return false;
            break;    
        case LESSER_EQUAL:
            if( !(sum<=rl.rhs()) ) return false;
            break;                                    
    }

    return true;
}

bool Formulation::check_constraints(vector<double> x){    
    ConstraintLine* rl;
    for(line_it it_r=begin();it_r!=end();it_r++){
        rl = (*it_r).second;
        if(check_constraint( *rl,x )==false) return false;
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

    ConstraintLine* rl;
    ConstraintMember rm;
    for(line_it it_r=begin();it_r!=end();it_r++){
        rl = (*it_r).second;
        for(member_it it_m=rl->begin();it_m!=rl->end();it_m++){
            rm = (*it_m);

            if(rm.cost>=0) s << " +";
            s << rm.cost << "x" << rm.index+1;
        }

        if(rl->op()==EQUAL) s << " = ";
        if(rl->op()==GREATER) s << " > ";
        if(rl->op()==GREATER_EQUAL) s << " >= ";
        if(rl->op()==LESSER) s << " < ";
        if(rl->op()==LESSER_EQUAL) s << " <= ";

        s << rl->rhs();
        s << "\n";
    }
    s << "\n";

    return s.str();
}
