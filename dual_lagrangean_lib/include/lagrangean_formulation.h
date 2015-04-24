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

    vector< RestrictionLine* > _dual_restrictions;
    vector< RestrictionLine* > _primal_restrictions;

    void check_lbda_setted();    
    void init(bool ALL_CONST_ARE_DUAL, int* dual_mask);
    void update_lagrangean_costs();

public:
    LagrangeanFormulation():Formulation(){};
    
    LagrangeanFormulation(Formulation& f, bool ALL_CONST_ARE_DUAL=true, int* dual_mask=NULL );
    LagrangeanFormulation(Formulation& f, bool ALL_CONST_ARE_DUAL, int* dual_mask, vector< RestrictionLine* > extra_primal, vector< RestrictionLine* > extra_dual );

    inline void lbda(vector<double> p_lbda){ _lbda=p_lbda; update_lagrangean_costs(); };
    inline vector<double>& lbda(){ return _lbda; };
    inline vector<double>& lagrangean_costs(){return _lagrangean_costs;};

    inline vector< RestrictionLine* >::iterator begin(){ return _primal_restrictions.begin(); };
    inline vector< RestrictionLine* >::iterator end(){ return _primal_restrictions.end(); };    

    inline vector< RestrictionLine* >::iterator dual_begin(){ return _dual_restrictions.begin(); }    
    inline vector< RestrictionLine* >::iterator dual_end(){ return _dual_restrictions.end(); }

    double compute(vector<double> p_x);
    bool check_restrictions(vector<double> p_x);

    string to_str();
};

#endif