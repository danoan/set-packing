#ifndef __SUBGRADIENT_ELEMENT__
#define __SUBGRADIENT_ELEMENT__

#include "constraint_line.h"
#include "solution.h"
#include "types.h"
#include "constants.h"

class SubgradientElement{
private:
    ConstraintLine* _cl;
    double _grad;
public:
    SubgradientElement(){};
    SubgradientElement(ConstraintLine* cl, double grad):_cl(cl),_grad(grad){};

    bool update_lbda(double& T, int& group);
    inline double lbda(){return _cl->lbda();};

    double compute_gradient(const int& objective_type, Solution& d);
    inline double grad(){ return _grad; };

    inline ConstraintLine* constraint(){ return _cl; }
};

#endif