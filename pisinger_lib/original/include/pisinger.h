#ifndef __PISINGER__
#define __PISINGER__

extern "C"{
    namespace pisinger{
        #include "minknap.h"
    }
}

pisinger::stype minknap(int n, long *c, int *a, int *x, int b);

#endif