#ifndef __TYPES__
#define __TYPES__

#include <vector>
#include <bitset>

using namespace std;

#define BITCLUSTER_SIZE 32

typedef struct{
    double x;
    bool fixed;
} solution_component;

typedef std::bitset<BITCLUSTER_SIZE> bit_cluster;

#endif