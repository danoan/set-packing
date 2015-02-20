#include "pure_lagrangean_solver.h"

subgradient_solution PureLagrangeanSolver::solve(int max_N){
    _f.set_backpack_restriction();

    vector<double> lbda;
    for(int i=0;i<_f.num_restrictions();i++){
        lbda.push_back(1);
    }    

    solution_pair p = find_primal_solution(_f);    
    solution_pair d = find_dual_solution(_lf,lbda);

    if(DEBUG) log_start(_f,_lf,lbda,p,d);    

    subgradient(_f,_lf,p,d,lbda,max_N);    

    subgradient_solution s;
    s.p = p;
    s.d = d;
    s.lbda = lbda;

    return s;
}


solution_pair PureLagrangeanSolver::find_primal_solution(Formulation& f){
    solution_pair s = find_int_solution_by_benefit_cost_heuristic(f);
    s.vx = f.compute(s.x);

    return s;
}

solution_pair PureLagrangeanSolver::update_primal(solution_pair& p, solution_pair& d, Formulation& f,LagrangeanFormulation& lf){
    // f.set_backpack_restriction( lf.lagrangean_costs() );
    solution_pair s = find_int_feasible_solution_from_dual(d,f);
    s.vx = f.compute(s.x);        

    if(f.objective_type()==MAX_TYPE){
        if(p.vx>=s.vx){
            s = p;
        }
    }else{
        if(p.vx<=s.vx){
            s = p;
        }
    }
    

    return s;
}

solution_pair PureLagrangeanSolver::find_dual_solution(LagrangeanFormulation& lf, vector<double>& lbda){
    lf.lbda(lbda);
    return find_int_solution_by_lagrangean_heuristic(lf);
}


vector<double> PureLagrangeanSolver::compute_gradient(bool& all_zeros,double& sum_square_g,LagrangeanFormulation& lf, solution_pair& d){
    all_zeros = true;
    double g;
    double sum_a;

    vector<double> G;

    sum_square_g = 0;
    int i;
    line_it it_r;
    for(i=0,it_r=lf.restrictions_begin();it_r!=lf.restrictions_end();it_r++,i++){
        restriction_line rl = (*it_r);
        sum_a=0;
        for(member_it it_m=rl.members.begin();it_m!=rl.members.end();it_m++){
            restriction_line_member rlm = (*it_m);
            sum_a+= rlm.cost*d.x[rlm.index];
        }
        g = rl.rhs - sum_a;

        if(lf.objective_type()==MAX_TYPE){
            G.push_back(-g);
        }else{
            G.push_back(g);
        }    

        if(g!=0) all_zeros=false;

        sum_square_g+=g*g;
    } 

    return G;
}

solution_pair PureLagrangeanSolver::subgradient(Formulation& f, LagrangeanFormulation& lf, solution_pair& p, solution_pair& d, vector<double>& lbda, int max_N=30){
    double pi = 2;
    int N=1;
    int no_improvement;

    vector<double> G;
    double sum_square_g;        
    bool all_zeros;

    solution_pair d_prime;   
    double T;
    int num_it=0;
    double factor;
    while(N<=max_N){
        no_improvement = 0;
        while(1){
            G = compute_gradient(all_zeros,sum_square_g,lf,d);

            factor = (rand()%10)/10.0;
            if(lf.objective_type()==MAX_TYPE){
                T= ( factor*pi*(1.05*d.vx-p.vx) )/sum_square_g;    
            }else{
                T= ( factor*pi*(1.05*p.vx-d.vx) )/sum_square_g;    
            }
            
            int i;
            line_it it_r;
            for(i=0, it_r=lf.restrictions_begin();it_r!=lf.restrictions_end();it_r++,i++){
                lbda[i] = lbda[i]+T*G[i];    
                
                if(lbda[i]<=0){
                    lbda[i]=0;
                }
            }
            d_prime = find_dual_solution(lf,lbda);
            
            if( !( (lf.objective_type()==MAX_TYPE && d_prime.vx<d.vx) xor 
                   (lf.objective_type()==MIN_TYPE && d_prime.vx>d.vx) ) ){                
                    no_improvement+=1;
            }

            d = d_prime;
            p = update_primal(p,d,f,lf);     

            if(DEBUG) log_subgradient(num_it,pi,N,G,sum_square_g,T,lbda,f,lf,d,p,no_improvement);

            num_it+=1;

            if(all_zeros){
                throw EX_ALL_GRADIENTS_ZERO();
            }            
            
            if( ( fabs( (d.vx-p.vx)/p.vx) ) <= 0.01 ){
                return d;
            } 

            if(no_improvement>=5) break;                        
        }        
        pi = pi/2.0;
        N+=1;
    }

    return d;

    throw EX_NO_LBDA_IMPROVE_SOLUTION();
}