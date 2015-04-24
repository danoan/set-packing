#include <stdio.h>
#include "pisinger.h"

int main(){
  double p[6] = {7,4,8,6,2,5};
  double w[6] = {2,3,5,4,2,3};
  int x[6];

  int n = 6;
  double c = 9;

  int z = minknap(n,p,w,x,c);
  int i;

  printf("x= (");
  for(i=0;i<n;i++){
    printf("%d,",x[i]);
  }
  printf(")\nz= %d\n",z);

  return 0;
}