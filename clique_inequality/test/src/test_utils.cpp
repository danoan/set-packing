#include "test_utils.h"

Formulation read_formulation(std::string filename){
    std::vector< std::vector<int> > A_index;
    std::vector< std::vector<double> > A_cost;
    std::vector<double> b;
    std::vector<double> c;
    std::vector<int> op;
    int objective_type;

    FILE* file = fopen(filename.c_str(),"rb");
    read_input(A_index,A_cost,b,c,op,objective_type,file);
    fclose(file);

    return Formulation(A_index,A_cost,b,c,op,objective_type);
}

Formulation read_big_formulation(std::string filename){
    std::vector< std::vector<double> > A;
    std::vector<double> b;
    std::vector<double> c;
    std::vector<int> op;
    int objective_type;

    FILE* file = fopen(filename.c_str(),"rb");
    read_input(A,b,c,op,objective_type,file);
    fclose(file);

    return Formulation(A,b,c,op,objective_type);
}

bool compare_constraint(ConstraintLine* cl, int size_members, const int* vars_indexes){
    if(cl->size()!=size_members) return false;

    bool ok;
    for(int i=0;i<size_members;i++){
        ok=false;
        for(member_it m_it=cl->begin();m_it!=cl->end();m_it++){
            if(m_it->index==vars_indexes[i]){
                ok=true;
            }
        }

        if(!ok) return false;
    }

    return true;
}