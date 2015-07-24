#include "pool_clique.h"

PoolClique::PoolClique(Formulation& p_f):_f(p_f),_cg(p_f),_replaced_constraints(0),_new_constraints(0){
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
        _cliques_access.insert( p_ci );          
        return true;
    }    
    return false;
}

CliqueInequality PoolClique::create_clique_inequality(ConstraintLine** cl, std::vector<int>& vertices_in_clique){
    std::vector<ConstraintMember> vec_cm;
    for(int i=0;i<vertices_in_clique.size();i++){
        ConstraintMember cm;
        cm.index = vertices_in_clique[i];
        cm.cost = 1;
        vec_cm.push_back(cm);
    }

    (*cl) = new ConstraintLine(vec_cm,1,LESSER_EQUAL);
    return CliqueInequality( (*cl),_f.c().size());
}

std::vector<int> PoolClique::find_clique(int seed, ColisionGraph temp_cg, std::vector<bool> vertice_marked){
    std::vector<int> vertices_in_clique;
    int max_degree;
    while(seed>=0){
        vertices_in_clique.push_back(seed);
        vertice_marked[seed]=true;

        temp_cg.reduce(seed);
        seed = temp_cg.max_degree(max_degree, vertice_marked);
    }  
    return vertices_in_clique;
}

int PoolClique::extend_pool(solution_pair& xk){
    ColisionGraph temp_cg = _cg;    //Create a copy og the biggest ColisionGraph

    std::vector<bool> zero_mask(temp_cg.num_vertices());
    std::vector<bool> vertice_marked(temp_cg.num_vertices());

    DynamicBitCluster bit_subgraph;
    bit_subgraph.resize_and_clear( xk.x.size() );

    //Puts in the mask only the variables set to one
    for(int i=0;i<xk.x.size();i++){
        if(xk.x[i]==1){
            bit_subgraph.set(i);            
        }
        zero_mask[i] = false;
    }
    vertice_marked = zero_mask;

    temp_cg.subgraph(xk,bit_subgraph);

    int max_degree;
    int seed;
    std::vector< std::vector<int> > cliques_found;

    while(1){
        seed = temp_cg.max_degree(max_degree, vertice_marked);
        if(seed==-1){
            break;
        }

        std::vector<int> vertices_in_clique = find_clique(seed,temp_cg,vertice_marked);
        if(vertices_in_clique.size()>1){
            cliques_found.push_back( vertices_in_clique );
        }

        temp_cg.remove( vertices_in_clique );
    }

    std::vector< std::vector<int> > final_cliques;

    for(int i=0;i<cliques_found.size();i++){
        ColisionGraph temp2_cg = _cg;
        vertice_marked = zero_mask;

        std::vector<int>& vertices = cliques_found[i];
        for(int j=0;j<vertices.size();j++){
            temp2_cg.reduce( vertices[j] );
            vertice_marked[ vertices[j] ] = true;
        }

        seed = temp2_cg.max_degree(max_degree, vertice_marked);
        if(seed<0){
            final_cliques.push_back( vertices );
        }else{
            final_cliques.push_back( find_clique(seed,temp2_cg,vertice_marked) );    
        }
        
    }

    ConstraintLine* cl;
    int clique_count=0;
    bool relevant_clique;
    for(int i=0;i<final_cliques.size();i++){
        CliqueInequality temp_clique = create_clique_inequality( &cl, final_cliques[i] );
        
        relevant_clique = true;

        if( _cliques_access.find(temp_clique) == _cliques_access.end() ){

            for( std::unordered_set<CliqueInequality>::iterator cl_it = _cliques_access.begin();cl_it!=_cliques_access.end();cl_it++){
                if( *cl_it > temp_clique ){
                    relevant_clique = false;
                    break;
                }

                if( temp_clique > *cl_it ){
                    relevant_clique = false;

                    CliqueInequality new_ci = _cg.replace_constraint(temp_clique, *cl_it);
                    _cliques_access.erase(cl_it);
                    add_clique( new_ci );
                    _replaced_constraints++;

                    break;
                }
            }

            if(relevant_clique){
                _f.add_new_constraint(cl);

                add_clique(temp_clique);
                _new_constraints++;
                clique_count++;                
            }else{
                temp_clique.clear();
            }


        }else{
            temp_clique.clear();    //Destroys ConstraintLine Pointer    
        }
        
    }

    return clique_count;
}