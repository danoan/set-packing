#include <cstdio>
#include <cstdlib>
#include <vector>
#include <unordered_set>
#include <functional>


#include "clique_inequality.h"
#include "pool_clique.h"
#include "io.h"
#include "formulation.h"

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

    return Formulation(A_index,A_cost,b,c,op,objective_type);    ;
}

void teste_pool(){
    Formulation f = read_formulation("test_input_1");   

    std::vector< CliqueInequality > cliques;
    for(line_it l_it=f.begin();l_it!=f.end();l_it++){
        cliques.push_back( CliqueInequality( (*l_it).second, 7) );
    }

    PoolClique p(f);

    for(line_it it=f.begin();it!=f.end();it++){
        p.add_clique( (*it).second, (*it).second->size() );
    }

    solution_pair s;
    s.x.push_back(1);s.x.push_back(1);s.x.push_back(1);s.x.push_back(0);s.x.push_back(0);s.x.push_back(0);s.x.push_back(0);
    
    p.extend_pool(s);
}

void teste_colision_graph(){
    Formulation f = read_formulation("test_input_1");   
    solution_pair s;
    s.x.push_back(1);s.x.push_back(1);s.x.push_back(1);s.x.push_back(0);s.x.push_back(0);s.x.push_back(0);s.x.push_back(0);    

    ColisionGraph cg(f);   
    for(int i=0;i<7;i++){
        printf("%d\n",cg._vertice_edges[i]._curr_bit);    //TUDO 18 arestas
    }

    line_it restr_4;
    ConstraintLine* cl;
    bool found_it ;
    for(line_it it=f.begin();it!=f.end();it++){
        cl = (*it).second;
        if(f.c().size()==2){
            found_it = true;
            for(member_it m_it=cl->begin();m_it!=cl->end();m_it++){
                if( m_it->index!= 2 and m_it->index!= 4){
                    found_it=false;
                    break;
                }
            }
            if(found_it) break;
        }
    }

    CliqueInequality ci( cl, f.c().size() );

    std::vector<bool> vertice_marked(cg.num_vertices());

    for(member_it m_it=ci.begin();m_it!=ci.end();m_it++){
        cg.reduce(m_it->index);
        vertice_marked[m_it->index] = true;
    }

    int max_degree;
    int vertice_index_sel = cg.max_degree(max_degree, vertice_marked);
    int extension_count=0;
    printf("%d\n",vertice_index_sel);
    while(vertice_index_sel>=0){
        vertice_marked[vertice_index_sel]=true;

        cg.reduce(vertice_index_sel);
        vertice_index_sel = cg.max_degree(max_degree, vertice_marked);

        extension_count++;
    }    

    printf("%d\n",extension_count);
    
}

int main(){
    teste_colision_graph();
    return 0;
}