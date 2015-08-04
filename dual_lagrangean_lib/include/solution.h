#ifndef __SOLUTION__
#define __SOLUTION__

#include <vector>
#include "types.h"
#include "constants.h"

class Solution{
private:
    std::vector<SolutionComponent> _components;
    double _vx;
    
    double _best_value;
    std::vector<SolutionComponent> _best_solution;

    int _solution_semantic;
    bool _any_value;

    inline SolutionComponent make_component(const double& x, const bool& fixed){ SolutionComponent s(x,fixed); return s;}

public:
    Solution(){};
    Solution(int num_components, int solution_semantic=UPPER_BOUND);

    void set_component(const int& index, double x);
    
    inline double x(const int& index){ return _components[index].x(); }    
    inline const std::vector<SolutionComponent>& x(){ return _components; }

    inline bool is_fixed(const int& index){ return _components[index].fixed(); }

    inline void fix(const int& index, int val){ _components[index].x(val); _components[index].fixed(true);}
    
    inline int num_components(){ return _components.size(); }

    void vx(double vx);
    inline double vx(){ return _vx; }

    inline double best_value(){ return _best_value; }
    inline std::vector<SolutionComponent> best_solution(){ return _best_solution; }

};

#endif