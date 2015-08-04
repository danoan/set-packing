#ifndef __IO__
#define __IO__

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <cstring>
#include <string>
#include <sstream>

#include "constants.h"
#include "solution.h"
#include "types.h"

using namespace std;

int read_input(vector< vector<double> >& A, vector<double>& b, vector<double>& c, vector<int>& op,int& objective_type);
int read_input(vector< vector<double> >& A, vector<double>& b, vector<double>& c, vector<int>& op,int& objective_type, FILE* f);

int read_input(vector< vector<int> >& A_index, vector< vector<double> >& A_cost, vector<double>& b, 
               vector<double>& c, vector<int>& op, int& objective_type);

int read_input(vector< vector<int> >& A_index, vector< vector<double> >& A_cost, vector<double>& b, 
               vector<double>& c, vector<int>& op, int& objective_type, FILE* f);

void print_solution(const string& name, Solution& s);
void print_solution(ostringstream& ss, const string& name, Solution& s);

void print_vector(const string& name, const vector<double>& lbda);
void print_vector(const string& name, const vector<SolutionComponent>& comps);
void print_vector(const string& name, const vector<double>& lbda, bool short_precision);

void print_vector(ostringstream& ss, const string& name, const vector<double>& lbda);
void print_vector(ostringstream& ss, const string& name, const vector<SolutionComponent>& comps);
void print_vector(ostringstream& ss, const string& name, const vector<double>& lbda, bool short_precision);

#endif