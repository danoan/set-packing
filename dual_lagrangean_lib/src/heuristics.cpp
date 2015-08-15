#include "heuristics.h"

bool compare_benefit_cost(BenefitCostPair a, BenefitCostPair b){
    //Decrescent order
    return a.value() > b.value();
}

/*
    Classes' atributes are defined such that the attribute cost_vector refers to the objective function 
    coeficients and the attribute cost of a RestrictionMember object is the constraint coefficient. In the 
    Set Packing context, the cost when we say benefit cost is the cardinality of each subset x[j] 
    (constraint coeffient), while the benefit is the profit (objective function coefficient). That's why 
    the equation to compute benefit cost is: ( (cost_vector[rm.index]/rm.cost) )
*/
vector<BenefitCostPair> compute_benefit_cost(ConstraintLine& br, vector<double>& cost_vector){
    vector<BenefitCostPair> benefit_cost;

    for(member_it it_m=br.begin();it_m!=br.end();it_m++){
        BenefitCostPair bcp;
        ConstraintMember rm = (*it_m);

        bcp.value( (cost_vector[rm.index]/rm.cost) );
        bcp.index(rm.index);

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
void find_primal_int_solution_by_benefit_cost_heuristic(Formulation& f, vector<BenefitCostPair>& bc_vector, Solution& s){
    BenefitCostPair bcp;
    for(int k=0;k<bc_vector.size();k++){
         bcp =  bc_vector[k];

         if(bcp.value()<0){
            s.set_component(bcp.index(),0);
         }else{
            s.set_component(bcp.index(),1);
         }
         

         if(f.check_constraints(s.x())==false){
            s.set_component(bcp.index(),0);
         }
    }
}


/*
    After the lagrangean subproblem is solved, a dual solution Dx for a certain u is known. This method
    uses the solution Dx to come up with a new feasible primal solution Px. In order to do that, it uses
    the Complementary Slackness Theorem. Considering the Set Packing Problem, this theorem ensures that
    if Dx[j]=0 then for sure Px[j]=0. In other words, only when Dx[j]=1 Px[j] could assume 1 as value.
*/
void find_primal_int_feasible_solution_from_dual(Solution& d, Formulation& f, vector<BenefitCostPair>& bc_vector, Solution& p){
    BenefitCostPair bcp;
    //Set the coefficients of the new solution as the same as the dual, but in order of benefit cost
    for(int k=0;k<bc_vector.size();k++){
        bcp =  bc_vector[k];
        if(d.x(bcp.index())==1){
            p.set_component(bcp.index(),1);

            if(f.check_constraints(p.x())==false){
                p.set_component(bcp.index(),0);
            }                    
        }else{
            p.set_component(bcp.index(),0);
        }
    }
    
    for(int k=0;k<bc_vector.size();k++){
         bcp =  bc_vector[k];
         if(p.x(bcp.index())==1){continue;} //It`s already set from the previous loop
         
         p.set_component(bcp.index(),1);
         if(f.check_constraints(p.x())==false){
            p.set_component(bcp.index(),0);
         }
    }
}

/*
    It solves the sublagrangean dual problem without restriction for a certain u at optimality.
    This is possible when the Lagrangean Formulation has no constraints.
*/
void find_int_optimal_solution_lagrangean_subproblem(LagrangeanFormulation& lf, Solution& d, const int fixed_variable, const int fixed_value){
    vector<double> lc = lf.lagrangean_costs();

    for(int j=0;j<lc.size();j++){

        if(j==fixed_variable){
            d.set_component(j,fixed_value);
            continue;
        }

        if(lf.objective_type()==MAX_TYPE){
            if(lc[j]>=0){
                d.set_component(j,1);
            }else{
                d.set_component(j,0);
            }            
        }else{
            if(lc[j]<=0){
                d.set_component(j,1);
            }else{
                d.set_component(j,0);
            }                        
        }

        if(lf.check_constraints(d.x())==false){
            d.set_component(j,0);
        }

    }
    d.vx( lf.compute(d.x()) );
}

bool local_search(Formulation& f, Solution& p, int deep){
    Solution best_solution = p;

    SolutionComponent sc;
    double start_value = p.best_value();
    for(int i=0;i<p.num_components();i++){
        sc = p.x()[i];
        if(sc.x()==1 && sc.fixed()==false){
            p.fix(i,0);
            local_search_it(f,p,best_solution,deep,0);
            p.unfix(i);
            p.set_component(i,1);
        }
    }
    
    if(p.best_value()>start_value){
        for(int i=0;i<p.num_components();i++){
            p.set_component(i,best_solution.x(i));
        }        
        p.vx( f.compute( p.x() ) );
        return true;
    }else{
        p.vx( f.compute( p.x() ) );
        return false;
    }
}

void local_search_it(Formulation& f, Solution& p, Solution& best_solution, const int& deep, int curr_deep){
    if(curr_deep==deep){
        return;
    }
    
    p.vx( f.compute( p.x() ) );
    if( p > best_solution){
        best_solution = p;
    }    

    SolutionComponent sc;
    for(int i=0;i<p.num_components();i++){
        sc = p.x()[i];
        if(sc.x()==0 && sc.fixed()==false){        
            p.fix(i,1);
            if(f.check_constraints(p.x())==true){
                local_search_it(f,p,best_solution,deep,curr_deep+1);    
            }            
            p.unfix(i);
            p.set_component(i,0);
        }
    }
}
