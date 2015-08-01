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

    find_primal_solution(_primal);    
    find_dual_solution(_dual);

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

void MinknapDualSolver::find_primal_solution(Solution& p){
    find_primal_int_solution_by_benefit_cost_heuristic(_f, _primal_kc,p);
    p.vx( _f.compute(p.x()) );
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

int MinknapDualSolver::active_constraints_for_vars(vector<int>& Ix, const int& fixed_variable=-1, const int& fixed_value=-1){
    /*
        It returns how many constraints uses the variables on Ix
        Ix is a vector of indexes of variables x. The indexes are supposed to be in order
    */

    map<int,char> marker;
    unordered_set<int> constr_indexes;
    int k=0;

    for(int i=0;i<Ix.size();i++){
        if(i==fixed_variable && fixed_value==1) continue;

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

void MinknapDualSolver::find_dual_solution(Solution& d, const int& fixed_variable, const int& fixed_value){    
    int lift_factor = 1000;
    long value;

    vector<int> Ix;
    int fv_intern_index = -1;
    bool flag_fixed = false;
    //Store the variables indexes that has non-negative lagrangean costs
    for(int i=0;i<_lf.lagrangean_costs().size();i++){
        
        if( _lf.lagrangean_costs()[i]>=0 ){
            Ix.push_back(i);
            if(i==fixed_variable){
                fv_intern_index = Ix.size()-1;                
                flag_fixed = true;
            }
        }            

    }


    //If there are more than 0 of such variables, prepare the correspondent knapsack constraint
    if( Ix.size()>0 ){
        int m = Ix.size();      //Number of Variables of Pisinger's Formulation
        int n = active_constraints_for_vars(Ix,fv_intern_index,fixed_value);    //Right Side of Knapsack Constraint

        if(fv_intern_index>-1 && flag_fixed){
            m-=1;   //One variable less in Pisinger's Formulation
        }
       
        long* costs = new long int[m];  //Objective Function Coefficients
        int costs_index=0;
        int i=0;
        while(i<Ix.size()){
            if(i==fv_intern_index && flag_fixed){i++; continue;}

            costs[costs_index++] = (long) ceil(_lf.lagrangean_costs()[ Ix[i] ]*lift_factor);
            if(costs[costs_index-1]<0){ printf("ERROR - %d - %lu\n",i,costs[costs_index-1]); }
            i++;
        }
        
        int* weights = new int[m];   //Knapsack Constraints Coefficients
        int sum_w=0;
        int weights_index=0;
        i=0;
        while(i<Ix.size()){
            if(i==fv_intern_index && flag_fixed){ 
                i++;                     
                continue;                    
            }
            
            weights[weights_index] = _lf.times_var_appears(Ix[i]);
            sum_w+=weights[weights_index++];
            i++;
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
            for(int i=0;i<m;i++){
                x_line[i]=0;
            }                

            minknap(m,costs,weights,x_line,n);                           
        }

        j=0;
        int c=0;
        for(int i=0;i<_lf.lagrangean_costs().size();i++){            
            if(Ix[j]==fixed_variable && flag_fixed){
                d.set_component(fixed_variable,fixed_value);
                j++;
                continue;
            }

            if(c>=m or Ix[j]!=i){
                d.set_component(i,0);   //Here are the variables not included in Pisinger's Formulation due to negative Lagrangean Cost
            }else{
                d.set_component(i, (double) x_line[c++]);
                j++;
            }
        }


        // printf("COSTS %d-%d\n",m,costs_index);                 
        // for(int i=0;i<m;i++){
        //     printf("%lu ", costs[i]);
        // }printf("\n");

        // printf("WEIGHTS %d-%d-%lu\n",m,weights_index,Ix.size());                                 
        // for(int i=0;i<m;i++){
        //     printf("%d ", weights[i]);
        // }printf("\n");      
    
        d.vx( _lf.compute(d.x()) );    

        // printf("XLINE\n");                 
        // for(int i=0;i<m;i++){
        //     printf("%d ", x_line[i]);
        // }printf("*** %.4lf ***\n", d.vx());        
        // print_vector("DUAL_SOL",d.x());

        delete[] costs;
        delete[] weights;
        delete[] x_line;

    }else{       
        find_int_optimal_solution_lagrangean_subproblem(_lf,d,fixed_variable,fixed_value);
    }
}

void MinknapDualSolver::fixing(){
    //FIXING x=0
    Solution fixed_sol = _dual;
    for(int i=0;i<_dual.num_components();i++){
        if(_dual.x(i)==0 && !(_dual.is_fixed(i))){
            find_dual_solution(fixed_sol,i,1);
            // printf("%.4lf  -  %.4lf\n", fixed_sol.vx(), _primal.vx() );
            if( fixed_sol.vx() < _primal.best_value() ){
                _dual.fix(i,0);
                _primal.fix(i,0);
                // printf("VAR %d FIXED TO %d\n",i,0);
            }
        }
    }

    //FIXING x=1
    for(int i=0;i<_dual.num_components();i++){
        if(_dual.x(i)==1  && !(_dual.is_fixed(i)) ){
            find_dual_solution(fixed_sol,i,0);
            if( fixed_sol.vx() < _primal.best_value() ){
                _dual.fix(i,1);
                _primal.fix(i,1);
                // printf("VAR %d FIXED TO %d\n",i,1);
            }
        }
    } 
}

void MinknapDualSolver::solve_lagrangean_subproblem(Formulation& f, LagrangeanFormulation& lf, int p_max_N, 
                                double p_pi_factor, double p_max_no_improvement, bool p_use_lagrangean_costs){


    SubgradientMethod sm(lf,p_max_N,p_pi_factor,p_max_no_improvement,_debug);
    PoolClique pool(lf);
    // bool still_extending = true;
    printf("RESTRICOES (INICIO): %d\n",_lf.num_constraints());    
    while( sm.next(lf,_primal,_dual) ){
        find_dual_solution(_dual);        
        sm.improvement_check(lf,_dual);

        update_primal(p_use_lagrangean_costs);   

        fixing();

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
    printf("NUM ITERATIONS: %d\n",sm.iterations());

    return;
}