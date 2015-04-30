#include "util.h"
#include "gtest/gtest.h"

#include <exception>
#define INPUT_FILES_FOLDER "test_input/"

FILE* load_file(string filename){
    string f_path;
    try{        
        f_path = string(ROOT_PATH) + string(INPUT_FILES_FOLDER) + filename;
        FILE* f = fopen(f_path.c_str(),"r");        
        return f;
    }catch(std::exception& ex){
        printf("File could not be loaded %s\n", f_path.c_str());
        printf("Error: %s\n",ex.what());
    }

    return NULL;
}

void standard_test(vector< vector<double> >& A, int a1, int a2,
                   vector<double>& b, int b1,
                   vector<double>& c, int c1,
                   vector<int>& op, int op1,
                   int& objective_type, int type){

    ASSERT_EQ(A.size(),a1) << "Wrong dimension for restriction matrix";
    ASSERT_EQ(A[0].size(),a2) << "Wrong dimension for restriction matrix";
    
    ASSERT_EQ(b.size(),b1);
    ASSERT_EQ(c.size(),c1);

    ASSERT_EQ(op.size(),op1);
    EXPECT_EQ(objective_type,type);
}

template<int n, int m>
void values_test(vector< vector<double> >& A, double ex_A[m][n],
                 vector<double>& b, double ex_b[m],
                 vector<double>& c, double ex_c[n],
                 vector<int>& op, int ex_op[m]){

    for(int i=0;i<m;i++){
        if(i>=A.size()) continue;
        for(int j=0;j<n;j++){
            if(j>=A[i].size()) continue;
            EXPECT_EQ(A[i][j],ex_A[i][j]) << "i:" << i << " j:" << j;
        }
        EXPECT_EQ(b[i],ex_b[i]) << "i:" << i;
        EXPECT_EQ(op[i],ex_op[i]) << "i:" << i;
    }

    for(int j=0;j<n;j++){
        EXPECT_EQ(c[j],ex_c[j]) << "j:" << j;
    }
}

template<int n, int m>
void values_test(vector< vector<int> >& A_index, int ex_A_index[m][n],
                 vector< vector<double> >& A_cost, double ex_A_cost[m][n],
                 vector<double>& b, double ex_b[m],
                 vector<double>& c, double ex_c[n],
                 vector<int>& op, int ex_op[m]){

    for(int i=0;i<m;i++){
        if(i>=A_index.size()) continue;
        for(int j=0;j<n;j++){
            if(j>=A_index[i].size()) continue;
            EXPECT_EQ(A_index[i][j],ex_A_index[i][j]) << "i:" << i << " j:" << j;
            EXPECT_EQ(A_cost[i][j],ex_A_cost[i][j]) << "i:" << i << " j:" << j;
        }
    }

    for(int i=0;i<m;i++){
        EXPECT_EQ(b[i],ex_b[i]) << "i:" << i;
        EXPECT_EQ(op[i],ex_op[i]) << "i:" << i;
    }

    for(int j=0;j<n;j++){
        EXPECT_EQ(c[j],ex_c[j]) << "j:" << j;
    }

}

void test_single_success(){
    FILE* f = load_file("input_util_1");

    vector< vector<double> > A;
    vector<double> b;
    vector<double> c;
    int objective_type;
    vector<int> op;

    read_input(A,b,c,op,objective_type,f);
    fclose(f);

    standard_test(A,1,1,b,1,c,1,op,1,objective_type,MAX_TYPE);

    EXPECT_EQ(A[0][0],12);
    EXPECT_EQ(b[0],17);
    EXPECT_EQ(c[0],5);

    EXPECT_EQ(op[0],LESSER_EQUAL);    
}

void test_stress_success(){
    FILE* f = load_file("input_util_2");
    vector< vector<double> > A;
    vector<double> b;
    vector<double> c;
    int objective_type;
    vector<int> op;

    read_input(A,b,c,op,objective_type,f);
    fclose(f);

    standard_test(A,13,10,b,13,c,10,op,13,objective_type,MIN_TYPE);

    double ex_A[13][10] = { {12,17,10,25,0,0,0,0,0,0},
                     {2,1,1,2,0,0,0,0,0,0},
                     {0,1,0,-2,0,0,0,0,0,0},
                     {0,0,0,0,-3,0,4,0,0,0},
                     {-10,0,0,0,0,3,-5,3,4,0},
                     {-10,3,0,0,0,0,5,0,1,0},
                     {0,1,0,0,3,-2,0,0,0,0},
                     {-5,0,0,0,0,0,0,1,1,1},
                     {1,0,3,-4,0,0,7,0,0,0},
                     {0,0,0,11,-7,0,0,0,10,0},
                     {0,-3,0,0,12,-4,-10,0,0,0},
                     {0,-1,4,0,2,0,3,0,0,0},
                     {0,0,1,0,-3,0,0,0,4,0}
                 };

    double ex_b[13] = {17,10,9,13,90,101,74,40,50,30,24,74,80};
    double ex_c[10] = {5,0,8,7,-2,5,-7,11,-13,1};

    int ex_op[13] = {LESSER_EQUAL, EQUAL, GREATER, EQUAL, 
                     GREATER, GREATER_EQUAL, EQUAL, LESSER, 
                     EQUAL, EQUAL, GREATER_EQUAL, GREATER, 
                     LESSER};    

    values_test<10,13>(A,ex_A,b,ex_b,c,ex_c,op,ex_op);    
}

void test_no_constraints_number(){
    FILE* f = load_file("input_util_3");
}

void test_no_objective_function_type(){
    FILE* f = load_file("input_util_4");
}

void test_no_coefficient_on_equations_1(){
    FILE* f = load_file("input_util_5");
}

void test_no_coefficient_on_equations_2(){
    FILE* f = load_file("input_util_6");
}

void test_operation_signs_separated(){
    FILE* f = load_file("input_util_7");
}

void test_number_of_variables_greater_than_indicated(){
    FILE* f = load_file("input_util_8");
}


void test_high_stress_success(){
    FILE* f = load_file("input_util_9");
    vector< vector<double> > A;
    vector<double> b;
    vector<double> c;
    int objective_type;
    vector<int> op;

    read_input(A,b,c,op,objective_type,f);
    fclose(f);

    standard_test(A,28,10,b,28,c,10,op,28,objective_type,MAX_TYPE);

    double ex_A[28][10] = { {0,0,0,0,0,0,1,0,0,0},
                        {0,0,0,1,0,0,0,0,0,0},
                        {0,0,0,0,1,0,0,0,1,1},
                        {0,0,0,0,0,0,1,0,0,1},
                        {0,0,0,0,0,1,1,0,1,1},
                        {0,0,0,0,1,0,0,0,1,0},
                        {0,0,0,1,0,0,0,0,0,1},
                        {0,0,0,0,0,0,0,1,0,0},
                        {1,1,1,0,0,0,0,0,1,0},
                        {1,0,0,0,1,0,0,0,0,0},
                        {0,0,0,0,1,0,0,0,0,0},
                        {0,1,0,0,0,0,0,0,0,0},
                        {1,1,0,0,1,0,0,0,0,0},
                        {0,1,0,1,0,0,1,0,0,0},
                        {1,1,0,0,0,1,1,0,0,0},
                        {0,0,1,0,0,0,1,1,0,0},
                        {0,0,0,0,0,0,1,0,0,0},
                        {1,1,0,0,0,1,0,0,0,0},
                        {1,0,0,0,0,0,1,0,0,0},
                        {1,0,0,1,0,1,0,0,0,0},
                        {0,0,0,0,0,1,1,0,0,0},
                        {0,0,0,1,0,0,0,0,1,0},
                        {0,0,0,0,0,0,1,0,1,0},
                        {1,0,0,0,1,0,0,0,0,0},
                        {0,0,0,0,0,0,0,1,0,1},
                        {1,0,0,0,0,0,0,0,0,0},
                        {1,0,0,0,0,0,0,0,0,1},
                        {1,0,0,0,0,0,0,0,0,0}  
                    };    
    double ex_b[28] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    double ex_c[10] = {1,1,1,1,1,1,1,1,1,1};
    int ex_op[28] = {LESSER_EQUAL,LESSER_EQUAL,LESSER_EQUAL,LESSER_EQUAL,LESSER_EQUAL,LESSER_EQUAL,LESSER_EQUAL,
                  LESSER_EQUAL,LESSER_EQUAL,LESSER_EQUAL,LESSER_EQUAL,LESSER_EQUAL,LESSER_EQUAL,LESSER_EQUAL,
                  LESSER_EQUAL,LESSER_EQUAL,LESSER_EQUAL,LESSER_EQUAL,LESSER_EQUAL,LESSER_EQUAL,LESSER_EQUAL,
                  LESSER_EQUAL,LESSER_EQUAL,LESSER_EQUAL,LESSER_EQUAL,LESSER_EQUAL,LESSER_EQUAL,LESSER_EQUAL};


    values_test<10,28>(A,ex_A,b,ex_b,c,ex_c,op,ex_op);  

}

void test_set_packing_read(){
    FILE* f = load_file("input_util_10");
    vector< vector<int> > A_index;
    vector< vector<double> > A_cost;

    vector<double> b;
    vector<double> c;
    int objective_type;
    vector<int> op;

    read_input(A_index,A_cost,b,c,op,objective_type,f);
    fclose(f);

    standard_test(A_cost,5,3,b,5,c,10,op,5,objective_type,MAX_TYPE);

    int ex_A_index[5][10] = { {0,1,2,0,0,0,0,0,0,0},
                     {3,4,5,6,7,0,0,0,0,0},
                     {2,3,0,0,0,0,0,0,0,0},
                     {5,6,7,8,0,0,0,0,0,0},
                     {7,8,9,0,0,0,0,0,0,0},
                 };

    double ex_A_cost[5][10] = { {1,1,1,0,0,0,0,0,0,0}, 
                                {1,1,1,1,1,0,0,0,0,0},
                                {1,1,0,0,0,0,0,0,0,0},
                                {1,1,1,1,0,0,0,0,0,0},
                                {1,1,1,0,0,0,0,0,0,0} };

    double ex_b[5] = {1,1,1,1,1};
    double ex_c[10] = {1,2,3,4,5,6,7,8,9,10};

    int ex_op[5] = {LESSER_EQUAL,LESSER_EQUAL,LESSER_EQUAL,LESSER_EQUAL,LESSER_EQUAL};    

    values_test<10,5>(A_index,ex_A_index,A_cost,ex_A_cost,b,ex_b,c,ex_c,op,ex_op);    
}

int main(int argc, char* argv[]){
    test_single_success();
    test_stress_success();
    test_high_stress_success();
    test_set_packing_read();

    return 0;
}