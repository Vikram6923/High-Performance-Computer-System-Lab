#include <iostream>
#include <vector>
#include <omp.h>
#include <math.h>
using namespace std;

int main(int argc, char* argv[]) {
    int n;
    int n_t;

    // read in values of n and n_t
    if (argc < 3) {
        cout << "Enter the size of the arrays: ";
        cin >> n;
        cout << "Enter the number of threads: ";
        cin >> n_t;
    } 
    else {
        n = atoi(argv[1]);
        n_t = atoi(argv[2]);
    }
    omp_set_num_threads(n_t);
    // float a,b,c;
    // #pragma omp parallel
    // {
    //     #pragma omp single
    //     // for (int i = 0; i < n; i++) 
    //     {

    //         int task_id = omp_get_thread_num();
    //         #pragma omp task depend(out: a)
    //         {
    //             a = 10;
    //             cout << 'a' << a << 'b' << b << 'c' << c << endl;
    //             // printf("Thread: %d of %d, task_id: %d, i: %d\n", omp_get_thread_num(), omp_get_num_threads(), task_id, i);
    //         }
    //         // #pragma omp taskwait
    //         #pragma omp task depend(in: a) depend (out: b)
    //         {
    //             b = a + 16;
    //             cout << 'a' << a << 'b' << b << 'c' << c << endl;
    //             // printf("THREAD: %d of %d, task_id: %d, i: %d\n", omp_get_thread_num(), omp_get_num_threads(), task_id, i);
    //         }
    //         #pragma omp task depend(in: b)
    //         {
    //             c = b + 12;
    //             cout << 'a' << a << 'b' << b << 'c' << c << endl;
    //         }
    //     }
    //     #pragma omp taskwait
    // }

    int A[2*n];

    for (int i = n; i < 2*n; i++) {
        A[i] = 1;
    }

    int start = n >> 1, end = n;
    #pragma omp parallel shared(A) 
    {
        #pragma omp single 
        {
            while (start < end) 
            {
                for (int i = start; i < end; i++) 
                {
                    #pragma omp task depend(in: A[ 2*i]) depend (in: A[2*i + 1]) depend (out: A[i])
                    {
                        cout << "Calculating A[" << i << "] = A[" << 2*i << "] + A[" << (2*i + 1) << "]"
                        // << " by thread " << omp_get_thread_num() 
                        << endl;
                        A[i] = A[2*i] + A[2*i + 1];
                        // cout << A[i] << endl;
                    }
                }
                start >>= 1;
                end >>= 1;
            }
        }
    }


    A[0] = 0;
    for (auto i : A)
        cout << i << " ";
    cout << endl;

    return 0;
}