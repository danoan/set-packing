#include "minknap_dual_solver.h"

MinknapDualSolver::MinknapDualSolver(Formulation& p_f){
    ConstraintLine* _primal_knapsack_restriction = get_knapsack_constraint(p_f);      

    _f = Formulation(p_f);
    _lf = LagrangeanFormulation(_f);
    
    _primal_kc = compute_benefit_cost(*_primal_knapsack_restriction,_f.c());
    _lagrangean_kc = compute_benefit_cost(*_primal_knapsack_restriction,_lf.lagrangean_costs());    
}

dual_lagrangean_solution MinknapDualSolver::solve(int max_N){
    vector<double> lbda;
    for(int i=0;i<_f.num_constraints();i++){
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
    solution_pair s = find_int_solution_by_benefit_cost_heuristic(_f, _primal_kc);
    s.vx = _f.compute(s.x);

    return s;
}

solution_pair MinknapDualSolver::update_primal(solution_pair& p, solution_pair& d){
    solution_pair s = find_int_feasible_solution_from_dual(d,_f,_primal_kc);
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
        double n = (double) active_constraints_for_vars(Ix);

        double* costs = (double*) malloc(sizeof(double)*m);
        for(int i=0;i<m;i++){
            costs[i] = _lf.lagrangean_costs()[ Ix[i] ];
        }

        
        double* weights = (double*) malloc(sizeof(double)*m);
        double sum_w=0;
        for(int i=0;i<m;i++){
            weights[i] = _lf.times_var_appears(Ix[i]);
            sum_w+=weights[i];
        }        

        solution_pair d2;                
        int* x_line = (int*) malloc(sizeof(int)*Ix.size());

        //If the sum of the weights are lesser than the kanpsack contraint threshold, the optimal solution is
        //to set all variables to 1
        if(sum_w<=n){
            for(int i=0;i<m;i++){
                x_line[i]=1;
            }
        }else{
            minknap(m,costs,weights,x_line,n); 
        }


        int j=0;
        for(int i=0;i<_lf.lagrangean_costs().size();i++){
            if(Ix[j]!=i){
                d.x.push_back(0.0);
            }else{
                d.x.push_back( (double) x_line[j++]);
            }
        }
        d.vx = _lf.compute(d.x);    

        free(costs);
        free(weights);
        free(x_line);

    }else{       
        d = find_int_solution_by_lagrangean_heuristic(_lf);
    }
    
    return d;
}

solution_pair MinknapDualSolver::solve_lagrangean_subproblem(Formulation& f, LagrangeanFormulation& lf, solution_pair& p,
                                  solution_pair& d, vector<double>& lbda, int max_N){
    solution_pair d_prime;   
    SubgradientMethod sm(lf,max_N,DEBUG);

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