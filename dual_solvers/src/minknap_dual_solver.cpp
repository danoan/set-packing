#include "minknap_dual_solver.h"

MinknapDualSolver::MinknapDualSolver(Formulation& p_f, bool p_debug):_debug(p_debug){
    ConstraintLine* _primal_knapsack_restriction = get_knapsack_constraint(p_f);      

    _f = Formulation(p_f);
    _lf = LagrangeanFormulation(_f);
    
    _primal_kc = compute_benefit_cost(*_primal_knapsack_restriction,_f.c());
    _lagrangean_kc = compute_benefit_cost(*_primal_knapsack_restriction,_lf.lagrangean_costs());    
}

dual_lagrangean_solution MinknapDualSolver::solve(int p_max_N, double p_pi_factor, double p_max_no_improvement){
    vector<double> lbda;
    for(int i=0;i<_f.num_constraints();i++){
        lbda.push_back(1);
    }    

    solution_pair p = find_primal_solution();    
    solution_pair d = find_dual_solution(lbda);

    if(_debug) log_start(_f,_lf,lbda,p,d);    

    solve_lagrangean_subproblem(_f,_lf,p,d,lbda,p_max_N, p_pi_factor, p_max_no_improvement);    

    dual_lagrangean_solution s;
    s.p = p;
    s.d = d;
    s.lbda = lbda;

    return s;    
}

ConstraintLine* MinknapDualSolver::get_knapsack_constraint(Formulation& f){
    ConstraintLine* backpack_restriction = new ConstraintLine();

    double sum_aj, sum_bj;
    sum_bj = 0;

    ConstraintMember backpack_member;
    for(int j=0;j<f.c().size();j++){
        sum_aj=0;
        for(line_it it_r=f.begin();it_r!=f.end();it_r++){
            ConstraintLine rl = *(*it_r);

            for(member_it it_m=rl.begin();it_m!=rl.end();it_m++){
                ConstraintMember rm = (*it_m);
                if(rm.index==j){
                    sum_aj+= rm.cost;
                }
            }
        }
        backpack_member.index = j;
        backpack_member.cost = sum_aj;


        backpack_restriction->add(backpack_member);
    }

    for(line_it it_r=f.begin();it_r!=f.end();it_r++){
        sum_bj+=(*it_r)->rhs();
    }
    backpack_restriction->rhs(sum_bj);
    backpack_restriction->op(LESSER_EQUAL);

    return backpack_restriction;
}

solution_pair MinknapDualSolver::find_primal_solution(){
    solution_pair s = find_primal_int_solution_by_benefit_cost_heuristic(_f, _primal_kc);
    s.vx = _f.compute(s.x);

    return s;
}

solution_pair MinknapDualSolver::update_primal(solution_pair& p, solution_pair& d){
    solution_pair s = find_primal_int_feasible_solution_from_dual(d,_f,_primal_kc);
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

int MinknapDualSolver::active_constraints_for_vars(vector<int>& Ix){
    /*
        It returns how many constraints uses the variables on Ix
        Ix is a vector of indexes of variables x. The indexes are supposed to be in order
    */

    map<int,char> marker;
    vector<int> constr_indexes;
    int k=0;

    for(int i=0;i<Ix.size();i++){

        constr_indexes = _lf.constr_var_appears( Ix[i] );

        for(int j=0;j<constr_indexes.size();j++){
            if( marker.find( constr_indexes[j] ) == marker.end() ){
                marker[constr_indexes[j]] = 1;
                k+=1; 
            }
        }
    }

    return k;
}

solution_pair MinknapDualSolver::find_dual_solution(vector<double>& lbda){
    _lf.lbda(lbda);    
    
    int lift_factor = 1000;
    long value;
    solution_pair d;
    vector<int> Ix;

    //Store the variables indexes that has non-negative lagrangean costs
    for(int i=0;i<_lf.lagrangean_costs().size();i++){
        if( _lf.lagrangean_costs()[i]>=0 ){
            Ix.push_back(i);
            // printf("LC: %lf\n",_lf.lagrangean_costs()[i]);
        }
    }

    //If there are more than 0 of such variables, prepare the correspondent knapsack constraint
    if( Ix.size()>0 ){
        int m = Ix.size();
        int n = active_constraints_for_vars(Ix);

        
        long* costs = new long int[m];
        for(int i=0;i<m;i++){
            costs[i] = (long) ceil(_lf.lagrangean_costs()[ Ix[i] ]*lift_factor);
        }

        
        int* weights = new int[m];   
        int sum_w=0;
        for(int i=0;i<m;i++){
            weights[i] = _lf.times_var_appears(Ix[i]);
            sum_w+=weights[i];
        }        

        // solution_pair d_knap;                
        int* x_line = new int[m];

        //If the sum of the weights are lesser than the kanpsack contraint threshold, the optimal solution is
        //to set all variables to 1
        int j;
        if(sum_w<=n){
            for(int i=0;i<m;i++){
                x_line[i]=1;
            }
        }else{
            
            // for(int i=0;i<m;i++){
            //     x_line[i]=0;
            // }            
            
            // value = knapsack(costs,weights,n,m,1,x_line);

            // j=0;
            // for(int i=0;i<_lf.lagrangean_costs().size();i++){            
            //     if(j>=m or Ix[j]!=i){
            //         d_knap.x.push_back(0.0);
            //     }else{
            //         d_knap.x.push_back( (double) x_line[j++]);
            //     }
            // }   


            for(int i=0;i<m;i++){
                x_line[i]=0;
            }            

            minknap(m,costs,weights,x_line,n);                           
        }

        j=0;
        for(int i=0;i<_lf.lagrangean_costs().size();i++){            
            if(j>=m or Ix[j]!=i){
                d.x.push_back(0.0);
            }else{
                d.x.push_back( (double) x_line[j++]);
            }
        }                 


        // if(d_knap.x.size()!=0)
        //     printf("VALUE KNAP: %.2f\n",_lf.compute(d_knap.x));

        // printf("VALUE MINKNAP: %.2f\n",_lf.compute(d.x));        
        d.vx = _lf.compute(d.x);    

        delete[] costs;
        delete[] weights;
        delete[] x_line;

    }else{       
        d = find_int_optimal_solution_lagrangean_subproblem(_lf);
    }
    
    return d;
}

solution_pair MinknapDualSolver::solve_lagrangean_subproblem(Formulation& f, LagrangeanFormulation& lf, solution_pair& p,
                                  solution_pair& d, vector<double>& lbda, int p_max_N, double p_pi_factor, double p_max_no_improvement){
    solution_pair d_prime;   
    SubgradientMethod sm(lf,p_max_N,p_pi_factor,p_max_no_improvement,_debug);

    while( sm.next(lbda,lf,p,d) ){
        d_prime = find_dual_solution(lbda);
        sm.improvement_check(lf,d,d_prime);

        d = d_prime;
        p = update_primal(p,d);   

        if( sm.after_check(p,d)==false ){
            return d;
        }
    }

    return d;
}