#include "pool_clique.h"

PoolClique::PoolClique(LagrangeanFormulation& p_f):_f(p_f),_cg(p_f),_replaced_constraints(0),_new_constraints(0){
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

void PoolClique::replace_clique(std::unordered_set<CliqueInequality>::iterator& cl_it, CliqueInequality& temp_clique){
    CliqueInequality new_ci = _cg.replace_constraint(temp_clique, *cl_it);
    _cliques_access.erase(cl_it);
    add_clique( new_ci );
}

void PoolClique::erase_clique_and_constraint(std::unordered_set<CliqueInequality>::iterator& cl_it){
    //I DO NOT remove constraints from the original formulation
    if( !( cl_it->constraint()->original()) ){
        _f.remove_constraint(cl_it->constraint());
        _cliques_access.erase(cl_it);        
    }
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

void PoolClique::assign_groups(std::vector<double>& lbda){
    for(std::unordered_set<CliqueInequality>::iterator cl_it=_cliques_access.begin();cl_it!=_cliques_access.end();cl_it++){
        
    }
}

/*
    This functions will use the current dual solution to find maximal cliques in the ColisionGraph
    of the formulation. It proceeeds as following:

    It first filters the Complete ColisionGraph creating a graph temp_cg induced by the vertices valued 
    as 1 in the solution xk.

    After that, clique inequalities are searched in temp_cg subgraph. Those clique inequalities are already
    violated clique inequalities, because involves more than one vertice valued as 1.

    An expansion routine acts over those clique inequalities in the context of the complete graph. The idea
    is to expand the violated cliques to make it stronger, involving more vertices.

    At the end, the final cliques are compared with the current cliques in the pool. If the final clique
    dominates a existing one, the final clique replaces the currrent one; If the final clique is dominated,
    it is not included; If the final clique doesnt dominate and is not dominated, it means it is a new 
    clique, so it is added.
*/
int PoolClique::extend_pool(Solution& xk){
    ColisionGraph temp_cg = _cg;    //Create a copy og the biggest ColisionGraph

    std::vector<bool> zero_mask(temp_cg.num_vertices());
    std::vector<bool> vertice_marked(temp_cg.num_vertices());

    DynamicBitCluster bit_subgraph;
    bit_subgraph.resize_and_clear( xk.num_components() );

    //Create mask to the subgraph routine.
    for(int i=0;i<xk.num_components();i++){
        if(xk.x(i)==1){
            bit_subgraph.set(i);            
        }
        zero_mask[i] = false;   //Initializes zero_mask
    }
    vertice_marked = zero_mask;
    temp_cg.subgraph(xk,bit_subgraph);


    //Search Maximal Cliques in temp_cg
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

    //For each Maximal Clique above, triy to expand it in the global Colision Graph.
    //These would be the final_cliques
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


    //For each final_clique, checks if it dominates, it is dominated or it is a new one
    ConstraintLine* cl;
    int clique_count=0;
    bool relevant_clique;
    bool replaced = false;
    for(int i=0;i<final_cliques.size();i++){
        CliqueInequality temp_clique = create_clique_inequality( &cl, final_cliques[i] );
        
        relevant_clique = true;

        if( _cliques_access.find(temp_clique) == _cliques_access.end() ){

            //Firstly checks if there is some existing clique which dominates it temp_clique
            for( std::unordered_set<CliqueInequality>::iterator cl_it = _cliques_access.begin();cl_it!=_cliques_access.end();cl_it++){
                if( *cl_it > temp_clique ){
                    relevant_clique = false;
                    break;
                }

            }

            //If none dominates temp_clique, temp_clique is relevant. So it might be included or replace other
            if(relevant_clique){
                replaced = false;
                for( std::unordered_set<CliqueInequality>::iterator cl_it = _cliques_access.begin();cl_it!=_cliques_access.end();){ 
                
                    //It could be the case that temp_clique dominates more than one clique already in the pool
                    //Here, only one is being replaced.
                    if( temp_clique > *cl_it ){

                        std::unordered_set<CliqueInequality>::iterator current = cl_it++;
                        if(!replaced){
                            replace_clique(current, temp_clique);
                            _replaced_constraints++;
                            replaced=true;
                        }else{                            
                            // printf("ERASE %d\n",current->constraint()->index());
                            erase_clique_and_constraint(current);                                                        
                        }
                        continue;
                    }
                    cl_it++;
                }

                if(!replaced){
                    _f.add_new_constraint(cl);

                    add_clique(temp_clique);
                    _new_constraints++;
                    clique_count++;                                
                }
            }else{
                temp_clique.clear();
            }


        }else{
            temp_clique.clear();    //Destroys ConstraintLine Pointer    
        }        
    }

    // assign_groups(lbda);

    return clique_count;
}