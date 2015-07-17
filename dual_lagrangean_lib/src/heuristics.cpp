#include "heuristics.h"

bool compare_benefit_cost(benefit_cost_pair a, benefit_cost_pair b){
    //Decrescent order
    return a.value > b.value;
}

/*
    Classes' atributes are defined such that the attribute cost_vector refers to the objective function 
    coeficients and the attribute cost of a RestrictionMember object is the constraint coefficient. In the 
    Set Packing context, the cost when we say benefit cost is the cardinality of each subset x[j] 
    (constraint coeffient), while the benefit is the profit (objective function coefficient). That's why 
    the equation to compute benefit cost is: ( (cost_vector[rm.index]/rm.cost) )
*/
vector<benefit_cost_pair> compute_benefit_cost(ConstraintLine& br, vector<double>& cost_vector){
    vector<benefit_cost_pair> benefit_cost;

    for(member_it it_m=br.begin();it_m!=br.end();it_m++){
        benefit_cost_pair bcp;
        ConstraintMember rm = (*it_m);

        bcp.value = ( (cost_vector[rm.index]/rm.cost) );
        bcp.index = rm.index;

        benefit_cost.push_back(bcp);
    }

    sort(benefit_cost.begin(),benefit_cost.end(),compare_benefit_cost);
    return benefit_cost;    
}

/*
    This is a simple method to find a feasible solution for the Set Packing primal problem. It simples 
    order the variables x[j] by benefit cost value and sets x[j]=1. If after x[j]=1 some constraint is 
    violated, x[j] is again set to 0.
*/
solution_pair find_primal_int_solution_by_benefit_cost_heuristic(Formulation& f, vector<benefit_cost_pair>& bc_vector){
    solution_pair s;
    for(int j=0;j<f.c().size();j++){
        s.x.push_back(0);
    }

    benefit_cost_pair bcp;
    for(int k=0;k<bc_vector.size();k++){
         bcp =  bc_vector[k];
         if(bcp.value<0){continue;}
         s.x[bcp.index] = 1;
         if(f.check_constraints(s.x)==false){
            s.x[bcp.index] = 0;
         }
    }

    return s;
}


/*
    After the lagrangean subproblem is solved, a dual solution Dx for a certain u is known. This method
    uses the solution Dx to come up with a new feasible primal solution Px. In order to do that, it uses
    the Complementary Slackness Theorem. Considering the Set Packing Problem, this theorem ensures that
    if Dx[j]=0 then for sure Px[j]=0. In other words, only when Dx[j]=1 Px[j] could assume 1 as a value.
*/
solution_pair find_primal_int_feasible_solution_from_dual(solution_pair& d, Formulation& f, vector<benefit_cost_pair>& bc_vector){
    solution_pair s;
    for(int j=0;j<f.c().size();j++){
        s.x.push_back(0);
    }

    benefit_cost_pair bcp;
    //Set the coefficients of the new solution as the same as the dual, but in order of benefit cost
    for(int k=0;k<bc_vector.size();k++){
        bcp =  bc_vector[k];
        if(d.x[bcp.index]==1){
            s.x[bcp.index]=1;

            if(f.check_constraints(s.x)==false){
                s.x[bcp.index] = 0;
            }                    
        }
    }
    
    for(int k=0;k<bc_vector.size();k++){
         bcp =  bc_vector[k];
         if(s.x[bcp.index]==1){continue;} //It`s already set from the previous loop
         if(bcp.value<0){continue;}

         s.x[bcp.index] = 1;
         if(f.check_constraints(s.x)==false){
            s.x[bcp.index] = 0;
         }
    }

    return s;
}

/*
    It solves the sublagrangean dual problem without restriction for a certain u at optimality.
    This is possible when the Lagrangean Formulation has no constraints.
*/
solution_pair find_int_optimal_solution_lagrangean_subproblem(LagrangeanFormulation& lf){
    solution_pair s;
    vector<double> lc = lf.lagrangean_costs();

    for(int j=0;j<lf.c().size();j++){
        s.x.push_back(0);
    }

    for(int j=0;j<lc.size();j++){

        if(lf.objective_type()==MAX_TYPE){
            if(lc[j]>=0){
                s.x[j] = 1;
            }else{
                s.x[j] = 0;
            }            
        }else{
            if(lc[j]<=0){
                s.x[j] = 1;
            }else{
                s.x[j] = 0;
            }                        
        }

        if(lf.check_constraints(s.x)==false){
            s.x[j] = 0;
        }

    }
    s.vx = lf.compute(s.x);

    return s;
}
