#include <iostream>
#include <vector>
#include <omp.h>
#include <math.h>
using namespace std;

vector<int> inplace_prefix_sum(vector<int> A, int n) {
    int x = 2;
    int i, j;
    while (x <= n) {
        #pragma pragma omp parallel for collapse(2)
        for ( i = x - 1; i < n; i += x) {
            for (j = i - x/2; j < i; j++) {
                A[j + 1] += A[i - x/2];
                printf("A[%d] += A[%d] by thread: %d\n", j + 1 + 1, i - x/2 + 1, omp_get_thread_num());
            }
        }               
        x = x * 2;
    }
    return A;
}

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
    omp_set_num_threads(n_t);

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("i: %d, j: %d, by thread: %d\n", i, j, omp_get_thread_num());
        }
    }

    vector<int> A(n);
    for (int i = 0; i < n; i++) {
        A[i] = 1;
    }

    vector<int> B = inplace_prefix_sum(A, n);

    for (int i = 0; i < n; i++) {
        cout << B[i] << " ";
    }
    cout << endl;


    return 0;

}