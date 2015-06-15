#include <cstdio>
#include <cstdlib>

#include "util.h"
#include "io.h"

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

void example_1(){
    int n = 4;
    int m = 1;
    double* c = new double[4];
    int* a = new int[4];
    int b = 7;

    c[0]=16; c[1]=19; c[2]=23; c[3]=28;
    a[0]=2; a[1]=3; a[2]=4; a[3]=5;

    int* x = new int[4];
    double solution = knapsack(c,a,b,n,m,x);

    printf("SOLUTION: %.4f\n", solution);
    printf("X=( ");
    for(int i=0;i<n;i++){
        printf("%d ", x[i]);
    }
    printf(")\n");    
}

void example_2(){
    int n = 4;
    int m = 1;
    long* c = new long[4];
    int* a = new int[4];
    int b = 7;

    c[0]=16; c[1]=19; c[2]=23; c[3]=28;
    a[0]=2; a[1]=3; a[2]=4; a[3]=5;


    int* x = new int[4];
    long value = knapsack(c,a,b,n,m,x);

    printf("SOLUTION: %.4f\n", value);
    printf("X=( ");
    for(int i=0;i<n;i++){
        printf("%d ", x[i]);
    }
    printf(")\n");    
}

void example_3(){
    //EVERYTHING MUST BE POSITIVE IN ORDER TO WORK
    FILE* f = load_file("input_util_10");
    vector< vector<int> > A_index;
    vector< vector<double> > A_cost;

    vector<double> b;
    vector<double> c;
    int objective_type;
    vector<int> op;

    read_input(A_index,A_cost,b,c,op,objective_type,f);
    fclose(f);  

    Formulation form(A_index,A_cost,b,c,op,objective_type);    

    int* x = new int[c.size()];
    double solution = knapsack(form,x);

    printf("SOLUTION: %.4f\n", solution);
    printf("X=( ");
    for(int i=0;i<c.size();i++){
        printf("%d ", x[i]);
    }
    printf(")\n");    

    delete[] x;   
}

int main(){
    example_1();
    example_2();
    example_3();

    return 0;
}