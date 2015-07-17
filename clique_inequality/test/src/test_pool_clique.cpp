#include "test_pool_clique.h"

void test_pool_clique_check_order(){
    Formulation f = read_formulation("../test_input_1");
    PoolClique p(f);

    std::vector<int> degrees;
    clique_queue cq = p._cliques_queue;
    while(!cq.empty()){        
        degrees.push_back( cq.top().active_variables() );
        cq.pop();
    }

    int last=-1;
    for(int i=0;i<degrees.size();i++){
        if(degrees[i]>=last){
            last = degrees[i];
        }else{
            FAILED("test_pool_clique_check_order");
        }
    }

    PASSED("test_pool_clique_check_order");
}

void test_extend_pool(){
    Formulation f = read_formulation("../test_input_1");
    printf("%s\n",f.to_str().c_str());
    PoolClique p(f);
   
    do{        
        print_current_pool(p);    
    }while(p.extend_pool());
    
    print_current_pool(p);    

    printf("%s",f.to_str().c_str());
}

void test_extend_pool_2(){
    Formulation f = read_formulation("../pb_100rnd0100.dat");
    printf("%s\n",f.to_str().c_str());
    PoolClique p(f);
   
    do{        
        print_current_pool(p);    
    }while(p.extend_pool());
    
    print_current_pool(p);    

    printf("%s",f.to_str().c_str());
}

void print_current_pool(PoolClique& p){
    printf("CURRENT POOL\n");

    CliqueInequality ci;
    clique_queue temp = p._cliques_queue;
    while(!temp.empty()){
        ci = temp.top();
        temp.pop();
        for(member_it it=ci.begin();it!=ci.end();it++){
            printf("%d ", (*it).index);
        }printf("\n");        
    }
}