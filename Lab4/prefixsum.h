#include <iostream>
#include <vector>
#include <omp.h>
#include <math.h>
using namespace std;

void nearestPowerOfTwo(int &n, vector<int> &A) {
    int a = log2(n);
    if (pow(2, a) == n)
        return;
    n = pow(2, a + 1);
    A.resize(n, 0);
}

vector<int> prefix_sum(vector<int> A) {
    int n = A.size();
    int t = n;
    // Resize the array to the nearest next power of 2
    nearestPowerOfTwo(n, A);

    vector<int> M(2*n);
    vector<int> L(n);
    vector<int> R(2 * n);
    vector<int> prefixsum(n);

    // Bottom-up phase
    #pragma omp parallel for
    for (int i = n; i < 2*n; i++) {
        M[i] = A[i - n];
    }

    for (int i = n - 1; i > 0; i--) {
        M[i] = M[2*i] + M[2*i + 1];
    }

    // Top-to-bottom phase
    #pragma omp parallel for
    for (int i = 1; i < n; i++) {
        L[i] = M[2*i];
    }

    for (int i = 0; i < 2*n; i++) {
        if (i % 2 == 0) {
            R[i] = R[i/2];
        }
        else {
            R[i] = R[i/2] + L[i/2];
        }
    }

    // Final phase
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        prefixsum[i] = M[i + n] + R[i + n];
    }

    // Resize prefixsum to the original size of A
    prefixsum.resize(t);
    return prefixsum;
}