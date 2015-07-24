#include "dyn_bit_cluster.h"

int DynamicBitCluster::add(int p_bit){

    if(_curr_bit==0){
        _bit_cluster_vector.push_back(bit_cluster(0));
        _curr_bit_cluster+=1;
    }

    _bit_cluster_vector[_curr_bit_cluster][_curr_bit] = p_bit; 
    _curr_bit=(_curr_bit+1)%32;

    return _n++;
}

void DynamicBitCluster::resize_and_clear(size_t p_num_bits){
    resize_and_clear(p_num_bits,0);
}

void DynamicBitCluster::resize_and_clear(size_t p_num_bits, unsigned long long value){
    //It clears everyting
    int _vector_size = (int) ceil(1.0*p_num_bits/BITCLUSTER_SIZE);
    _bit_cluster_vector.resize(_vector_size);
    for(int i=0;i<_vector_size;i++){
        _bit_cluster_vector[i] = bit_cluster(value);
    }

    _n = p_num_bits;
    _curr_bit_cluster = _vector_size-1;
}

DynamicBitCluster& DynamicBitCluster::operator<<(const DynamicBitCluster& p_dy){
    if( p_dy.size()!=size() ){        
        exit(1);
    }

    for(int i=0;i<_bit_cluster_vector.size();i++){
        _bit_cluster_vector[i] &= p_dy._bit_cluster_vector[i];
    }

    return *(this);
}

DynamicBitCluster& DynamicBitCluster::operator<<(const int& v){
    for(int i=0;i<_bit_cluster_vector.size();i++){
        _bit_cluster_vector[i] &= v;
    }

    return *(this);
}

int DynamicBitCluster::count(){
    int count=0;
    for(int i=0;i<_bit_cluster_vector.size();i++){
        count+=_bit_cluster_vector[i].count();
    }
    return count;
}