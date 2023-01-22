#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

int main(int argc, char* argv[]) {
    int n; // number of elements in the arrays
    int n_t; // number of threads
    int c;

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
    c = n/n_t + (n%n_t!=0);

    vector<int> A(n);

    for (int i = 0; i < n; i++) {
        A[i] = rand() % 20 + 1;
    }

    double start_time = omp_get_wtime();
    omp_set_num_threads(n_t);
    #pragma omp parallel 
    {
        int tid = omp_get_thread_num();
        int start = tid * c;
        int end = start + c;

        // check if this is the last thread
        if (end > n) {
            end = n;
        }

        long long local_sum = 0;
        for (int i = start; i < end; i++) {
            local_sum += A[i];
        }
        sum += local_sum;
    }
    double end_time = omp_get_wtime();

    // print the results
    // for (int i = 0; i < n; i++) {
    //     cout << A[i] << " + " << B[i] << " = " << C[i] << endl;
    // }
    cout << "Time taken : "<<end_time-start_time<<" s" << endl;

    return 0;
}
