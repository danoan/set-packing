#include "test_pool_clique.h"

void test_extend_pool(){
    Formulation f = read_formulation("../test_input_1");
    printf("%s\n",f.to_str().c_str());
    PoolClique p(f);

    solution_pair s;
    for(int i=0;i<f.c().size();i++){
        s.x.push_back(0);
    }

    s.x[2] = 1;
    s.x[4] = 1;
    s.x[6] = 1;
   
    p.extend_pool(s);

    printf("%s",f.to_str().c_str());

    PASSED("test_extend_pool");
}

void test_extend_pool_2(){
    Formulation f = read_formulation("../test_input_1");
    printf("%s\n",f.to_str().c_str());
    PoolClique p(f);

    solution_pair s;
    for(int i=0;i<f.c().size();i++){
        s.x.push_back(0);
    }

    s.x[0] = 1;
    s.x[1] = 1;
    s.x[2] = 1;
    s.x[3] = 1;
    s.x[5] = 1;
   
    p.extend_pool(s);

    printf("%s",f.to_str().c_str());

    PASSED("test_extend_pool_2");
}

void test_extend_pool_3(){
    Formulation f = read_formulation("../test_input_1");
    printf("%s\n",f.to_str().c_str());
    PoolClique p(f);

    solution_pair s;
    for(int i=0;i<f.c().size();i++){
        s.x.push_back(0);
    }

    s.x[0] = 1;
    s.x[1] = 1;
    s.x[2] = 1;
   
    p.extend_pool(s);

    printf("%s",f.to_str().c_str());
    
    PASSED("test_extend_pool_3");
}
