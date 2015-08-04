#ifndef __TYPES__
#define __TYPES__

#include <vector>
#include <bitset>

using namespace std;

#define BITCLUSTER_SIZE 32

class BenefitCostPair{
private:
    double _value;
    int _index;
public:
    inline double value(){return _value;}
    inline void value(double value){_value=value;}

    inline int index(){return _index;}
    inline void index(int index){_index=index;}
};


class SolutionComponent{
private:
    double _x;
    bool _fixed;
public:
    SolutionComponent(){};
    SolutionComponent(double p_x, bool p_fixed):_x(p_x),_fixed(p_fixed){};

    inline double x() const{return _x;}
    inline void x(double p_x){_x=p_x;}

    inline bool fixed() const{return _fixed;}
    inline void fixed(bool p_fixed){_fixed=p_fixed;}
};

typedef std::bitset<BITCLUSTER_SIZE> bit_cluster;

#endif