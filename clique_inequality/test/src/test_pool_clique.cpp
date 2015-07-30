#include "test_pool_clique.h"

void test_extend_pool(){
    Formulation f = read_formulation("../test_input_1");
    LagrangeanFormulation lf(f);
    printf("%s\n",f.to_str().c_str());
    PoolClique p(lf);

    Solution s(f.c().size(),LOWER_BOUND);
    for(int i=0;i<s.num_components();i++){
        s.set_component(i,0);
    }

    s.set_component(2,1);
    s.set_component(4,1);
    s.set_component(6,1);
   
    p.extend_pool(s);

    printf("%s",f.to_str().c_str());

    PASSED("test_extend_pool");
}

void test_extend_pool_2(){
    Formulation f = read_formulation("../test_input_1");
    LagrangeanFormulation lf(f);
    printf("%s\n",f.to_str().c_str());
    PoolClique p(lf);

    Solution s(f.c().size(),LOWER_BOUND);
    for(int i=0;i<s.num_components();i++){
        s.set_component(i,0);
    }

    s.set_component(0,1);
    s.set_component(1,1);
    s.set_component(2,1);
    s.set_component(3,1);
    s.set_component(5,1);
   
    p.extend_pool(s);

    printf("%s",f.to_str().c_str());

    PASSED("test_extend_pool_2");
}

void test_extend_pool_3(){
    Formulation f = read_formulation("../test_input_1");
    LagrangeanFormulation lf(f);
    printf("%s\n",f.to_str().c_str());
    PoolClique p(lf);

    Solution s(f.c().size(),LOWER_BOUND);
    for(int i=0;i<s.num_components();i++){
        s.set_component(i,0);
    }

    s.set_component(0,1);
    s.set_component(1,1);
    s.set_component(2,1);
   
    p.extend_pool(s);

    printf("%s",f.to_str().c_str());
    
    PASSED("test_extend_pool_3");
}
