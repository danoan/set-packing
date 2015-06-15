#include <stdio.h>
#include "pisinger.h"

void test_1(){
  long p[6] = {7,4,8,6,2,5};
  int w[6] = {2,3,5,4,2,3};
  int x[6];

  int n = 6;
  int c = 9;

  int z = minknap(n,p,w,x,c);  

  printf("x= (");
  for(int i=0;i<n;i++){
    printf("%d,",x[i]);
  }
  printf(")\nz= %d\n",z);  
}

int test_2(){
  long p[6] = {-2,-1,-3,-4,-5,-1};
  int w[6] = {2,3,5,4,2,3};
  int x[6];

  int n = 6;
  int c = 9;

  int z = minknap(n,p,w,x,c);  

  printf("x= (");
  for(int i=0;i<n;i++){
    printf("%d,",x[i]);
  }
  printf(")\nz= %d\n",z);  
}

int main(){
  test_1();

  return 0;
}