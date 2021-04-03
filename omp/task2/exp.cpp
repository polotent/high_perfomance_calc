#include <iostream>
#include <omp.h>

using namespace std;

int main()
{
    int arr[3] = {1,2,3};
    int res[3] = {5,6,7};
    int i;
    #pragma omp parallel private(i) shared(arr) num_threads(4)
    {
        #pragma omp for schedule(static) reduction(+:res)
        for (i=0; i<3;i++)
        {
            res[i] += arr[i]; 
        }
    }

    for (int i=0; i<3;i++)
    {
        cout << res[i] << ", " << endl;
    }
    return 0;
}