#include "subgradient_method.h"

SubgradientMethod::SubgradientMethod(LagrangeanFormulation& p_lf, int p_max_N, 
                                    double p_pi_factor, double p_max_no_improvement_factor, 
                                    bool p_debug):_pi(2),
                                   _num_it(0),
                                   _max_N(p_max_N),
                                   _no_improvement(0),
                                   _pi_factor(p_pi_factor),
                                   _DEBUG(p_debug){
                                                                        
    if(p_max_no_improvement_factor>1){
        _max_no_improvement = p_max_no_improvement_factor;
    }else{
        _max_no_improvement = (int) floor( _max_N*p_max_no_improvement_factor );    
    }

    for(line_it cl_it=p_lf.dual_begin();cl_it!=p_lf.dual_end();cl_it++){
        _elements[ cl_it->second->index()] = SubgradientElement(cl_it->second,0);
    }    

    p_lf.add_new_constraint_callback( this, &SubgradientMethod::add_constraint_callback );
    p_lf.add_remove_constraint_callback( this, &SubgradientMethod::remove_constraint_callback );

}

bool SubgradientMethod::next(LagrangeanFormulation& lf, Solution& p, Solution& d){
    if(_num_it>_max_N or _pi<1e-8) return false;

    if(_num_it==0){ 
        _best_value = d.vx();
    }

    double sum_square_g = 0;
    double temp;
    for(subelem_it se_it=_elements.begin();se_it!=_elements.end();se_it++){
        temp = se_it->second.compute_gradient(lf.objective_type(),d);
        sum_square_g += temp*temp;
    }

    if(sum_square_g==0) return false;

    double T;
    double factor = 1.0;  //Can be used as a way to randomize the algorithm. Not being used, however
    if(lf.objective_type()==MAX_TYPE){
        T= ( factor*_pi*(1.05*d.vx()-p.vx()) )/sum_square_g;    
    }else{
        T= ( factor*_pi*(1.05*p.vx()-d.vx()) )/sum_square_g;    
    }

    int group;
    for(subelem_it se_it =_elements.begin();se_it!=_elements.end();se_it++){
        if( se_it->second.update_lbda(T,group) ){
            if(group==1){   //ACTIVE
                lf.make_active_constraint( se_it->second.constraint() );
            }else{
                lf.make_inactive_constraint( se_it->second.constraint() );
            }
        }
    }  
    lf.update_lagrangean_costs();

    if(_DEBUG) log(p,d,lf,T);

    return true;
}

void SubgradientMethod::improvement_check(LagrangeanFormulation& lf, Solution& d){

    if( !( (lf.objective_type()==MAX_TYPE && _best_value>d.vx()) xor 
           (lf.objective_type()==MIN_TYPE && _best_value<d.vx()) ) ){                
            _no_improvement+=1;
    }else{
        _best_value = d.vx();
        _no_improvement=0;
    }

    if(_no_improvement>=_max_no_improvement){
        _no_improvement = 0;
        _pi = _pi*_pi_factor;
    }

}

bool SubgradientMethod::after_check(Solution& p, Solution& d){
    _num_it+=1;
   
    if( ( fabs( (d.vx()-p.vx())/p.vx()) ) <= 0.0001 ){
        return false;
    }       

    return true;    
}

void SubgradientMethod::log(Solution& p, Solution& d, LagrangeanFormulation& lf, double& T){
    std::ostringstream _buffer;

    _buffer << "SUBGRADIENT ITERATION: " << _num_it << std::endl;
    _buffer << "PI: " << _pi << std::endl;
    _buffer << "STEP SIZE: " << T << std::endl;
    _buffer << "GRADIENT: (";
    
    for(subelem_it se_it=_elements.begin();se_it!=_elements.end();se_it++){
        _buffer << se_it->second.grad() << ",";
    }
    _buffer << ")" << std::endl;
    _buffer << "LBDA: (";

    for(subelem_it se_it=_elements.begin();se_it!=_elements.end();se_it++){
        _buffer << se_it->second.lbda() << ",";
    }
            
    _buffer << ")" << std::endl;

    _buffer << "STEP SIZE: " << T << std::endl;
    _buffer << "NO IMPROVEMENT: " << _no_improvement << std::endl;
    _buffer << "BEST BOUND: " << _best_value << std::endl;

    _out = _buffer.str();
}
