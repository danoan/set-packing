#include "solution.h"

Solution::Solution(int num_components, int solution_semantic):_solution_semantic(solution_semantic),_any_value(false){
    _components.resize(num_components);
    for(int i=0;i<num_components;i++){
        _components[i] = make_component(0,false);
    }
}

void Solution::set_component(const int& index, double x){ 
    if( !(_components[index].fixed()) ){
        _components[index].x(x);     
    }    
}

void Solution::vx(double vx){ 
    _vx = vx; 
    if(_any_value==false){
        _any_value = true;
        _best_value = _vx;
        _best_solution = _components;
    }
    
    if(_solution_semantic==UPPER_BOUND){
        if( _vx<_best_value){
            _best_value = _vx;
            _best_solution = _components;
        }
    }else{
        if( _vx>_best_value){
            _best_value = _vx;
            _best_solution = _components;
        }        
    }
}