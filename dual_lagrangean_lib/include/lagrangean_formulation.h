#ifndef __LAGRANGEAN_FORMULATION__
#define __LAGRANGEAN_FORMULATION__

#include <vector>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>

#include "formulation.h"

class LagrangeanFormulation: public Formulation{
private:
    vector<double> _lbda;
    vector<double> _lagrangean_costs;

    vector< ConstraintLine* > _dual_constraints;
    vector< vector<int> > _restr_var_appears;   //Stores the restrictions indexes where the var appears;

    vector< ConstraintLine* > _primal_constraints;
    vector< int > _dual_mask;

    void check_lbda_setted();    
    void init();
    void update_lagrangean_costs();
    void select_restrictions();

public:
    LagrangeanFormulation():Formulation(){};
    
    LagrangeanFormulation(Formulation& f);
    LagrangeanFormulation(Formulation& f, vector< int >& dual_mask, vector< ConstraintLine* > extra_primal, vector< ConstraintLine* > extra_dual );

    LagrangeanFormulation(const LagrangeanFormulation& p_lf);
    LagrangeanFormulation& operator=(const LagrangeanFormulation& p_lf);    

    inline void lbda(vector<double> p_lbda){ _lbda=p_lbda; update_lagrangean_costs(); };
    inline vector<double>& lbda(){ return _lbda; };
    inline vector<double>& lagrangean_costs(){return _lagrangean_costs;};
    
    inline int times_var_appears(int var_index){ return _restr_var_appears[var_index].size(); }
    inline vector<int>& constr_var_appears(int var_index){ return _restr_var_appears[var_index]; }

    inline int num_dual_constraints(){ return _dual_constraints.size(); }

    inline vector< ConstraintLine* >::iterator begin(){ return _primal_constraints.begin(); };
    inline vector< ConstraintLine* >::iterator end(){ return _primal_constraints.end(); };    

    inline vector< ConstraintLine* >::iterator dual_begin(){ return _dual_constraints.begin(); }    
    inline vector< ConstraintLine* >::iterator dual_end(){ return _dual_constraints.end(); }

    double compute(vector<double> p_x);
    bool check_constraints(vector<double> p_x);

    string to_str();
};

#endif