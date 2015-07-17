#ifndef __TEST_UTILS__
#define __TEST_UTILS__

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <vector>
#include <unordered_set>
#include <functional>

#include "io.h"
#include "formulation.h"

#define FAILED(s){printf("*** %s: FAILED\n",s);exit(1);}
#define PASSED(s){printf("*** %s: PASSED\n",s);}

Formulation read_formulation(std::string filename);
Formulation read_big_formulation(std::string filename);
bool compare_constraint(ConstraintLine* cl, int size_members, const int* vars_indexes);

#endif