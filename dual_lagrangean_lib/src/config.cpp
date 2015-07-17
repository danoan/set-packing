#include "config.h"

double Config::gap_improving = 0.05;
double Config::pi_factor = 0.8;
PrimalHeuristic Config::primal_heuristic = CARDINALITY;
int Config::iterations = 1000;
bool Config::debug = false;
Solver Config::solver = NO_CONSTRAINTS;
InputType Config::input_type = SHORT;


int Config::read_input(int argc, char* argv[]){
    const char* options = "i:g:p:h:n:f:d:?";
    int c=0;

    while(1){
        c = getopt(argc,argv,options);
        if(c<0) break;
        switch(c){
            case 'i':{
                std::string i = std::string(optarg);
                if(i=="L") Config::input_type = LONG;
                else if(i=="S") Config::input_type = SHORT;                
                }        
                break;                            
            case 'g':
                Config::gap_improving = atof(optarg);
                break;
            case 'p':
                Config::pi_factor = atof(optarg);
                break;
            case 'h':{
                std::string h = std::string(optarg);
                if(h=="C") Config::primal_heuristic = CARDINALITY;
                else if(h=="L") Config::primal_heuristic = LAGRANGEAN_COST;                
                }
                break;                
            case 'n':
                Config::iterations = atoi(optarg);
                break;                
            case 'f':{
                std::string s = std::string(optarg);
                if(s=="N") Config::solver = NO_CONSTRAINTS;
                else if(s=="K") Config::solver = KNAPSACK;                
                }
                break;                
            case 'd':
                Config::debug = true;
                break;                
            case '?':              
                printf("-i What is the type of the input? (L:Long, S:Short). - (S)\n");
                printf("-g How many without gap improving iterations should be done before decrease pi (It should be enter as a fraction of number of iterations)(0-1) - (0.05).\n");
                printf("-p What is the decrease ratio of pi (0-1). - (0.8)\n");
                printf("-h Which heuristic to set a primal solution from a dual solution will be used? (C: Cardinality; L: Lagrangean Costs) - (C)\n");
                printf("-n Number of iterations. - (1000)\n");
                printf("-f Which solver to use? (N: No constraints; K: Knapsack Constraint) - (N) \n");
                printf("-d Print debug log? - (F)\n");
                
                return -1;
                break;
        }
    }

    return 0;
}