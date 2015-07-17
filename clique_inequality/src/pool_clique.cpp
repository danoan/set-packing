#include "pool_clique.h"

PoolClique::PoolClique(Formulation& p_f):_f(p_f),_cg(p_f){
    for(line_it it=p_f.begin();it!=p_f.end();it++){
        add_clique( (*it).second, p_f.c().size() );
    }
}

bool PoolClique::add_clique(ConstraintLine* p_cl, int p_num_variables){
    CliqueInequality ci = CliqueInequality(p_cl,p_num_variables);
    return add_clique(ci);
}

bool PoolClique::add_clique(CliqueInequality& p_ci){
    if( _cliques_access.find(p_ci) == _cliques_access.end() ){
        _cliques_queue.push( p_ci );            
        _cliques_access.insert( p_ci );          
        return true;
    }    
    return false;
}

void PoolClique::pop_clique(){
    _cliques_access.erase(_cliques_queue.top());
    _cliques_queue.pop();
}

bool PoolClique::extend_pool(){
    _pool_updated=false;
    if(_cliques_queue.size()==0){
        return false;
    }

    ColisionGraph temp_cg = _cg;    //Create a copy og the biggest ColisionGraph

    std::vector<bool> vertice_marked(temp_cg.num_vertices());

    //Gets the clique with the smaller number of elements    
    CliqueInequality ci = _cliques_queue.top();


    if(ci.is_maximal()){
        //All cliques are alread maximal.
        return false;
    }



    for(member_it m_it=ci.begin();m_it!=ci.end();m_it++){
        temp_cg.reduce(m_it->index);
        vertice_marked[m_it->index] = true;
    }

    int max_degree;
    int vertice_index_sel = temp_cg.max_degree(max_degree, vertice_marked);
    int extension_count=0;

    while(vertice_index_sel>=0){
        vertice_marked[vertice_index_sel]=true;

        temp_cg.reduce(vertice_index_sel);
        vertice_index_sel = temp_cg.max_degree(max_degree, vertice_marked);

        extension_count++;
    }
   
    if(extension_count==0){
        //I'm not able to extend this clique, so it is maximal
        ci.set_as_maximal();
        pop_clique();
        // add_clique(ci);
    }else{
        pop_clique();        

        CliqueInequality temp_clique = temp_cg.check_new_clique(ci, vertice_marked);
        if( _cliques_access.find(temp_clique) == _cliques_access.end() ){
            ci = temp_cg.create_clique_and_constraint( ci, vertice_marked);    

            add_clique(ci);
            _pool_updated = true;
        }

        temp_clique.clear();
    }

    return true;
}