#include "subgradient_method.h"

SubgradientMethod::SubgradientMethod(LagrangeanFormulation& lf, int max_N, bool debug):_pi(2),_N(1),_num_it(0),
                                                                                        _max_N(max_N),_DEBUG(debug), 
                                                                                        _no_improvement(0){
    _G.resize(lf.num_constraints());
}

void SubgradientMethod::compute_gradient(LagrangeanFormulation& lf, solution_pair& d){
    _all_zeros = true;
    double g;
    double sum_a;

    _sum_square_g = 0;
    int i;
    line_it it_r;
    ConstraintLine rl;
    ConstraintMember rm;
    for(i=0,it_r=lf.dual_begin();it_r!=lf.dual_end();it_r++,i++){
        rl = *(*it_r);
        sum_a=0;
        for(member_it it_m=rl.begin();it_m!=rl.end();it_m++){
            rm = (*it_m);
            sum_a+= rm.cost*d.x[rm.index];
        }
        g = rl.rhs() - sum_a;

        if(lf.objective_type()==MAX_TYPE){
            _G[i]=-g;
        }else{
            _G[i]=g;
        }    

        if(g!=0) _all_zeros=false;

        _sum_square_g+=g*g;
    } 
}

bool SubgradientMethod::next(vector<double>& lbda, LagrangeanFormulation& lf, solution_pair& p, solution_pair& d){
    if(_N>_max_N) return false;

    compute_gradient(lf,d);

    _factor = 1.0;
    if(lf.objective_type()==MAX_TYPE){
        _T= ( _factor*_pi*(1.05*d.vx-p.vx) )/_sum_square_g;    
    }else{
        _T= ( _factor*_pi*(1.05*p.vx-d.vx) )/_sum_square_g;    
    }

    int i;
    line_it it_r;
    for(i=0, it_r=lf.dual_begin();it_r!=lf.dual_end();it_r++,i++){
        lbda[i] = lbda[i]+_T*_G[i];    
        
        if(lbda[i]<=0){
            lbda[i]=0;
        }
    }  

    if(_DEBUG) log(lbda);
    return true;
}

void SubgradientMethod::improvement_check(LagrangeanFormulation& lf, solution_pair& d_prime, solution_pair& d){

    if( !( (lf.objective_type()==MAX_TYPE && d_prime.vx<d.vx) xor 
           (lf.objective_type()==MIN_TYPE && d_prime.vx>d.vx) ) ){                
            _no_improvement+=1;
    }    

    if(_no_improvement>=5){
        _no_improvement = 0;
        _pi = _pi/2.0;
        _N+=1;        
    }
}

bool SubgradientMethod::after_check(solution_pair& p, solution_pair& d){
    _num_it+=1;

    if(_all_zeros){
        // throw EX_ALL_GRADIENTS_ZERO();
        return false;
    }            
    
    if( ( fabs( (d.vx-p.vx)/p.vx) ) <= 0.01 ){
        return false;
    }       

    return true;    
}

void SubgradientMethod::log(vector<double>& lbda){

    printf("SUBGRADIENT ITERATION: %d\n",_num_it);
    printf("PI: %.12lf (%d)\n",_pi,_N);
    print_vector("GRADIENT",_G);
    printf("G SUM SQUARE: %.4lf\n", _sum_square_g);
    printf("STEP SIZE: %.12lf\n", _T);
    print_vector("LBDA", lbda);
    printf("NO IMPROVEMENT: %d\n",_no_improvement);

    printf("\n");
}