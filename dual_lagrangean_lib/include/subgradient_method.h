#ifndef __SUBGRADIENT_METHOD__
#define __SUBGRADIENT_METHOD__

#include <vector>

#include "lagrangean_formulation.h"
#include "types.h"
#include "io.h"

class SubgradientMethod{
private:
    double _pi;
    double _pi_factor;
    double _T;
    double _factor;    

    int _max_N; //Max Number of Iterations
    
    int _no_improvement;    //Number of iterations without any improvement
    int _max_no_improvement;    //Number of no improvement iterations to happen to halve _pi
    int _num_it;    // Current Iteration Number

    vector<double> _G;  //Gradient Vector
    double _sum_square_g;   //Sum of the square of the gradient components

    bool _all_zeros;

    bool _DEBUG;
    void compute_gradient(LagrangeanFormulation& lf, solution_pair& d);    
    void log(vector<double>& lbda);

public:
    SubgradientMethod(LagrangeanFormulation& p_lf, int p_max_N, double p_pi_factor, double p_max_no_improvement_factor, bool p_debug=false);
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