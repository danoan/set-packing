#ifndef __SUBGRADIENT_METHOD__
#define __SUBGRADIENT_METHOD__

#include <vector>
#include <unordered_map>
#include <sstream>

#include "lagrangean_formulation.h"
#include "subgradient_element.h"
#include "solution.h"
#include "types.h"
#include "io.h"

typedef std::unordered_map<int,SubgradientElement>::iterator subelem_it;

class SubgradientMethod{
private:
    double _pi;
    double _pi_factor;

    int _max_N; //Max Number of Iterations
    double _best_value;
    bool _start;
    
    int _no_improvement;    //Number of iterations without any improvement
    int _max_no_improvement;    //Number of no improvement iterations to happen to halve _pi
    int _num_it;    // Current Iteration Number
    double _step_size;

    std::unordered_map<int,SubgradientElement> _elements;
    bool _all_zeros;    

    bool _DEBUG;
    void compute_gradient(LagrangeanFormulation& lf, Solution& d);    

    void remove_constraint_callback(ConstraintLine* cl){ _elements.erase(cl->index());};
    void add_constraint_callback(ConstraintLine* cl){ _elements[cl->index()] = SubgradientElement(cl,0); };

public:
    SubgradientMethod(LagrangeanFormulation& p_lf, int p_max_N, double p_pi_factor, double p_max_no_improvement_factor, bool p_debug=false);
    bool next(LagrangeanFormulation& lf, Solution& p, Solution& d);
    bool improvement_check(LagrangeanFormulation& lf, Solution& d);
    bool after_check(Solution& p, Solution& d); 
    
    inline const double& pi(){ return _pi; }
    inline const int& no_improvement(){ return _no_improvement; }
    inline const int& num_it(){ return _num_it; }
    inline const double& step_size(){ return _step_size; }
    inline subelem_it begin(){ return _elements.begin(); }
    inline subelem_it end(){ return _elements.end(); }

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