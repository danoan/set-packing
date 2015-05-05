#include <cmath>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <dirent.h>
#include <map>

#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>


#include "util.h"
#include "formulation.h"
#include "lagrangean_formulation.h"
#include "simple_dual_solver.h"
#include "minknap_dual_solver.h"

#define INPUT_DIR "/home/daniel/Projects/set_packing/code/dual_solvers/input/instances/"

bool DEBUG = false;
std::map<string,double> OPT_VALUES;


struct timeval start_timer(){
    struct timeval timer;
    gettimeofday(&timer,NULL);

    return timer;
}

struct timeval end_timer(struct timeval *start){
    struct timeval stop;
    gettimeofday(&stop,NULL);

    struct timeval time_passed;
    time_passed.tv_sec = stop.tv_sec - start->tv_sec;
    time_passed.tv_usec = stop.tv_usec - start->tv_usec;

    if(time_passed.tv_usec<0){
        time_passed.tv_sec -= 1;
        time_passed.tv_usec += 1000000;
    }

    return time_passed;
}

void end_and_print(const char *timer_name, struct timeval *timer){
    struct timeval final = end_timer(timer);
    printf("*** %s - %d.%d ***\n",timer_name,final.tv_sec,final.tv_usec);
}

void init(){
    OPT_VALUES["pb_100rnd0100.dat"] = 372.0;    
    OPT_VALUES["pb_100rnd0200.dat"] = 34.0;
    OPT_VALUES["pb_100rnd0300.dat"] = 203.0;
    OPT_VALUES["pb_100rnd0400.dat"] = 16.0;
    OPT_VALUES["pb_100rnd0500.dat"] = 639.0;
    OPT_VALUES["pb_100rnd0600.dat"] = 64.0;
    OPT_VALUES["pb_100rnd0700.dat"] = 503.0;
    OPT_VALUES["pb_100rnd0800.dat"] = 39.0;
    OPT_VALUES["pb_100rnd0900.dat"] = 463.0;
    OPT_VALUES["pb_100rnd1000.dat"] = 40.0;
    OPT_VALUES["pb_100rnd1100.dat"] = 306.0;
    OPT_VALUES["pb_100rnd1200.dat"] = 23.0;

    OPT_VALUES["pb_200rnd0100.dat"] = 416.0;
    OPT_VALUES["pb_200rnd0200.dat"] = 32.0;
    OPT_VALUES["pb_200rnd0300.dat"] = 731.0;
    OPT_VALUES["pb_200rnd0400.dat"] = 64.0;
    OPT_VALUES["pb_200rnd0500.dat"] = 184.0;
    OPT_VALUES["pb_200rnd0600.dat"] = 14.0;
    OPT_VALUES["pb_200rnd0700.dat"] = 1004.0;
    OPT_VALUES["pb_200rnd0800.dat"] = 83.0;
    OPT_VALUES["pb_200rnd0900.dat"] = 1324.0;
    OPT_VALUES["pb_200rnd1000.dat"] = 118.0;
    OPT_VALUES["pb_200rnd1100.dat"] = 545.0;
    OPT_VALUES["pb_200rnd1200.dat"] = 43.0;
    OPT_VALUES["pb_200rnd1300.dat"] = 571.0;
    OPT_VALUES["pb_200rnd1400.dat"] = 45.0;
    OPT_VALUES["pb_200rnd1500.dat"] = 926.0;
    OPT_VALUES["pb_200rnd1600.dat"] = 79.0;
    OPT_VALUES["pb_200rnd1700.dat"] = 255.0;
    OPT_VALUES["pb_200rnd1800.dat"] = 19.0;

    OPT_VALUES["pb_500rnd0100.dat"] = 323.0;
    OPT_VALUES["pb_500rnd0200.dat"] = 25.0;
    OPT_VALUES["pb_500rnd0300.dat"] = 776.0;
    OPT_VALUES["pb_500rnd0400.dat"] = 62.0;
    OPT_VALUES["pb_500rnd0500.dat"] = 122.0;
    OPT_VALUES["pb_500rnd0600.dat"] = 8.0;
    OPT_VALUES["pb_500rnd0700.dat"] = 1141.0;
    OPT_VALUES["pb_500rnd0800.dat"] = 89.0;
    OPT_VALUES["pb_500rnd0900.dat"] = 2236.0;
    OPT_VALUES["pb_500rnd1000.dat"] = 179.0;
    OPT_VALUES["pb_500rnd1100.dat"] = 424.0;
    OPT_VALUES["pb_500rnd1200.dat"] = 33.0;
    OPT_VALUES["pb_500rnd1300.dat"] = 474.0;
    OPT_VALUES["pb_500rnd1400.dat"] = 38.0;
    OPT_VALUES["pb_500rnd1500.dat"] = 1196.0;
    OPT_VALUES["pb_500rnd1600.dat"] = 88.0;
    OPT_VALUES["pb_500rnd1700.dat"] = 192.0;
    OPT_VALUES["pb_500rnd1800.dat"] = 13.0;

    OPT_VALUES["pb_1000rnd0100.dat"] = 67.0;    
    OPT_VALUES["pb_1000rnd0200.dat"] = 4.0;    
    OPT_VALUES["pb_1000rnd0300.dat"] = 661.0;    
    OPT_VALUES["pb_1000rnd0400.dat"] = 48.0;    
    OPT_VALUES["pb_1000rnd0500.dat"] = 222.0;    
    OPT_VALUES["pb_1000rnd0600.dat"] = 15.0;    
    OPT_VALUES["pb_1000rnd0700.dat"] = 2260.0;    
    OPT_VALUES["pb_1000rnd0800.dat"] = 175.0;    

    OPT_VALUES["pb_2000rnd0100.dat"] = 40.0;    
    OPT_VALUES["pb_2000rnd0200.dat"] = 2.0;    
    OPT_VALUES["pb_2000rnd0300.dat"] = 478.0;    
    OPT_VALUES["pb_2000rnd0400.dat"] = 32.0;    
    OPT_VALUES["pb_2000rnd0500.dat"] = 140.0;    
    OPT_VALUES["pb_2000rnd0600.dat"] = 9.0;    
    OPT_VALUES["pb_2000rnd0700.dat"] = 1811.0;    
    OPT_VALUES["pb_2000rnd0800.dat"] = 135.0;    
}

Formulation short_input(FILE* file){
    int objective_type;
    vector< vector<int> > A_index;
    vector< vector<double> > A_cost;
    vector<double> b;
    vector<double> c;
    vector<int> op;

    read_input(A_index,A_cost,b,c,op,objective_type,file);
    Formulation f(A_index,A_cost,b,c,op,objective_type);    

    return f;
}

Formulation long_input(FILE* file){
    int objective_type;
    vector< vector<double> > A_cost;
    vector<double> b;
    vector<double> c;
    vector<int> op;

    read_input(A_cost,b,c,op,objective_type,file);
    Formulation f(A_cost,b,c,op,objective_type);

    return f;
}

void solve(int max_N, FILE* file, const string filename){
    Formulation f;

    f = short_input(file);
    dual_lagrangean_solution s_simp, s_mink;

    struct timeval simple_timer_start = start_timer();
    SimpleDualSolver pls_simp(f);    
    s_simp = pls_simp.solve(max_N);
    struct timeval simple_timer_end = end_timer(&simple_timer_start);

    struct timeval mink_timer_start = start_timer();
    MinknapDualSolver pls_mink(f);    
    s_mink = pls_mink.solve(max_N);
    struct timeval mink_timer_end = end_timer(&mink_timer_start);

    bool check_simp, check_mink;
    check_simp = (s_simp.p.vx <= OPT_VALUES[filename] && s_simp.d.vx >= OPT_VALUES[filename]);
    check_mink = (s_mink.p.vx <= OPT_VALUES[filename] && s_mink.d.vx >= OPT_VALUES[filename]);

    bool mink_better = (s_simp.d.vx - s_simp.p.vx) > (s_mink.d.vx - s_mink.p.vx);
    double gap_diff = (s_mink.d.vx - s_mink.p.vx) - (s_simp.d.vx - s_simp.p.vx);
        
    printf("SIMPLE: \tPRIMAL: %.5lf \tDUAL:%.5lf\t BEST VALUE KNOWN: %.5lf\t %s (%d.%d)\n",s_simp.p.vx, s_simp.d.vx,OPT_VALUES[filename], check_simp?"OK":"SOMETHING WRONG",simple_timer_end.tv_sec,simple_timer_end.tv_usec);
    printf("MINKNAP:\tPRIMAL: %.5lf \tDUAL:%.5lf\t BEST VALUE KNOWN: %.5lf\t %s - %s (%.5lf) (%d.%d)\n",s_mink.p.vx, s_mink.d.vx,OPT_VALUES[filename], check_mink?"OK":"SOMETHING WRONG", mink_better?"BETTER":"WORST", gap_diff,mink_timer_end.tv_sec,mink_timer_end.tv_usec);
}

bool is_regular_file(const char* filepath){
    struct stat path_stat;
    stat(filepath,&path_stat);
    return S_ISREG(path_stat.st_mode);
}

int main(){
    init();
    struct timeval total_timer = start_timer();

    DIR *dir;
    struct dirent *ent;
    FILE* file;
    char buffer[512];
    int a;
    if( (dir=opendir(INPUT_DIR))!=NULL ){
        while(  (ent=readdir(dir))!=NULL ){
            if( is_regular_file( ( string(INPUT_DIR) + string(ent->d_name) ).c_str() ) ){
                printf("**** %s ****\n",ent->d_name);    
                a = snprintf(buffer,sizeof(buffer),"%s%s",INPUT_DIR,ent->d_name);
                buffer[a]='\0';

                file = fopen( buffer ,"r" );
                solve(30,file,string(ent->d_name));
                printf("\n");

                fclose(file);
            }
        }
    }

    end_and_print("TEMPO TOTAL", &total_timer);
}