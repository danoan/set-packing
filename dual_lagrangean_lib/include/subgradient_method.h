#ifndef __SUBGRADIENT_METHOD__
#define __SUBGRADIENT_METHOD__

#include <vector>

#include "lagrangean_formulation.h"
#include "types.h"
#include "io.h"

class SubgradientMethod{
private:
    double _pi;
    double _T;
    double _factor;    

    int _N;
    int _max_N;
    int _no_improvement;
    int _num_it;

    vector<double> _G;
    double _sum_square_g;        

    bool _all_zeros;

    bool _DEBUG;
    void compute_gradient(LagrangeanFormulation& lf, solution_pair& d);    
    void log(vector<double>& lbda);

public:
    SubgradientMethod(LagrangeanFormulation& lf, int max_N, bool debug=false);
    bool next(vector<double>& lbda, LagrangeanFormulation& lf, solution_pair& p, solution_pair& d);
    void improvement_check(LagrangeanFormulation& lf, solution_pair& d_prime, solution_pair& d);
    bool after_check(solution_pair& p, solution_pair& d); 
};

class EX_ALL_GRADIENTS_ZERO:public exception{
    virtual const char* what() const throw(){
        return "All gradients are equal to zero, no evolution on subgradient method from here.";
    }
};

class EX_NO_LBDA_IMPROVE_SOLUTION:public exception{
    virtual const char* what() const throw(){
        return "There is no lbda that improves the current best solution.";
    }
};

#endif