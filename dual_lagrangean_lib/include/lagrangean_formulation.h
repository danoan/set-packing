#ifndef __LAGRANGEAN_FORMULATION__
#define __LAGRANGEAN_FORMULATION__

#include <vector>
#include <unordered_set>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>

#include "formulation.h"

class LagrangeanFormulation: public Formulation{
private:
    vector<double> _lagrangean_costs;

    unordered_map< int, ConstraintLine* > _dual_constraints;    //Constraints in the objective function
    unordered_map< int, ConstraintLine* > _primal_constraints;  //Constraints in the polyhedron

    unordered_map< int, ConstraintLine* > _active_constraints;

    vector< unordered_set<int> > _restr_var_appears;   //Stores the restrictions indexes where the var appears;    

    vector< int > _primal_mask;

    void init();    
    void select_restrictions();

public:
    LagrangeanFormulation():Formulation(){};
    
    LagrangeanFormulation(Formulation& f);
    LagrangeanFormulation(Formulation& f, vector< int >& dual_mask, const vector< ConstraintLine* >& extra_primal, const vector< ConstraintLine* >& extra_dual );

    LagrangeanFormulation(const LagrangeanFormulation& p_lf);
    LagrangeanFormulation& operator=(const LagrangeanFormulation& p_lf);    

    inline vector<double>& lagrangean_costs(){return _lagrangean_costs;};
    void update_lagrangean_costs();
    
    inline int times_var_appears(int var_index){ return _restr_var_appears[var_index].size(); }
    inline unordered_set<int>& constr_var_appears(int var_index){ return _restr_var_appears[var_index]; }

    inline int num_dual_constraints(){ return _dual_constraints.size(); }

    inline int num_active_constraints(){ return _active_constraints.size(); }
    inline const unordered_map< int, ConstraintLine* >& active_constraints(){ return _active_constraints; }    
    void make_active_constraint(ConstraintLine* cl);
    void make_inactive_constraint(ConstraintLine* cl);


    ConstraintLine* replace_constraint(vector<ConstraintMember>& vec_cm, ConstraintLine* cl);
    void add_new_constraint(ConstraintLine* cl);
    void remove_constraint(ConstraintLine* cl);


    inline line_it primal_begin(){ return _primal_constraints.begin(); };
    inline line_it primal_end(){ return _primal_constraints.end(); };    

    inline line_it dual_begin(){ return _dual_constraints.begin(); }    
    inline line_it dual_end(){ return _dual_constraints.end(); }

    double compute(const vector<SolutionComponent>& comps);
    bool check_constraints(const vector<SolutionComponent>& comps);

    string to_str();
};

#endif