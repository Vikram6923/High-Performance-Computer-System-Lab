#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

int main(int argc, char* argv[]) {
    int n; // number of elements in the arrays
    int n_t; // number of threads

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


    vector<int> A(n);
    vector<int> B(n);
    vector<int> C(n);

    for (int i = 0; i < n; i++) {
        A[i] = rand() % 20 + 1;
        B[i] = rand() % 20 + 1;
    }

    double start_time = omp_get_wtime();
    omp_set_num_threads(n_t);
        #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        C[i] = A[i] + B[i];
    }
    double end_time = omp_get_wtime();

    // print the results
    for (int i = 0; i < n; i++) {
        cout << A[i] << " + " << B[i] << " = " << C[i] << endl;
    }
    cout << "Time taken : "<<end_time-start_time<<" s" << endl;

    return 0;
}
