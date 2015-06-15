#include "io.h"
#include "constants.h"

void jumps_newline(FILE* f){
    //Jumps the white characters and the newline
    int comeback;
    fscanf(f,"%*d%n",&comeback);
    fseek(f,-(comeback-1),SEEK_CUR);    
}

int read_input(vector< vector<double> >& A, vector<double>& b, vector<double>& c, vector<int>& op, 
               int& objective_type){
    return read_input(A,b,c,op,objective_type,stdin);
}

int read_input(vector< vector<int> >& A_index, vector< vector<double> >& A_cost, vector<double>& b, 
               vector<double>& c, vector<int>& op, int& objective_type){

    return read_input(A_index,A_cost,b,c,op,objective_type,stdin);
}

int read_input(vector< vector<double> >& A, vector<double>& b, vector<double>& c, vector<int>& op, 
               int& objective_type, FILE* f){
    /*
        File format:

        n (number of restrictions)
        m (number of variables)
        max || min (objective function type)
        2x1 +3x2 +5x4 (objective function)
        x1 +1x3 +4x4 -2x5 <= 17 (m restrictions)
    */    
    int n,m;
    int xj;
    double aij,bi,cj;
    char type[3];
    char* line = (char*) malloc(sizeof(char)*1024);
    char* op_str = (char*) malloc(sizeof(char)*2);
    int size_op_str;

    fscanf(f,"%d %d",&n,&m);    
    c.resize(m);
    for(int j=0;j<m;j++){
        c[j] = 0;
    }
    op.resize(n);
    
    fscanf(f,"%s",type);
    if(type[0]=='m' && type[1]=='a' && type[2]=='x'){
        objective_type = MAX_TYPE;
    }else{
        objective_type = MIN_TYPE;
    }

    //Jumps the white characters and the newline
    jumps_newline(f);

    fgets(line,1024,f);

    int chars_read;
    int total_read=0;
    while(sscanf(line+total_read,"%lfx%d%n",&cj,&xj,&chars_read)==2){
        c[xj-1] = cj;
        total_read+=chars_read;
    }
    printf("AKAIAKI\n");
    for(int i=0;i<n;i++){
        memset(line,'\0',sizeof(char)*1024);
        memset(op_str,'\0',sizeof(char)*2);

        A.push_back(vector<double>());
        A[i].resize(m);
        for(int j=0;j<m;j++){
            A[i][j] = 0;
        }

        fgets(line,1024,f); //Advances f pointer
        // printf("%s\n",line);
        total_read=0;
        while(sscanf(line+total_read,"%lfx%d%n",&aij,&xj,&chars_read)==2){
            A[i][xj-1]=aij;
            total_read+=chars_read;
        }        

        sscanf(line+total_read,"%*[ ]%[<=>]%n",op_str,&chars_read);   //It does the trim
        size_op_str = strlen(op_str);
        total_read+=chars_read;
        if(size_op_str==1){
            if(op_str[0]=='>'){
                op[i] = GREATER;
            }else if(op_str[0]=='<'){
                op[i] = LESSER;
            }else{
                op[i] = EQUAL;
            }
        }else if(size_op_str==2){
            if(op_str[0]=='>'){
                op[i] = GREATER_EQUAL;
            }else if(op_str[0]=='<'){
                op[i] = LESSER_EQUAL;
            }
        }
        sscanf(line+total_read,"%lf%n",&bi,&chars_read);
        total_read+=chars_read;

        b.push_back(bi);
    }

    free(line);
    free(op_str);
}

int read_input(vector< vector<int> >& A_index, vector< vector<double> >& A_cost, vector<double>& b, 
               vector<double>& c, vector<int>& op, int& objective_type, FILE* f){
    /*
        File format: (SET PACKING) it considers 1-based-index for n and m

        n (number of restrictions)
        m (number of variables)
        m coefficients (values), one for each variable
        For each constraint j=1,...,n
            The number K of non-null coefficients in this constraint
            K variables indexes, representin the variables with coefficient 1
    */    
    int n,m,k,var_index;
    int xj;
    double cj;

    fscanf(f,"%d %d",&n,&m);    
    c.resize(m);
    for(int j=0;j<m;j++){
        fscanf(f,"%lf",&cj);
        c[j] = cj;
    }

    op.resize(n);
    b.resize(n);
    A_index.resize(n);
    A_cost.resize(n);
    
    objective_type = MAX_TYPE;

    for(int i=0;i<n;i++){
        fscanf(f,"%d",&k);        
        A_index[i].resize(k);
        A_cost[i].resize(k);

        for(int j=0;j<k;j++){
            fscanf(f,"%d",&var_index);
            A_index[i][j] = var_index-1;
            A_cost[i][j] = 1;
        }

        op[i] = LESSER_EQUAL;
        b[i] = 1;
    }

}

void print_solution(string name, solution_pair& s){
    printf("%s: ", name.c_str());
    printf("(");
    for(int i=0;i<s.x.size();i++){
        printf("%.2lf ",i,s.x[i]);
    }
    printf(") VALUE: %.4lf\n", s.vx);
}

void print_vector(string name, vector<double> lbda){
    print_vector(name,lbda,true);
}

void print_vector(string name, vector<double> lbda, bool short_precision = true){
    printf("%s: (",name.c_str());
    for(int i=0;i<lbda.size();i++){
        if(short_precision) printf("%.6lf ",lbda[i]);
        else printf("%.6lf ",lbda[i]);
    }
    printf(")\n");
}
