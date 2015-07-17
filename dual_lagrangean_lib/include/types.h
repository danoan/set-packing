#ifndef __TYPES__
#define __TYPES__

#include <vector>
#include <bitset>

using namespace std;

#define BITCLUSTER_SIZE 32

typedef struct _solution_pair{
    vector<double> x;
    double vx;
} solution_pair;

typedef std::bitset<BITCLUSTER_SIZE> bit_cluster;

#endif