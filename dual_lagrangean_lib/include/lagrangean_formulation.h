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
    vector<double> _lbda;
    vector<double> _lagrangean_costs;

    unordered_map< int, ConstraintLine* > _dual_constraints;    //Constraints in the objective function
    unordered_map< int, ConstraintLine* > _primal_constraints;  //Constraints in the polyhedron

    vector< unordered_set<int> > _restr_var_appears;   //Stores the restrictions indexes where the var appears;    

    vector< int > _primal_mask;

    void check_lbda_setted();    
    void init();
    void update_lagrangean_costs();
    void select_restrictions();

public:
    LagrangeanFormulation():Formulation(){};
    
    LagrangeanFormulation(Formulation& f);
    LagrangeanFormulation(Formulation& f, vector< int >& dual_mask, const vector< ConstraintLine* >& extra_primal, const vector< ConstraintLine* >& extra_dual );

    LagrangeanFormulation(const LagrangeanFormulation& p_lf);
    LagrangeanFormulation& operator=(const LagrangeanFormulation& p_lf);    

    inline void lbda(vector<double> p_lbda){ _lbda=p_lbda; update_lagrangean_costs(); };
    inline vector<double>& lbda(){ return _lbda; };
    inline vector<double>& lagrangean_costs(){return _lagrangean_costs;};
    
    inline int times_var_appears(int var_index){ return _restr_var_appears[var_index].size(); }
    inline unordered_set<int>& constr_var_appears(int var_index){ return _restr_var_appears[var_index]; }

    inline int num_dual_constraints(){ return _dual_constraints.size(); }

    ConstraintLine* replace_constraint(vector<ConstraintMember>& vec_cm, ConstraintLine* cl);

    inline line_it begin(){ return _primal_constraints.begin(); };
    inline line_it end(){ return _primal_constraints.end(); };    

    inline line_it dual_begin(){ return _dual_constraints.begin(); }    
    inline line_it dual_end(){ return _dual_constraints.end(); }

    double compute(vector<double> p_x);
    bool check_constraints(vector<double> p_x);

    string to_str();
};

#endif