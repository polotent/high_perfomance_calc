#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <omp.h>

using namespace std;

int* MultiplyMatrixByVec(int** m, int* v, 
                         const int& i_dim, const int& j_dim, 
                         const string& distr_type="row",
                         const bool& is_parallel=true, 
                         const int& n_threads=8)
{
    int* res = new int[i_dim];
    for (int i=0; i<i_dim; i++)
    {
        res[i] = 0;
    }

    if (distr_type == "row") 
    {
        int i, j, sum;
        #pragma omp parallel private(i,j,sum) shared(m,v) \
        if(is_parallel) num_threads(n_threads)
        {
            #pragma omp for schedule(static)
            for (i=0; i<i_dim; i++)
            {
                sum = 0;
                for (j=0; j<j_dim; j++)
                {
                    sum += m[i][j] * v[j];
                }
                res[i] = sum;
            }            
        }
    } 
    else if (distr_type == "col")
    {
        int i, j, sum;
        #pragma omp parallel private(i,j,sum) shared(m,v) \
        if(is_parallel) num_threads(n_threads)
        {
            #pragma omp for schedule(static)
            for (j=0; j<j_dim; j++)
            {
                sum = 0;
                for (i=0; i<i_dim; i++)
                {
                    sum += m[i][j] * v[j];
                }
                res[i] = sum;
            }
        }
    }
    // else if (distr_type == "block") 
    // {
    //     int block_size_i = 2;
    //     int block_size_j = 2;
    //     int num_blocks_i = matrix.size() / block_size_i;
    //     int num_blocks_j = matrix.size() / block_size_j;

    //     int b_i, b_j;
    //     vector<int> sum;
    //     // #pragma omp parallel private(b_i,b_j,i,j,sum) shared(matrix,vec,result) \
    //     // if(is_parallel) num_threads(n_threads)
    //     // {
    //     //     #pragma omp for schedule(static) reduction(vec_int_plus : sum)
    //     //     for (b_i=0;b_i<num_blocks_i;b_i++)
    //     //     {
    //     //         for(b_j=0;b_j<num_blocks_j;b_j++)
    //     //         {

    //     //             for(i=0;i<block_size_i;i++)
    //     //             {
    //     //                 sum = vector<int>(matrix.size(), 0);
    //     //                 for (j=0;j<block_size_j;j++)
    //     //                 {
    //     //                     sum[b_i*block_size_i+i] = matrix[b_i*block_size_i+i][b_j*block_size_j+j] * vec[b_j*block_size_j+j];
    //     //                 }
    //     //                 transform(result.begin( ), result.end( ), sum.begin( ), result.begin( ), plus<int>());
    //     //             }
    //     //         }
    //     //     }
    //     // }
    // }
    else 
    {
        throw invalid_argument( "wrong distr_type argument value" );
    }
    return res;
}

void PrintResult(int* res, const int& j_dim)
{
    cout << "result = {";
    for(int j=0; j<j_dim; j++){
        cout << res[j];
        if (j != j_dim - 1)
            cout << ", ";
    }
    cout << "}" << endl;
}


int main()
{
    int i_dim = 10;
    int j_dim = 10;
    int n_threads = 8;

    int** m = new int*[i_dim];
    cout << "m = {";
    for (int i=0; i<i_dim; i++)
    {
        cout << "{";
        m[i] = new int[j_dim];
        for (int j=0; j<j_dim; j++)
        {
            m[i][j] = rand() % 10;
            cout << m[i][j];
            if (j != j_dim - 1) {
                cout << ", ";
            }
        }
        if (i != i_dim - 1)
        {
            cout << "}, ";
        }
        else 
        {
            cout << "}";
        }
        
    }
    cout << "}" << endl;


    cout << "v = {";
    int* v = new int[j_dim];
    for (int j=0; j<j_dim ;j++)
    {
        v[j] = rand() % 10;
        cout << v[j];
        if (j != j_dim - 1)
        {
            cout << ", ";
        }
    }
    cout << "}" << endl;

    int* res1 = MultiplyMatrixByVec(m, v, i_dim, j_dim, "row", false);
    PrintResult(res1, j_dim);

    int* res2 = MultiplyMatrixByVec(m, v, i_dim, j_dim, "row", true, n_threads);
    PrintResult(res2, j_dim);

    int* res3 = MultiplyMatrixByVec(m, v, i_dim, j_dim, "col", false);
    PrintResult(res3, j_dim);

    return 0;
}