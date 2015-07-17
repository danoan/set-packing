#include "simple_dual_solver.h"

SimpleDualSolver::SimpleDualSolver(Formulation& p_f, bool p_debug):_f(p_f),_debug(p_debug){
    _lf = LagrangeanFormulation(_f);
    _primal_backpack_restriction = get_backpack_restriction(p_f);    

    _primal_bc = compute_benefit_cost(_primal_backpack_restriction,p_f.c());
    _lagrangean_bc = compute_benefit_cost(_primal_backpack_restriction,_lf.lagrangean_costs());
}

ConstraintLine SimpleDualSolver::get_backpack_restriction(Formulation& f){
    ConstraintLine backpack_restriction;

    double sum_aj, sum_bj;
    sum_bj = 0;

    ConstraintMember backpack_member;
    for(int j=0;j<f.c().size();j++){
        sum_aj=0;
        for(line_it it_r=f.begin();it_r!=f.end();it_r++){
            ConstraintLine* rl = (*it_r).second;

            for(member_it it_m=rl->begin();it_m!=rl->end();it_m++){
                ConstraintMember rm = (*it_m);
                if(rm.index==j){
                    sum_aj+= rm.cost;
                }
            }
        }
        backpack_member.index = j;
        backpack_member.cost = sum_aj;

        backpack_restriction.add( backpack_member );
    }

    for(line_it it_r=f.begin();it_r!=f.end();it_r++){
        sum_bj+= (*it_r).second->rhs();
    }

    backpack_restriction.rhs(sum_bj);
    backpack_restriction.op(LESSER_EQUAL);

    return backpack_restriction;
}

dual_lagrangean_solution SimpleDualSolver::solve(int p_max_N, double p_pi_factor, double p_max_no_improvement,
                         bool p_use_lagrangean_costs){
    vector<double> lbda;
    for(int i=0;i<_f.num_constraints();i++){
        lbda.push_back(1);
    }    

    solution_pair p = find_primal_solution();    
    solution_pair d = find_dual_solution(lbda);

    if(_debug) log_start(_f,_lf,lbda,p,d);    

    solve_lagrangean_subproblem(_f,_lf,p,d,lbda,p_max_N, p_pi_factor,p_max_no_improvement,p_use_lagrangean_costs);    

    dual_lagrangean_solution s;
    s.p = p;
    s.d = d;
    s.lbda = lbda;

    return s;
}


solution_pair SimpleDualSolver::find_primal_solution(){
    solution_pair s = find_primal_int_solution_by_benefit_cost_heuristic(_f, _primal_bc);
    s.vx = _f.compute(s.x);

    return s;
}

solution_pair SimpleDualSolver::update_primal(solution_pair& p, solution_pair& d, bool p_use_lagrangean_costs){
    solution_pair s;

    if(p_use_lagrangean_costs){
        _lagrangean_bc = compute_benefit_cost(_primal_backpack_restriction,_lf.lagrangean_costs());
        s = find_primal_int_feasible_solution_from_dual(d,_f,_lagrangean_bc);
    }else{
        s = find_primal_int_feasible_solution_from_dual(d,_f,_primal_bc);
    }
    
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
    return find_int_optimal_solution_lagrangean_subproblem(_lf);
}

solution_pair SimpleDualSolver::solve_lagrangean_subproblem(Formulation& f, LagrangeanFormulation& lf, 
              solution_pair& p, solution_pair& d, vector<double>& lbda, int p_max_N, 
              double p_pi_factor, double p_max_no_improvement, bool p_use_lagrangean_costs){
    solution_pair d_prime;   
    SubgradientMethod sm(lf,p_max_N,p_pi_factor,p_max_no_improvement,_debug);

    bool still_extending=true;
    PoolClique pool(lf);
    while( sm.next(lbda,lf,p,d) ){
        d_prime = find_dual_solution(lbda);
        sm.improvement_check(lf,d,d_prime);

        d = d_prime;
        p = update_primal(p,d,p_use_lagrangean_costs);   

        do{
            still_extending = pool.extend_pool();
        }while( !pool.is_pool_updated() && still_extending );


        if( sm.after_check(p,d)==false ){
            return d;
        }
    }

    return d;
}