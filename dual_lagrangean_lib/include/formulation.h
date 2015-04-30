#ifndef __FORMULATION__
#define __FORMULATION__

#include <vector>
#include <string>
#include <sstream>
#include "constants.h"
#include "types.h"
#include "constraint_line.h"

using namespace std;

class Formulation{
private:
    void copy_formulation(Formulation& p_f);

protected:
    vector< ConstraintLine* > _constraints;
    vector<double> _c;
    int _objective_type;

    bool check_constraint(ConstraintLine& rl,vector<double>& x);

public:
    Formulation(){};
    Formulation(vector< vector<double> >& p_A, vector<double>& p_b, vector<double>& p_c, 
                vector<int>& p_op, int p_objective_type);

    Formulation(vector< vector<int> >& p_A_index, vector< vector<double> >& p_A_cost, vector<double>& p_b, 
                vector<double>& p_c, vector<int>& p_op, int p_objective_type );

    Formulation(Formulation& f);

    inline vector< ConstraintLine* >::iterator begin(){ return _constraints.begin(); };
    inline vector< ConstraintLine* >::iterator end(){ return _constraints.end(); };    

    inline int num_constraints(){return _constraints.size();};

    inline vector<double>& c(){ return _c; };    

    inline int objective_type(){ return _objective_type; };

    virtual double compute(vector<double> p_x);    
    virtual bool check_constraints(vector<double> p_x);    

    string to_str();
};

#endif