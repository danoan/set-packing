#include "clique_inequality.h"

CliqueInequality::CliqueInequality():_total_active_variables(0),_index_sum_active_variables(0),
                                  _maximal_clique(false),_total_variables(0){

}

CliqueInequality::CliqueInequality(ConstraintLine* p_cl, int p_total_variables):_cl(p_cl),
                                  _total_active_variables(0),_index_sum_active_variables(0),
                                  _maximal_clique(false),_total_variables(p_total_variables){
    int j=0;

    int total_bit_cluster = (int) ceil(1.0*p_total_variables/BITCLUSTER_SIZE);
    for(int i=0;i<total_bit_cluster;i++){
        _variables_in_use.push_back(bit_cluster(0));
    }

    int vars_mask_index;
    int vars_mask_pos;
    for(member_it m_it=p_cl->begin();m_it!=p_cl->end();m_it++){
        vars_mask_index = (int) floor(m_it->index/BITCLUSTER_SIZE);
        vars_mask_pos = m_it->index%BITCLUSTER_SIZE;
        
        _total_active_variables+=1;
        _index_sum_active_variables+=m_it->index;

        _variables_in_use[vars_mask_index][vars_mask_pos] = 1;
    }
}

bool CliqueInequality::operator==(const CliqueInequality& p_ci) const{
    for(int i=0;i<_variables_in_use.size();i++){
        if( ( _variables_in_use[i] xor p_ci._variables_in_use[i] )!=0 ){    //There are some bits that are different
            return false;
        }
    }
    return true;
}


bool CliqueInequality::operator>(const CliqueInequality& p_ci) const{
    //This must have all the bits of p_ci and more
    for(int i=0;i<_variables_in_use.size();i++){
        if( ( _variables_in_use[i] & p_ci._variables_in_use[i] )!=p_ci._variables_in_use[i] ){    //There are some bits that are different
            return false;
        }
    }
    return true;   
}

bool CliqueInequality::operator<(const CliqueInequality& p_ci) const{
    return p_ci>(*this);
}
