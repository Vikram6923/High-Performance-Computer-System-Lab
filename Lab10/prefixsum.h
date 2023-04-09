#include <iostream>
#include <vector>
#include <omp.h>
#include <math.h>
#include <climits>
using namespace std;

vector<int> nearestPowerOfTwo(int& n, vector<int> A) {
    int a = log2(n);
    if (pow(2, a) == n)
        return A;
    n = pow(2, a + 1);
    vector<int> newA(n, 0);
    copy(A.begin(), A.end(), newA.begin());
    return newA;
}

vector<int> prefixsum(vector<int> A, int n, int (*f1)(int), int (*f2)(int, int)) {
    // Resize the array to the nearest next power of 2
    int a = log2(n);
    if (pow(2, a) != n)
        A = nearestPowerOfTwo(n, A);
    int log2n = log2(n);

    vector<int> M(2*n);
    vector<int> R(2*n);
    vector<int> prefixsum(n);

    // Bottom-up phase
    M[0] = 0;
    #pragma omp parallel for schedule(static, 1024)
    for (int i = n; i < 2*n; i++) {
        M[i] = f1( A[i-n] );
    }

    // Declaring offsets
    int offsets[log2n + 2];
    int offset = n << 1;
    for (int i = 0; i <= log2n + 1; i++) {
        offsets[i] = offset >> i;
    }

    for (int r = 1; r <= log2n; r++) {
        #pragma omp parallel for schedule(static, 1024)
        for (int i = offsets[r + 1]; i < offsets[r]; i += 2) {
            M[i] = f2(M[2 * i], M[2 * i + 1]);
            M[i + 1] = f2(M[2 * (i + 1)], M[2 * (i + 1) + 1]);
        }
    }


    // Final phase
    R[0] = R[1] = 0;
    // int m = log2n - 1;
    int m = log2n + 1;
    for (int r = 2; r <= log2n + 1; r++) {
        #pragma omp parallel for schedule(static, 1024)
        for (int i = offsets[m]; i < offsets[m - 1]; i++) {
            R[2*i] = R[i];
            R[2*i + 1] = f2(R[i], M[2*i]);
        }
        m--;
    }

    #pragma omp parallel for schedule(dynamic, 1024)
    for (int i = 0; i < n; i++) {
        prefixsum[i] = f2(M[i + n], R[i + n]);
    }

    return prefixsum;
}
