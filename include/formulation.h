#ifndef __FORMULATION__
#define __FORMULATION__

#include <vector>
#include <string>
#include <sstream>
#include "constants.h"

using namespace std;

typedef struct _restriction_line_member{
    int index;
    double cost;
} restriction_line_member;

typedef struct _line{
    vector< restriction_line_member > members;
    double rhs;
    int op;
} restriction_line;

typedef vector<restriction_line>::iterator line_it;
typedef vector<restriction_line_member>::iterator member_it;

class Formulation{
private:
    bool _backpack_restriction_flag;
    void init_backpack_restriction(int p_num_variables);
protected:
    vector<restriction_line> _restrictions;
    vector<double> _c;

    restriction_line _backpack_restriction;
    int _objective_type;

    bool check_restriction(restriction_line& rl,vector<double>& x);

public:
    Formulation(){};
    Formulation(vector< vector<double> >& p_A, vector<double>& p_b, vector<double>& p_c, 
                vector<int>& p_op, int p_objective_type);

    Formulation(vector< vector<int> >& p_A_index, vector< vector<double> >& p_A_cost, vector<double>& p_b, 
                vector<double>& p_c, vector<int>& p_op, int p_objective_type );

    inline vector< restriction_line >::iterator restrictions_begin(){ return _restrictions.begin(); };
    inline vector< restriction_line >::iterator restrictions_end(){ return _restrictions.end(); };
    inline int num_restrictions(){return _restrictions.size();};

    inline vector<double> c(){ return _c; };    

    inline int objective_type(){ return _objective_type; };
    inline restriction_line backpack_restriction(){ return _backpack_restriction; };

    virtual double compute(vector<double> p_x);    
    virtual bool check_restrictions(vector<double> p_x);    

    void set_backpack_restriction();
    void set_backpack_restriction(vector<double> p_new_A);

    string to_str();
};

#endif