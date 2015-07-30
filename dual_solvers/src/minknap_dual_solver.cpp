#include "minknap_dual_solver.h"

MinknapDualSolver::MinknapDualSolver(Formulation& p_f, bool p_debug): 
                                    DualLagrangeanMethod(p_f.c().size(), p_f.objective_type()),
                                    _debug(p_debug){
                                        
    _primal_knapsack_restriction = get_knapsack_constraint(p_f);      

    _f = Formulation(p_f);
    _lf = LagrangeanFormulation(_f);
    
    _primal_kc = compute_benefit_cost(*_primal_knapsack_restriction,_f.c());
    _lagrangean_kc = compute_benefit_cost(*_primal_knapsack_restriction,_lf.lagrangean_costs());    
}

std::pair<Solution,Solution> MinknapDualSolver::solve(int p_max_N, double p_pi_factor, 
                         double p_max_no_improvement, bool p_use_lagrangean_costs){

    find_primal_solution();    
    find_dual_solution();

    if(_debug) log_start(_f,_lf,_primal,_dual);    

    solve_lagrangean_subproblem(_f,_lf,p_max_N, p_pi_factor, p_max_no_improvement, p_use_lagrangean_costs);    

    return std::make_pair(_primal,_dual);    
}

ConstraintLine* MinknapDualSolver::get_knapsack_constraint(Formulation& f){
    ConstraintLine* backpack_restriction = new ConstraintLine();

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


        backpack_restriction->add(backpack_member);
    }

    for(line_it it_r=f.begin();it_r!=f.end();it_r++){
        sum_bj+=(*it_r).second->rhs();
    }
    backpack_restriction->rhs(sum_bj);
    backpack_restriction->op(LESSER_EQUAL);

    return backpack_restriction;
}

void MinknapDualSolver::find_primal_solution(){
    find_primal_int_solution_by_benefit_cost_heuristic(_f, _primal_kc,_primal);
    _primal.vx( _f.compute(_primal.x()) );
}

void MinknapDualSolver::update_primal(bool p_use_lagrangean_costs){
    if(p_use_lagrangean_costs){
        _lagrangean_kc = compute_benefit_cost(*_primal_knapsack_restriction,_lf.lagrangean_costs());
        find_primal_int_feasible_solution_from_dual(_dual,_f,_lagrangean_kc,_primal);
    }else{
        find_primal_int_feasible_solution_from_dual(_dual,_f,_primal_kc,_primal);
    }

    _primal.vx( _f.compute(_primal.x()) );        
}

int MinknapDualSolver::active_constraints_for_vars(vector<int>& Ix){
    /*
        It returns how many constraints uses the variables on Ix
        Ix is a vector of indexes of variables x. The indexes are supposed to be in order
    */

    map<int,char> marker;
    unordered_set<int> constr_indexes;
    int k=0;

    for(int i=0;i<Ix.size();i++){

        constr_indexes = _lf.constr_var_appears( Ix[i] );

        for(unordered_set<int>::iterator it=constr_indexes.begin();it!=constr_indexes.end();it++){

            if( marker.find( *it ) == marker.end() ){
                marker[ *it ] = 1;
                k+=1; 
            }
        }
    }

    return k;
}

void MinknapDualSolver::find_dual_solution(){    
    int lift_factor = 1000;
    long value;

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
                _dual.set_component(i,0);
            }else{
                _dual.set_component(i, (double) x_line[j++]);
            }
        }                 


        // if(d_knap.x.size()!=0)
        //     printf("VALUE KNAP: %.2f\n",_lf.compute(d_knap.x));

        // printf("VALUE MINKNAP: %.2f\n",_lf.compute(d.x));        
        _dual.vx( _lf.compute(_dual.x()) );    

        delete[] costs;
        delete[] weights;
        delete[] x_line;

    }else{       
        find_int_optimal_solution_lagrangean_subproblem(_lf,_dual);
    }
}

void MinknapDualSolver::solve_lagrangean_subproblem(Formulation& f, LagrangeanFormulation& lf, int p_max_N, 
                                double p_pi_factor, double p_max_no_improvement, bool p_use_lagrangean_costs){


    SubgradientMethod sm(lf,p_max_N,p_pi_factor,p_max_no_improvement,_debug);
    PoolClique pool(lf);
    // bool still_extending = true;
    printf("RESTRICOES (INICIO): %d\n",_lf.num_constraints());    
    while( sm.next(lf,_primal,_dual) ){
        find_dual_solution();        
        sm.improvement_check(lf,_dual);

        update_primal(p_use_lagrangean_costs);   

        // do{
        //     still_extending = pool.extend_pool();
        // }while( !pool.is_pool_updated() && still_extending);        

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
    printf("NUM_IT: %d\nBEST DUAL BOUND:%.4lf\n",sm.iterations(),sm.best_bound());

    return;
}