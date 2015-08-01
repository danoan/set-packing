#include "simple_dual_solver.h"

SimpleDualSolver::SimpleDualSolver(Formulation& p_f, bool p_debug):
                                DualLagrangeanMethod( p_f.c().size(), p_f.objective_type() ),_f(p_f),
                                _debug(p_debug){

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

std::pair<Solution,Solution> SimpleDualSolver::solve(int p_max_N, double p_pi_factor, double p_max_no_improvement,
                         bool p_use_lagrangean_costs){

    find_primal_solution(_primal);    
    find_dual_solution(_dual);

    // printf("RESTRICOES (INICIO): %d\n",_lf.num_constraints());

    if(_debug) log_start(_f,_lf,_primal,_dual);    

    solve_lagrangean_subproblem(_f,_lf,p_max_N, p_pi_factor,p_max_no_improvement,p_use_lagrangean_costs);    

    printf("RESTRICOES:%d %d\n",_f.num_constraints(),_lf.num_constraints());
    // printf("%s\n",_lf.to_str().c_str());

    return std::make_pair(_primal,_dual);
}


void SimpleDualSolver::find_primal_solution(Solution& p){
    find_primal_int_solution_by_benefit_cost_heuristic(_f, _primal_bc, p);
    p.vx( _f.compute(p.x()) );
}

void SimpleDualSolver::update_primal(bool p_use_lagrangean_costs){
    if(p_use_lagrangean_costs){
        _lagrangean_bc = compute_benefit_cost(_primal_backpack_restriction,_lf.lagrangean_costs());
        find_primal_int_feasible_solution_from_dual(_dual,_f,_lagrangean_bc,_primal);
    }else{
        find_primal_int_feasible_solution_from_dual(_dual,_f,_primal_bc,_primal);
    }
    
    _primal.vx( _f.compute(_primal.x()) );        

}

void SimpleDualSolver::find_dual_solution(Solution& d){
    find_int_optimal_solution_lagrangean_subproblem(_lf,d);
}

void SimpleDualSolver::fixing(){
    //FIXING x=0
    Solution fixed_sol = _dual;
    for(int i=0;i<_dual.num_components();i++){
        if(_dual.x(i)==0  && !(_dual.is_fixed(i)) ){
            find_int_optimal_solution_lagrangean_subproblem(_lf,fixed_sol,i,1);
            // printf("%.4lf  -  %.4lf\n", fixed_sol.vx(), _primal.vx() );
            if( fixed_sol.vx() <_primal.best_value() ){
                _dual.fix(i,0);
                _primal.fix(i,0);
                // printf("VAR %d FIXED TO %d\n",i,0);
            }
        }
    }

    //FIXING x=1
    for(int i=0;i<_dual.num_components();i++){
        if(_dual.x(i)==1  && !(_dual.is_fixed(i)) ){
            find_int_optimal_solution_lagrangean_subproblem(_lf,fixed_sol,i,0);
            if( fixed_sol.vx() < _primal.best_value() ){
                _dual.fix(i,1);
                _primal.fix(i,1);
                // printf("VAR %d FIXED TO %d\n",i,1);
            }
        }
    }    
}

void SimpleDualSolver::solve_lagrangean_subproblem(Formulation& f, LagrangeanFormulation& lf, 
              int p_max_N, double p_pi_factor, double p_max_no_improvement, bool p_use_lagrangean_costs){
    
    SubgradientMethod sm(lf,p_max_N,p_pi_factor,p_max_no_improvement,_debug);

    // bool still_extending=true;
    PoolClique pool(lf);
    printf("RESTRICOES (INICIO): %d\n",_lf.num_constraints());    
    while( sm.next(lf,_primal,_dual) ){
        find_dual_solution(_dual);
        sm.improvement_check(lf,_dual);

        update_primal(p_use_lagrangean_costs);   

        fixing();

        // do{
        //     still_extending = pool.extend_pool();
        // }while( !pool.is_pool_updated() && still_extending );

        pool.extend_pool(_dual);

        if(_debug){
            printf("%s\n",sm.log().c_str());

            print_vector("LAGRANGEAN COSTS", lf.lagrangean_costs());
            print_solution("PRIMAL",_primal);
            print_solution("DUAL",_dual);
        }

        if( sm.after_check(_primal,_dual)==false ){
            break;
        }
    }
    printf("RESTRICOES (FINAL): %d\t REPLACED:%d\n",_lf.num_constraints(),pool.replaced_constraints());
    printf("NUM ITERATIONS: %d\n",sm.iterations());

    return;
}