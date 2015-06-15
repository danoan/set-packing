#ifndef __PD_SOLVER__
#define __PD_SOLVER__

#include "formulation.h"
#include "constraint_line.h"
#include "io.h"


double knapsack(Formulation& f,int* x);
ConstraintLine* get_knapsack_constraint(Formulation& f);

template<typename T>
T knapsack(T* c, int* a, const int b, const int n, const int m, int* x){
    T** pd;
    int** path;

    pd = (T**) malloc(sizeof(T*)*(n+1));
    if(pd==NULL) exit(1);

    path = (int**) malloc(sizeof(int*)*(n+1));
    if(path==NULL) exit(1);

    for(int i=0;i<n+1;i++){
        pd[i] = (T*) malloc(sizeof(T)*(b+1));
        if(pd[i]==NULL) exit(1);

        path[i] = (int*) malloc(sizeof(int)*(b+1));
        if(path[i]==NULL) exit(1);

        pd[i][0] = 0;
    }    

    for(int j=0;j<b+1;j++){
        pd[0][j] = 0;
    }

    for(int i=1;i<n+1;i++){
        for(int j=1;j<b+1;j++){
            if( a[i-1] > j ){
                pd[i][j] = pd[i-1][j];
                path[i][j] = 0;
            }else{
                if( c[i-1] + pd[i-1][ j-a[i-1] ] > pd[i-1][j] ){
                    pd[i][j] = c[i-1] + pd[i-1][ j-a[i-1] ];
                    path[i][j] = 1;
                }else{
                    pd[i][j] = pd[i-1][j];
                    path[i][j] = 0;
                }
            }
        }
    }

    int i=n;
    int j=b;
    while(i>0 && j>0){
        if( path[i][j]==0 ){
            x[i-1] = 0;
            i-=1;
        }else{
            x[i-1] = 1;
            j-=a[i-1];
            i-=1;
        }
    }

    // for(int i=0;i<n+1;i++){
    //     for(int j=0;j<b+1;j++){    
    //         printf("%.2f ",pd[i][j]);
    //     }
    //     printf("\n");
    // }

    // for(int i=0;i<n+1;i++){
    //     for(int j=0;j<b+1;j++){    
    //         printf("%d ",path[i][j]);
    //     }
    //     printf("\n");
    // }    

    T value = pd[n][b];

    for(int i=0;i<n+1;i++){
        free(pd[i]);
        free(path[i]);
    }
    free(pd);
    free(path);    

    return value;
}

#endif