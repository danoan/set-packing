#include "simple_dual_solver.h"

SimpleDualSolver::SimpleDualSolver(Formulation& p_f, LagrangeanFormulation& p_lf):_f(p_f),_lf(p_lf){
    _primal_backpack_restriction = get_backpack_restriction(p_f);    

    _primal_bc = compute_benefit_cost(_primal_backpack_restriction,p_f.c());
    _lagrangean_bc = compute_benefit_cost(_primal_backpack_restriction,p_lf.lagrangean_costs());
}

restriction_line SimpleDualSolver::get_backpack_restriction(Formulation& f){
    restriction_line backpack_restriction;
    backpack_restriction.members.resize(f.c().size());

    double sum_aj, sum_bj;
    sum_bj = 0;

    restriction_line_member backpack_member;
    for(int j=0;j<f.c().size();j++){
        sum_aj=0;
        for(line_it it_r=f.restrictions_begin();it_r!=f.restrictions_end();it_r++){
            restriction_line rl = (*it_r);

            for(member_it it_m=rl.members.begin();it_m!=rl.members.end();it_m++){
                restriction_line_member rlm = (*it_m);
                if(rlm.index==j){
                    sum_aj+= rlm.cost;
                }
            }
        }
        backpack_member.index = j;
        backpack_member.cost = sum_aj;

        backpack_restriction.members[j] = backpack_member;
    }

    for(line_it it_r=f.restrictions_begin();it_r!=f.restrictions_end();it_r++){
        sum_bj+=(*it_r).rhs;
    }
    backpack_restriction.rhs = sum_bj;
    backpack_restriction.op = LESSER_EQUAL;

    return backpack_restriction;
}

dual_lagrangean_solution SimpleDualSolver::solve(int max_N){
    vector<double> lbda;
    for(int i=0;i<_f.num_restrictions();i++){
        lbda.push_back(1);
    }    

    solution_pair p = find_primal_solution();    
    solution_pair d = find_dual_solution(lbda);

    if(DEBUG) log_start(_f,_lf,lbda,p,d);    

    solve_lagrangean_subproblem(_f,_lf,p,d,lbda,max_N);    

    dual_lagrangean_solution s;
    s.p = p;
    s.d = d;
    s.lbda = lbda;

    return s;
}


solution_pair SimpleDualSolver::find_primal_solution(){
    solution_pair s = find_int_solution_by_benefit_cost_heuristic(_f, _primal_bc);
    s.vx = _f.compute(s.x);

    return s;
}

solution_pair SimpleDualSolver::update_primal(solution_pair& p, solution_pair& d){
    solution_pair s = find_int_feasible_solution_from_dual(d,_f,_primal_bc);
    s.vx = _f.compute(s.x);        

    if(_f.objective_type()==MAX_TYPE){
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

solution_pair SimpleDualSolver::find_dual_solution(vector<double>& lbda){
    _lf.lbda(lbda);
    return find_int_solution_by_lagrangean_heuristic(_lf);
}


vector<double> SimpleDualSolver::compute_gradient(bool& all_zeros,double& sum_square_g,LagrangeanFormulation& lf, solution_pair& d){
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

solution_pair SimpleDualSolver::solve_lagrangean_subproblem(Formulation& f, LagrangeanFormulation& lf, solution_pair& p, solution_pair& d, vector<double>& lbda, int max_N=30){
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

            factor = 1.0;
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
            d_prime = find_dual_solution(lbda);
            
            if( !( (lf.objective_type()==MAX_TYPE && d_prime.vx<d.vx) xor 
                   (lf.objective_type()==MIN_TYPE && d_prime.vx>d.vx) ) ){                
                    no_improvement+=1;
            }

            d = d_prime;
            p = update_primal(p,d);     

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

void log_subgradient(int num_it, double pi, int N, vector<double>& G, double sum_square_g, 
                     double T, vector<double>& lbda, Formulation& f, LagrangeanFormulation& lf,
                     solution_pair& d, solution_pair& p, int no_improvement){

    printf("SUBGRADIENT ITERATION: %d\n",num_it);
    printf("PI: %.12lf (%d)\n",pi,N);
    print_vector("GRADIENT",G);
    printf("G SUM SQUARE: %.4lf\n", sum_square_g);
    printf("STEP SIZE: %.12lf\n", T);
    print_vector("LBDA", lbda);
    print_solution("PRIMAL",p);
    print_vector("LAGRANGEAN COSTS",lf.lagrangean_costs(),false);
    print_solution("DUAL",d);
    printf("NO IMPROVEMENT: %d\n",no_improvement);

    printf("\n");
}