#ifndef __DYN_BIT_CLUSTER__
#define __DYN_BIT_CLUSTER__

#ifndef __DEBUG__ 
    #define ACCESS_MODE private:
#else
    #define ACCESS_MODE public:
#endif

#include <vector>
#include <cmath>

#include "types.h"

class DynamicBitCluster{
ACCESS_MODE
    std::vector<bit_cluster> _bit_cluster_vector;
    int _curr_bit;
    int _curr_bit_cluster;
    size_t _n;

public:
    DynamicBitCluster():_curr_bit(0),_curr_bit_cluster(-1),_n(0){};
    
    int add(int p_bit);
    //void rem(int p_bit_cluster_index, int p_bit);
    inline size_t size() const{return _bit_cluster_vector.size();}
    inline size_t bits() const{ return _n; }
    inline const std::vector<bit_cluster>& bit_cluster_vector(){return _bit_cluster_vector;}    

    void resize_and_clear(size_t p_num_bits);
    void resize_and_clear(size_t p_num_bits, unsigned long long value);
    void inline set(int p_num_bit){ int pos = (int) floor(1.0*p_num_bit/BITCLUSTER_SIZE); _bit_cluster_vector[pos][p_num_bit%BITCLUSTER_SIZE]=1; }
    void inline reset(int p_num_bit){ int pos = (int) floor(1.0*p_num_bit/BITCLUSTER_SIZE); _bit_cluster_vector[pos][p_num_bit%BITCLUSTER_SIZE]=0; }
    bool inline get(int p_num_bit){ int pos = (int) floor(1.0*p_num_bit/BITCLUSTER_SIZE); return _bit_cluster_vector[pos][p_num_bit%BITCLUSTER_SIZE]==1; }
    int count();

    //Reduce (intersection) operator
    DynamicBitCluster& operator<<(const DynamicBitCluster& p_dy);
    DynamicBitCluster& operator<<(const int& v);

};

#endif