#ifndef __FORMULATION__
#define __FORMULATION__

#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include "constants.h"
#include "types.h"
#include "constraint_line.h"


class SubgradientMethod;

using namespace std;

class Formulation{
private:
    void copy_formulation(const Formulation& p_f);

    std::vector< std::pair< SubgradientMethod*, void(SubgradientMethod::*)(ConstraintLine* cl) > > _callback_new_constraint_vector;
    std::vector< std::pair< SubgradientMethod*, void(SubgradientMethod::*)(ConstraintLine* cl) > > _callback_remove_constraint_vector;

protected:
    unordered_map< int, ConstraintLine* > _constraints;
    vector<double> _c;
    int _objective_type;
    bool _initialized_flag;

    bool check_constraint(ConstraintLine& rl,const vector<solution_component>& comps);

public:
    Formulation():_initialized_flag(false){};
    Formulation(vector< vector<double> >& p_A, vector<double>& p_b, vector<double>& p_c, 
                vector<int>& p_op, int p_objective_type);

    Formulation(vector< vector<int> >& p_A_index, vector< vector<double> >& p_A_cost, vector<double>& p_b, 
                vector<double>& p_c, vector<int>& p_op, int p_objective_type );

    Formulation(const Formulation& f);    
    ~Formulation();
    Formulation& operator=(const Formulation& p_f);

    inline line_it begin(){ return _constraints.begin(); };
    inline line_it end(){ return _constraints.end(); };    

    inline int num_constraints(){return _constraints.size();};
    inline vector<double>& c(){ return _c; };    
    inline int objective_type(){ return _objective_type; };

    virtual ConstraintLine* replace_constraint(vector<ConstraintMember>& vec_cm, ConstraintLine* cl);
    virtual void add_new_constraint(ConstraintLine* cl);
    virtual void remove_constraint(ConstraintLine* cl);

    virtual double compute(const vector<solution_component>& p_x);    
    virtual bool check_constraints(const vector<solution_component>& p_x);    

    inline void add_new_constraint_callback( SubgradientMethod* obj, void (SubgradientMethod::*_callback)(ConstraintLine*) ){
        _callback_new_constraint_vector.push_back( std::make_pair(obj, _callback) );
    };
    
    inline void add_remove_constraint_callback( SubgradientMethod* obj, void (SubgradientMethod::*_callback)(ConstraintLine*) ){
        _callback_remove_constraint_vector.push_back( std::make_pair(obj, _callback) );  
    };

    string to_str();
};

typedef std::vector< std::pair< SubgradientMethod*, void(SubgradientMethod::*)(ConstraintLine*) > >::iterator call_it;

#endif