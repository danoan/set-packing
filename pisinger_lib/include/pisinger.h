#ifndef __PISINGER__
#define __PISINGER__

extern "C"{
    namespace pisinger{
        #include "minknap.h"
    }
}

pisinger::stype minknap(int n, double *c, double *a, int *x, double b);

#endif