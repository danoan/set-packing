#ifndef __SOLUTION__
#define __SOLUTION__

#include <vector>
#include "types.h"
#include "constants.h"

class Solution{
private:
    std::vector<solution_component> _components;
    double _vx;
    
    double _best_value;
    std::vector<solution_component> _best_solution;

    int _solution_semantic;
    bool _any_value;

    inline solution_component make_component(const double& x, const bool& fixed){ solution_component s; s.x=x; s.fixed=fixed; return s;}

public:
    Solution(){};
    Solution(int num_components, int solution_semantic=UPPER_BOUND);

    void set_component(const int& index, const double& x);
    inline const double& x(const int& index){ return _components[index].x; }
    inline const std::vector<solution_component>& x(){ return _components; }
    
    inline int num_components(){ return _components.size(); }

    void vx(const double& vx);
    inline double vx(){ return _vx; }

    inline double best_value(){ return _best_value; }
    inline std::vector<solution_component> best_solution(){ return _best_solution; }

};

#endif