#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

int main(int argc, char* argv[]) {
    int n; // number of elements in the arrays
    int n_t; // number of threads
    int c;
    long long int sum = 0;

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
    #pragma omp parallel for reduction( + : sum )
        for (int i = 0; i < n; i++) {
            sum += A[i];
    }
    double end_time = omp_get_wtime();

    // print the results
    printf("Sum: %lld\n", sum);
    printf("Time taken : %f s\n", end_time-start_time);

    return 0;
}
