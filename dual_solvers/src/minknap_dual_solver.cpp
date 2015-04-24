#include "minknap_dual_solver.h"

MinknapDualSolver::MinknapDualSolver(Formulation& p_f){
    RestrictionLine* _primal_backpack_restriction = get_backpack_restriction(p_f);      

    vector<RestrictionLine*> extra_primal;
    extra_primal.push_back(_primal_backpack_restriction);

    _f = Formulation(p_f);
    _lf = LagrangeanFormulation(_f, true, NULL, extra_primal, vector<RestrictionLine*>());
    // _lf = LagrangeanFormulation(_f);
    
    _primal_bc = compute_benefit_cost(*_primal_backpack_restriction,_f.c());
    _lagrangean_bc = compute_benefit_cost(*_primal_backpack_restriction,_lf.lagrangean_costs());    
}

dual_lagrangean_solution MinknapDualSolver::solve(int max_N){
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

RestrictionLine* MinknapDualSolver::get_backpack_restriction(Formulation& f){
    RestrictionLine* backpack_restriction = new RestrictionLine();
    _summed_coef_restrictions.resize(f.c().size());

    double sum_aj, sum_bj;
    sum_bj = 0;

    RestrictionMember backpack_member;
    for(int j=0;j<f.c().size();j++){
        sum_aj=0;
        for(line_it it_r=f.begin();it_r!=f.end();it_r++){
            RestrictionLine rl = *(*it_r);

            for(member_it it_m=rl.begin();it_m!=rl.end();it_m++){
                RestrictionMember rm = (*it_m);
                if(rm.index==j){
                    sum_aj+= rm.cost;
                }
            }
        }
        backpack_member.index = j;
        backpack_member.cost = sum_aj;

        _summed_coef_restrictions[j] = sum_aj;

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
    solution_pair s = find_int_solution_by_benefit_cost_heuristic(_f, _primal_bc);
    s.vx = _f.compute(s.x);

    return s;
}

solution_pair MinknapDualSolver::update_primal(solution_pair& p, solution_pair& d){
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

solution_pair MinknapDualSolver::find_dual_solution(vector<double>& lbda){
    _lf.lbda(lbda);    
    
    // printf("%d %d\n",_lf.lagrangean_costs().size(),_summed_coef_restrictions.size());

    double* costs = new double[_lf.lagrangean_costs().size()];
    double* weights = new double[_summed_coef_restrictions.size()];

    int m=_lf.lagrangean_costs().size();
    for(int i=0;i<m;i++){ 
        costs[i] = _lf.lagrangean_costs()[i] + 1.0; 
        weights[i] = _summed_coef_restrictions[i];
    }

    for(int i=0;i<m;i++){ 
        if(costs[i]<0){
            costs[i] = 0.0;
        }
    }

    int* x = new int[m];
    vector<double> xd;

    for(int i=0;i<_lf.lagrangean_costs().size();i++){
        xd.push_back(0);
    }    

    solution_pair d1 = find_int_solution_by_lagrangean_heuristic(_lf);
    solution_pair d2;    

    if(m!=0){
        int a = minknap(m, costs , weights, x, 500);        
        m=0;
        for(int i=0;i<_lf.lagrangean_costs().size();i++){
            xd[ i ] = x[i];    
        }
    }

    d2.x = xd;
    d2.vx = _lf.compute(d2.x);            

    //========
    //
    //d2 deveria retornar sempre um limite superior, mas ele está retornando limites inferiores também.
    //

    // printf("%lf - %lf\n",d1.vx,d2.vx);
    // print_vector("NORMAL",d1.x);
    // print_vector("PISINGER",d2.x);
    // print_vector("LC",_lf.lagrangean_costs());

    for(int i=0;i<_lf.lagrangean_costs().size();i++){
        if(d2.x[i]==1 && _lf.lagrangean_costs()[i]<0.0){
            printf("DEU RUIM %d: %lf\n",i,_lf.lagrangean_costs()[i]);
        }

        if(d1.x[i]==1 && _lf.lagrangean_costs()[i]<0.0){
            printf("DEU RUIM DUPLO %d: %lf\n",i,_lf.lagrangean_costs()[i]);
        }        
    }

    return d2;
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