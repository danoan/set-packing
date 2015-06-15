#include "util.h"

ConstraintLine* get_knapsack_constraint(Formulation& f){
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

double knapsack(Formulation& f, int* x){
    ConstraintLine* cl_knapsack = get_knapsack_constraint(f);

    int m = f.num_constraints();
    int n = f.c().size();
    double* c = new double[n];
    int* a = new int[n];
    int b = (int) cl_knapsack->rhs();
   
    int i=0;
    for(member_it m_it=cl_knapsack->begin();m_it!=cl_knapsack->end();m_it++){
        a[i++]=(int) m_it->cost;
    }

    for(int i=0;i<f.c().size();i++){
        c[i] = f.c()[i];
    }


    return knapsack(c,a,b,n,m,x);   
}