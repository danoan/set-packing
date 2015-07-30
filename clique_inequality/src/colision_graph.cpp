#include "colision_graph.h"

ColisionGraph::ColisionGraph(LagrangeanFormulation& p_f):_f(p_f){
    _vertice_links.resize(_f.c().size());
    for(int i=0;i<_vertice_links.size();i++){
        _vertice_links[i].resize_and_clear(_f.c().size());
    }

    ConstraintLine* cl;   
    for(line_it l_it=p_f.begin();l_it!=p_f.end();l_it++){
        cl = (*l_it).second;
        std::vector<int> vars_indexes;        
        for(member_it m_it=cl->begin();m_it!=cl->end();m_it++){
            vars_indexes.push_back( m_it->index );    
        }

        if(vars_indexes.size()>0)
            create_all_edges(vars_indexes);
    }
}

void ColisionGraph::create_all_edges(std::vector<int>& vars_indexes){
    int _edge_index;
    int var_i,var_j;
    for(int i=0;i<vars_indexes.size()-1;i++){
        for(int j=i+1;j<vars_indexes.size();j++){
            var_i = vars_indexes[i];
            var_j = vars_indexes[j];

            if(_vertice_links[var_i].get(var_j)==false){
                _vertice_links[var_i].set(var_j);
                _vertice_links[var_j].set(var_i);

                // printf("ADD EDGE %d->%d\n",var_i,var_j);
            }
        }
    }
}

void ColisionGraph::reduce(const int& p_vertice_index){
    DynamicBitCluster bit_reduce = _vertice_links[p_vertice_index];
    bit_reduce.set(p_vertice_index);

    for(int i=0;i<_f.c().size();i++){
        if(bit_reduce.get(i)==false){
            _vertice_links[i] << 0;
        }else{
            _vertice_links[i] << bit_reduce;    
        }        
    }
}

void ColisionGraph::subgraph(Solution& xk, const DynamicBitCluster& bit_subgraph){
    //Variables not set to one are discarded and the other are reduced
    for(int i=0;i<xk.num_components();i++){
        if(xk.x(i)==1){
            _vertice_links[i] << bit_subgraph;
        }else{
            _vertice_links[i] << 0;
        }
    }
}

void ColisionGraph::remove(const std::vector<int>& vertices_in_clique){
    DynamicBitCluster bit_remove;
    bit_remove.resize_and_clear(num_vertices(),(1<<(BITCLUSTER_SIZE) )-1);

    for(int i=0;i<vertices_in_clique.size();i++){
        bit_remove.reset( vertices_in_clique[i] );
        _vertice_links[ vertices_in_clique[i] ] << 0;
    }

    for(int i=0;i<num_vertices();i++){
        _vertice_links[i] << bit_remove;
    }

}

int ColisionGraph::max_degree(int& max_degree, std::vector<bool>& vertice_marked){
    max_degree=0;
    int cur_degree=0;
    int vertice_max_degree=-1;
    for(int i=0;i<_f.c().size();i++){    
        cur_degree=_vertice_links[i].count();
        if(cur_degree>max_degree && vertice_marked[i]==false){
            max_degree=cur_degree;
            vertice_max_degree = i;
        }
    }
    return vertice_max_degree;
}

CliqueInequality ColisionGraph::check_new_clique(const CliqueInequality& ci, std::vector<bool>& vertice_marked){
    std::vector<ConstraintMember> vec_cm;
    for(int i=0;i<vertice_marked.size();i++){
        if(vertice_marked[i]){
            ConstraintMember cm;
            cm.index = i;
            cm.cost = 1;
            vec_cm.push_back(cm);
        }
    }

    ConstraintLine* cl = new ConstraintLine(vec_cm,1,LESSER_EQUAL);
    return CliqueInequality(cl,ci.total_variables());
}

CliqueInequality ColisionGraph::create_clique_and_constraint(const CliqueInequality& ci, std::vector<bool>& vertice_marked){
    std::vector<ConstraintMember> vec_cm;
    for(int i=0;i<vertice_marked.size();i++){
        if(vertice_marked[i]){
            ConstraintMember cm;
            cm.index = i;
            cm.cost = 1;
            vec_cm.push_back(cm);
        }
    }
    
    return CliqueInequality(_f.replace_constraint(vec_cm,ci.constraint()),_f.c().size());
}

CliqueInequality ColisionGraph::replace_constraint(const CliqueInequality& new_ci, const CliqueInequality& previous_ci){
    std::vector<ConstraintMember> vec_cm;
    ConstraintLine* cl = new_ci.constraint();

    for(member_it m_it=cl->begin();m_it!=cl->end();m_it++){
        vec_cm.push_back(*m_it);
    }
    
    return CliqueInequality( _f.replace_constraint(vec_cm,previous_ci.constraint()),_f.c().size() );
}