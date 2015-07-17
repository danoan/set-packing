#include "test_dyn_bit_cluster.h"

void test_dyn_reduce(){
    DynamicBitCluster d1;
    DynamicBitCluster d2;

    int l1[] = {0,0,1,1,1,0,1,0};
    int l2[] = {1,0,0,1,0,0,1,0};

    for(int i=7;i>=0;i--){
        d1.add( l1[i] );
        d2.add( l2[i] );
    }

    d1<<d2;
    printf("%d %d %d\n%s\n%s\n",d1.get(1),d1.get(4),d1.count(),d1._bit_cluster_vector[0].to_string().c_str(),d2._bit_cluster_vector[0].to_string().c_str() );

    if( !(d1.get(1) && d1.get(4) && d1.count()==2) ){
        FAILED("test_dyn_reduce");
    }

    d2<<d1;
    printf("%d %d %d\n%s\n%s\n",d1.get(1),d1.get(4),d1.count(),d1._bit_cluster_vector[0].to_string().c_str(),d2._bit_cluster_vector[0].to_string().c_str() );    

    if( !(d2.get(1) && d2.get(4) && d2.count()==2) ){
        FAILED("test_dyn_reduce");
    }

    PASSED("test_dyn_reduce");
}

void test_dyn_stress(){
    DynamicBitCluster d1;

    for(int i=0;i<1<<20;i++){
        d1.add( i%2 );
    }
    
    printf("COUNT: %d\n",d1.count());
    if(d1.count()==1<<19){
        PASSED("test_dyn_stress");    
    }else{
        FAILED("test_dyn_stress");    
    }
    
}