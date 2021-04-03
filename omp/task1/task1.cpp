#include <omp.h>
#include <iostream>
#include <math.h>

const int num_intervals = 100000000;

using namespace std;


void experiment1(){
    cout << "Running nonparallel for loop..."<< endl;
    double res = 0.0;

    double l_point = 0.0;
    double r_point = 1.0;
    double step = (r_point - l_point) / num_intervals;  
   
    double start = omp_get_wtime();
    for (int i=0; i<num_intervals; i++){ 
        res += 4.0 * (atan((i + 1) * step) - atan(i * step)); 
    }
    double end = omp_get_wtime();
    
    cout << "Integral = " << res << endl 
        << "Duration of nonparallel for loop = " << end - start << " seconds" << endl;
}

void experiment2(){
    cout << "Running parallel for loop..."<< endl;
    double res = 0.0;

    double l_point = 0.0;
    double r_point = 1.0;
    double step = (r_point - l_point) / num_intervals;  
   
    double start = omp_get_wtime();
    #pragma omp parallel for
    for (int i=0; i<num_intervals; i++){ 
        res += 4.0 * (atan((i + 1) * step) - atan(i * step)); 
    }
    double end = omp_get_wtime();
   
    // Result should be wrong if number of threads > 1, because of the data race 
    cout << "Integral = " << res << endl 
        << "Duration of parallel for loop = " << end - start << " seconds" << endl;
}

void experiment3(){
    cout << "Running parallel for loop with reduction..."<< endl;
    double res = 0.0;

    double l_point = 0.0;
    double r_point = 1.0;
    double step = (r_point - l_point) / num_intervals;  
   
    double start = omp_get_wtime();
    #pragma omp parallel for reduction(+: res)
    for (int i=0; i<num_intervals; i++){ 
        res += 4.0 * (atan((i + 1) * step) - atan(i * step)); 
    }
    double end = omp_get_wtime();
    
    cout << "Integral = " << res << endl 
        << "Duration of parallel for loop with reduction = " << end - start << " seconds" << endl;
}


int main(){
    experiment1();
    experiment2();
    experiment3();
    return 0;
}


