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

    find_primal_solution();    
    find_dual_solution();

    // printf("RESTRICOES (INICIO): %d\n",_lf.num_constraints());

    if(_debug) log_start(_f,_lf,_primal,_dual);    

    solve_lagrangean_subproblem(_f,_lf,p_max_N, p_pi_factor,p_max_no_improvement,p_use_lagrangean_costs);    

    printf("RESTRICOES:%d %d\n",_f.num_constraints(),_lf.num_constraints());
    printf("%s\n",_lf.to_str().c_str());

    return std::make_pair(_primal,_dual);
}


void SimpleDualSolver::find_primal_solution(){
    find_primal_int_solution_by_benefit_cost_heuristic(_f, _primal_bc, _primal);
    _primal.vx( _f.compute(_primal.x()) );
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

void SimpleDualSolver::find_dual_solution(){
    find_int_optimal_solution_lagrangean_subproblem(_lf,_dual);
}

void SimpleDualSolver::solve_lagrangean_subproblem(Formulation& f, LagrangeanFormulation& lf, 
              int p_max_N, double p_pi_factor, double p_max_no_improvement, bool p_use_lagrangean_costs){
    
    SubgradientMethod sm(lf,p_max_N,p_pi_factor,p_max_no_improvement,_debug);

    // bool still_extending=true;
    PoolClique pool(lf);
    while( sm.next(lf,_primal,_dual) ){
        find_dual_solution();
        sm.improvement_check(lf,_dual);

        update_primal(p_use_lagrangean_costs);   

        //FIXING VARIABLES: Fixo na solucao primal e na solucao dual

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
    printf("REPLACED: %d\n",pool.replaced_constraints());

    return;
}