#include "test_colision_graph.h"

#include "colision_graph.h"

void test_colision_graph_creation(){
    Formulation f = read_formulation("../test_input_3");
    ColisionGraph cg(f);

    //Testes if the degree count of each vertex is corrected
    int count_expected_result[] = {3,4,0,4,0,4,3,0,0,0};    
    for(int i=0;i<7;i++){
        if(cg._vertice_links[i].count()!=count_expected_result[i]){
            FAILED("test_colision_graph_creation");
        }
    }

    int links_expected[7][4] = { {1,5,3,-1}, {0,3,5,6}, {-1,-1,-1,-1}, {0,1,6,5}, {-1,-1,-1,-1}, 
                                  {3,0,1,6}, {1,3,5,-1} };    
    for(int i=0;i<cg._vertice_links.size();i++){

        for(int j=0;j<4;j++){
            if(links_expected[i][j]==-1) break;
            if( cg._vertice_links[i].get( links_expected[i][j] )==false ){
                printf("VERTICE %d EDGE %d-%d\n",i,i,links_expected[i][j]);
                FAILED("test_colision_graph_creation");                
            }
        }
    }

    PASSED("test_colision_graph_creation");
}

void test_colision_graph_reduce(){
    Formulation f = read_formulation("../test_input_3");
    ColisionGraph cg(f);

    cg.reduce(1);
    for(int i=0;i<7;i++){
        if( cg._vertice_links[4].get(i)==true ){
            printf("NODE INDEX 4 MUST NOT HAVE ANY EDGE LINK\n");
            FAILED("test_colision_graph_reduce");
        }
    }

    PASSED("test_colision_graph_reduce");
}