#include <stdio.h>
#include "pisinger.h"

void test_1(){
  double p[6] = {7.0,4.0,8.0,6.0,2.0,5.0};
  double w[6] = {2.0,3.0,5.0,4.0,2.0,3.0};
  int x[6];

  int n = 6;
  double c = 9.0;

  double z = minknap(n,p,w,x,c);  

  printf("x= (");
  for(int i=0;i<n;i++){
    printf("%d,",x[i]);
  }
  printf(")\nz= %lf\n",z);  
}

int main(){
  test_1();
  return 0;
}