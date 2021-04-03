#include <omp.h>
#include <iostream>

using namespace std;

int main(){
    int n = 4;
    cout << "number of threads: " << n << endl;
    #pragma omp parallel num_threads(n)
    {
        #pragma omp for schedule (runtime)
        for (int i=0; i<100; i++){
            #pragma omp critical
            cout << "Thread " << omp_get_thread_num() << " runs i=" << i << endl;
        }
    }
}

