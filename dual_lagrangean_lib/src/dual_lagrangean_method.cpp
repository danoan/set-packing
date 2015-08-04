#include "dual_lagrangean_method.h"

void log_start(Formulation& f, LagrangeanFormulation& lf, Solution& p, Solution& d){
    // printf("Begin Solver\n\n");

    // printf("Original Formulation\n");
    // printf("%s\n",f.to_str().c_str());

    // printf("Lagrangean Formulation\n");
    // printf("%s\n",lf.to_str().c_str());    

    // printf("Initial Data\n");
    // // print_vector("LBDA",lbda);
    // print_solution("PRIMAL",p);
    // print_solution("DUAL",d);

    // printf("\n");
}

void DualLagrangeanMethod::log(SubgradientMethod& sm, LagrangeanFormulation& lf){
    ostringstream os;
    ostringstream active_cliques_os;
    ostringstream inactive_cliques_os;

    os << "***************************************************";
    os << "***************************************************" << std::endl;
    os << "\t\t\t ITERACAO: " << sm.num_it() << std::endl;
    os << "---------------------------------------------------";
    os << "---------------------------------------------------" << std::endl;

    os << "PRIMAL: " << _primal.vx() << "\t\t\t" << "BEST PRIMAL: " << _primal.best_value() << std::endl;
    os << "DUAL: " << _dual.vx() << "\t\t\t" << "BEST DUAL: " << _dual.best_value() << std::endl << std::endl;

    os << "PI: " << sm.pi() << "\t\tSTEP SIZE: " << sm.step_size() << "\t\tNO IMPROVEMENT: " << sm.no_improvement() << std::endl << std::endl;


    std::unordered_map<int,ConstraintLine*> active_constraints = lf.active_constraints();
    int original_inactive_cliques = 0;
    int discovered_inactive_cliques = 0;
    int previous_index;
    for(int i=_previous_active_constraints.size()-1;i>=0;i--){
        previous_index = _previous_active_constraints[i];
        if(active_constraints.find(previous_index)==active_constraints.end()){
            ConstraintLine* cl = lf.constraint( previous_index );
            if( cl->original() ){
                original_inactive_cliques+=1;
            }else{
                discovered_inactive_cliques+=1;
                print_clique(inactive_cliques_os,cl,lf);            
            }
            
        }
    }
    _previous_active_constraints.clear();



    int original_active_cliques = 0;
    int discovered_active_cliques = 0;
    for(std::unordered_map<int,ConstraintLine*>::iterator active_it=active_constraints.begin();active_it!=active_constraints.end();active_it++){
        ConstraintLine* cl = (*active_it).second;
        _previous_active_constraints.push_back( (*active_it).first );

        if( cl->original() ){
            original_active_cliques+=1;
        }else{
            discovered_active_cliques+=1;
            print_clique(active_cliques_os,cl,lf);
        }
        
    }


    os << "CLIQUES ATIVAS APÓS ESTA ITERACAO: " << lf.num_active_constraints();
    os << "(ORIGINAL: " << original_active_cliques << "\tDESCOBERTAS: ";
    os << discovered_active_cliques << ")" << std::endl;
    os << "---------------------------------------------------";
    os << "---------------------------------------------------" << std::endl;

    os << active_cliques_os.str();


    os << "CLIQUES DESATIVADAS NESTA ITERACAO: " << original_inactive_cliques + discovered_inactive_cliques;
    os << "(ORIGINAL: " << original_inactive_cliques << "\tDEESCOBERTAS: ";
    os << discovered_inactive_cliques << ")" << std::endl;

    os << inactive_cliques_os.str();

    os << "---------------------------------------------------";
    os << "---------------------------------------------------" << std::endl;
    os << std::endl;

    printf("%s\n",os.str().c_str());
}

void DualLagrangeanMethod::print_clique(ostringstream& os,ConstraintLine* cl, LagrangeanFormulation& lf){
    for(member_it m_it=cl->begin();m_it!=cl->end();m_it++){
        ConstraintMember cm = *m_it;
        if(cm.cost>0){
            os << "+";
        }
        os << "x" << cm.index+1 << " ";                
    }       

    if(cl->op()==EQUAL){
        os << " = ";
    }else if(cl->op()==GREATER){
        os << " > ";
    }else if(cl->op()==GREATER_EQUAL){
        os << " >= ";
    }else if(cl->op()==LESSER){
        os << " < ";
    }else if(cl->op()==LESSER_EQUAL){
        os << " <= ";
    }     
    os << cl->rhs();
    os << "\t\t(lbda: " << cl->lbda() << ")";;

    if(cl->original()){
        os << std::endl;
    }else{
        os << "(*)" << std::endl;
    }

    for(member_it m_it=cl->begin();m_it!=cl->end();m_it++){
        std::unordered_set<int> restr_appears = lf.constr_var_appears((*m_it).index);        
        os << (*m_it).index+1 << ": ";
        for(std::unordered_set<int>::iterator it = restr_appears.begin();it!=restr_appears.end();it++){
            os << (*it)+1 << ", ";
        }                
        os << std::endl;
    }
    os << std::endl;    
}
