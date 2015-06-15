#include <stdio.h>

int main(){
    int d,t;
    double vm;

    scanf("%d %d", &d, &t);
    vm = (double) d/t;

    printf("%lf\n", vm);
    return 0;
}