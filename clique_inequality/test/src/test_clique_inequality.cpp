#include "test_clique_inequality.h"

ci_expected_data create_data(bool maximal_clique,int total_active_variables,
                             int index_sum_active_variables,const int* r,
                             int total_variables_form){
    ci_expected_data c;
    c.maximal_clique = maximal_clique;
    c.total_active_variables = total_active_variables;
    c.index_sum_active_variables = index_sum_active_variables;
    std::vector<int> variables(r,r + sizeof(r) / sizeof(r[0]));

    int bit_cluster_size = (int) ceil(1.0*total_variables_form/BITCLUSTER_SIZE);
    c.variables_in_use.resize(bit_cluster_size);

    for(int i=0;i<variables.size();i++){
        int index_var = variables[i];
        int pos = floor( 1.0*index_var/BITCLUSTER_SIZE );
        int offset = index_var%BITCLUSTER_SIZE;

        c.variables_in_use[pos].set( offset );
    }

    return c;
}

void test_correctness(){
    Formulation f = read_formulation("../test_input_1");

    vector<ci_expected_data> form_data;

    static const int r1[] = {0,2,5};
    static const int r2[] = {1,5};
    static const int r3[] = {1,3};
    static const int r4[] = {0,1,3};
    static const int r5[] = {2,4};
    static const int r6[] = {4,6};
    static const int r7[] = {0,1,2,4};
    static const int r8[] = {0,3};
    static const int r9[] = {2,3,5,6};
    static const int r10[] = {2,6};

    form_data.push_back( create_data(false,3,7,r1,7) );
    form_data.push_back( create_data(false,2,6,r2,7) );
    form_data.push_back( create_data(false,2,4,r3,7) );
    form_data.push_back( create_data(false,3,4,r4,7) );
    form_data.push_back( create_data(false,2,6,r5,7) );
    form_data.push_back( create_data(false,2,10,r6,7) );
    form_data.push_back( create_data(false,4,7,r7,7) );
    form_data.push_back( create_data(false,2,3,r8,7) );
    form_data.push_back( create_data(false,4,16,r9,7) );
    form_data.push_back( create_data(false,2,8,r10,7) );


    ConstraintLine* cl;
    for(line_it l_it=f.begin();l_it!=f.end();l_it++){
        cl = (*l_it).second;
        CliqueInequality ci_1(cl,7);
        CliqueInequality ci_2(cl,7);
        ci_expected_data data = form_data[ cl->index() ];
        
        if(data.maximal_clique!=ci_1.is_maximal()){
            printf("MAXIMAL ERROR %d\n",cl->index());            
            FAILED("test_correctness");
        }
        if(data.total_active_variables!=ci_1.active_variables()){
            printf("TOTAL ACTIVE ERROR (%d) %d-%d\n",cl->index(),data.total_active_variables,ci_1.active_variables());
            FAILED("test_correctness");
        }
        if(data.index_sum_active_variables!=ci_1.index_sum_active()){
            printf("INDEX SUM ERROR (%d) %d-%d\n",cl->index(),data.index_sum_active_variables,ci_1.index_sum_active());
            FAILED("test_correctness");
        }
        if( !(ci_1==ci_2) ){
            printf("== OPERATOR ERRO %d\n",cl->index());
            FAILED("test_correctness");
        }
    }

    PASSED("test_correctness");
}

void test_dominated(){
    Formulation f = read_formulation("../test_input_1");    
    ConstraintLine* cl;
    ConstraintLine* cl_1;
    ConstraintLine* cl_2;

    int r1[] = {2,4};
    int r2[] = {0,1,2,4};

    for(line_it l_it=f.begin();l_it!=f.end();l_it++){
        cl = (*l_it).second;
        if( compare_constraint(cl,2,r1 ) ){
            cl_1 = cl;  //2 3 5
        }else if( compare_constraint(cl,4,r2) ){
            cl_2 = cl;  //4 1 2 3 5
        }
    }

    CliqueInequality c1(cl_1,7);
    CliqueInequality c2(cl_2,7);

    if( c1>c2 ){
        printf("DOMINATES ERROR\n");
        FAILED("test_dominated");
    }

    if( !(c1<c2) ){
        printf("DOMINATED ERROR\n");   
        FAILED("test_dominated");
    }

    if( !(c2>c1) ){
        printf("DOMINATES ERROR\n");   
        FAILED("test_dominated");
    }

    if( (c2<c1) ){
        printf("DOMINATED ERROR\n");
        FAILED("test_dominated");
    }

    PASSED("test_dominated");
}

void test_hash(){
    Formulation f = read_formulation("../test_input_2");

    int r1[] = {4,5};   //index 9
    int r2[] = {1,2,3}; //index 10

    std::vector< CliqueInequality > cliques;
    int c1,c2;
    for(line_it l_it=f.begin();l_it!=f.end();l_it++){
        cliques.push_back( CliqueInequality( (*l_it).second, 7) );
        if( compare_constraint( (*l_it).second, 2, r1) ){
            c1 = cliques.size()-1;
        }else if( compare_constraint( (*l_it).second,3,r2) ){
            c2 = cliques.size()-1;
        }
    }

    for(int i=0;i<cliques.size()-1;i++){
        for(int j=i+1;j<cliques.size();j++){
            if(cliques[i]==cliques[j]){
                printf("ERRO %d %d\n",i,j);
                FAILED("test_hash");
            }
        }
    }


    //CLIQUES 0 and 9 have the same hash_value
    std::unordered_set<CliqueInequality> myset;
    std::pair< std::unordered_set<CliqueInequality>::iterator, bool> res = myset.insert(cliques[c1]);
    myset.insert(cliques[c2]);

    std::hash<CliqueInequality> myhash;

    if( myhash( *(res).first ) != myhash(cliques[c2]) ){
        printf("Different Hash Values\n");
        FAILED("test_hash");
    }

    if( myset.size()<2 ){
        printf("Set size is less than 2\n");
        FAILED("test_hash");
    }

    PASSED("test_hash");
}

