#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>

#include "heuristics.h"


void print_results(int** sets,int num_sets,int num_elements){
    for(int i=0;i<num_elements;i++){
        for(int j=0;j<num_sets;j++){
            printf("%d ",sets[i][j]);
        }
        printf("\n");
    }
}

void create_example(int num_elements, int num_sets, int** elements, int*** sets){
    (*elements) = (int*) malloc(sizeof(int)*num_elements);
    if((*elements)==NULL) exit(1);

    (*sets) = (int**) malloc(sizeof(int*)*num_elements);
    if((*sets)==NULL) exit(1);
    for(int i=0;i<num_elements;i++){
        (*sets)[i] = (int*) calloc(num_sets,sizeof(int));
        if((*sets)[i]==NULL) exit(1);
    }

    for(int i=0;i<num_elements;i++){
        (*elements)[i] = i;
    }

    int el_choosen;
    for(int i=0;i<num_elements;i++){
        for(int j=0;j<num_sets;j++){
            if( rand()%5==0 ) (*sets)[i][j] = 1;
            else (*sets)[i][j] = 0;
        }
    }    
}


void print_solutions(vector<solution_pair> solutions){
    for(int i=0;i<solutions.size();i++){
        printf("Solution %d: ", i);
        for(int j=0;j<solutions[i].x.size();j++){
            printf("%.2lf ",solutions[i].x[j]);
        }
        printf("\n");
    }
}


//Set_Packing: Each xj variable represents the use of subset xj
double objective_function(solution_pair s){
    double m=0;
    for(int i=0;i<s.x.size();i++){
        m+=s.x[i];
    }
    return m;
}

//xj = 1 means the subset j is being used on the solution
//If one element appears more than once on the subsets used, the solution is not valid
bool check_constraints(solution_pair s,int** sets,int num_elements,int num_sets){
    int* elements_used = (int*) calloc(num_elements,sizeof(int));
    if(elements_used==NULL) exit(1);

    for(int j=0;j<num_sets;j++){
        if(s.x[j]==1){
            for(int i=0;i<num_elements;i++){
                if(sets[i][j]==1 && elements_used[i]==1){
                    return false;
                }

                if(sets[i][j]==1){
                    elements_used[i] = 1;    
                }//After set as one, can`t reset as 0 again.
                
            }
        }
    }
    return true;
    free(elements_used);
}

vector<solution_pair> enumerate_valid_solutions(int num_sets){
    int* power_2 = (int*) malloc(sizeof(int)*num_sets);
    if(power_2==NULL) exit(1);

    power_2[0]=1;
    for(int i=1;i<num_sets;i++){
        power_2[i]=power_2[i-1]*2;
    }

    int num_solutions = power_2[num_sets-1]*2;

    vector<solution_pair> solutions;
    int cur_n;
    for(int n=0;n<num_solutions;n++){
        solution_pair s;
        cur_n = n;
        for(int i=num_sets-1;i>=0;i--){
            s.x.push_back( cur_n/power_2[i] );
            cur_n-= (cur_n/power_2[i])*power_2[i];
        }
        solutions.push_back(s);
    }

    free(power_2);

    return solutions;
}

vector<solution_pair> get_optimal_solution(vector<solution_pair> all, int** sets, int num_elements, int num_sets){
    double max = 0;
    vector<solution_pair> max_solutions;

    double v;
    for(int i=0;i<all.size();i++){
        if(check_constraints(all[i],sets,num_elements,num_sets)==false) continue;
        all[i].vx = objective_function(all[i]);
        if(all[i].vx>max){
            max=all[i].vx;
        }
    }

    for(int i=0;i<all.size();i++){    
        if(check_constraints(all[i],sets,num_elements,num_sets)==false) continue;
        if(all[i].vx==max){
            max_solutions.push_back(all[i]);
        }
    }

    return max_solutions;
}

void create_input(int** sets, int num_elements, int num_sets, solution_pair optimal_solution, int version){
    char name[1024];
    char buffer[2<<16];
    int total_written=0;

    sprintf(name,"%d_elem_%d_sets_sol_%d_version_%d",num_elements,num_sets,(int)(optimal_solution.vx),version);
    
    total_written += sprintf( (buffer+total_written),"%d\n%d\nmax\n",num_elements,num_sets); //num_constraints,num_variables
    for(int j=0;j<num_sets;j++){
        total_written += sprintf( (buffer+total_written),"+1x%d ",j+1);
    }
    total_written += sprintf( (buffer+total_written),"\n");

    bool has_at_least_one;
    int total_constraints=0;
    for(int i=0;i<num_elements;i++){
        has_at_least_one=false;
        for(int j=0;j<num_sets;j++){
            if(sets[i][j]==1){
                has_at_least_one=true;
                total_written += sprintf( (buffer+total_written),"+1x%d ",j+1);            
            }
        }
        if(has_at_least_one){
            total_constraints+=1;
            total_written += sprintf( (buffer+total_written)," <= 1\n");
        }      
    }
    
    //update restrictions
    total_written = sprintf( buffer,"%d\n%d\nmax",total_constraints,num_sets); //num_constraints,num_variables
    buffer[total_written]='\n';

    FILE* f = fopen(name,"w");
    fprintf(f,"%s",buffer);

    fclose(f);
}

int main(int argc, char* argv[]){
    srand(time(NULL));

    int num_elements;
    int num_sets;

    int* elements;
    int** sets;

    printf("Enter with the number of elements and the number os subsets:\n");
    (void)(scanf("%d %d",&num_elements,&num_sets)+1);

    create_example(num_elements,num_sets,&elements,&sets);

    print_results(sets,num_sets,num_elements);

    vector<solution_pair> solutions = enumerate_valid_solutions(num_sets);
    
    vector<solution_pair> optimal_solutions = get_optimal_solution(solutions,sets,num_elements,num_sets);
    if(optimal_solutions.size()>0){
        printf("OPTIMAL SOLUTIONS (%.2lf)\n",optimal_solutions[0].vx);
        print_solutions(optimal_solutions);
        create_input(sets,num_elements,num_sets,optimal_solutions[0],1);
    }else{
        printf("NO SOLUTION\n");
    }




    free(elements);
    for(int i=0;i<num_elements;i++) free(sets[i]);
    free(sets);

    return 0;
}