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
    vector<int> _no_lagrangean_restrictions;

    void check_lbda_setted();    
    void init();
    void update_lagrangean_costs();

public:
    LagrangeanFormulation():Formulation(){};
    LagrangeanFormulation(vector< vector<double> >& p_A, vector<double>& p_b, 
                          vector<double>& p_c, vector<int>& p_op, int p_objective_type);   

    LagrangeanFormulation(vector< vector<double> >& p_A, vector<double>& p_b, 
                          vector<double>& p_c, vector<int>& p_op, 
                          int p_objective_type, vector<int>& p_no_lagrangean_restrictions );


    LagrangeanFormulation(vector< vector<int> >& p_A_index, vector< vector<double> >& p_A_cost, 
                          vector<double>& p_b, vector<double>& p_c, vector<int>& p_op, 
                          int p_objective_type);

    LagrangeanFormulation(vector< vector<int> >& p_A_index, vector< vector<double> >& p_A_cost, 
                          vector<double>& p_b, vector<double>& p_c, vector<int>& p_op, 
                          int p_objective_type, vector<int>& p_no_lagrangean_restrictions );


    inline void lbda(vector<double> p_lbda){ _lbda=p_lbda; update_lagrangean_costs(); };
    inline vector<double> lbda(){ return _lbda; };
    inline vector<double> lagrangean_costs(){return _lagrangean_costs;};

    double compute(vector<double> p_x);
    bool check_restrictions(vector<double> p_x);

    string to_str();
};

#endif