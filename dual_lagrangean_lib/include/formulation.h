#ifndef __FORMULATION__
#define __FORMULATION__

#include <vector>
#include <string>
#include <sstream>
#include "constants.h"
#include "types.h"
#include "restriction_line.h"

using namespace std;

class Formulation{
private:
    void copy_formulation(Formulation& p_f);

protected:
    vector< RestrictionLine* > _restrictions;
    vector<double> _c;
    int _objective_type;

    bool check_restriction(RestrictionLine& rl,vector<double>& x);

public:
    Formulation(){};
    Formulation(vector< vector<double> >& p_A, vector<double>& p_b, vector<double>& p_c, 
                vector<int>& p_op, int p_objective_type);

    Formulation(vector< vector<int> >& p_A_index, vector< vector<double> >& p_A_cost, vector<double>& p_b, 
                vector<double>& p_c, vector<int>& p_op, int p_objective_type );

    Formulation(Formulation& f);

    inline vector< RestrictionLine* >::iterator begin(){ return _restrictions.begin(); };
    inline vector< RestrictionLine* >::iterator end(){ return _restrictions.end(); };    

    inline int num_restrictions(){return _restrictions.size();};

    inline vector<double>& c(){ return _c; };    

    inline int objective_type(){ return _objective_type; };

    virtual double compute(vector<double> p_x);    
    virtual bool check_restrictions(vector<double> p_x);    

    string to_str();
};

#endif