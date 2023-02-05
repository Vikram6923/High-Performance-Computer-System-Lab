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

vector<int> inplace_prefix_sum(vector<int> A) {
    int n = A.size();
    int x = 2;
    // Resize the array to the nearest next power of 2
    nearestPowerOfTwo(n, A);

    while (x <= n) {
        for (int i = x - 1; i < n; i += x) {
            for (int j = i - x/2; j < i; j++)
                A[j + 1] += A[i - x/2];
        }
        x = x * 2;
    }
    return A;
}

vector<int> inplace_prefix_sum_p(vector<int> A) {
    int n = A.size();
    int x = 1;
    // Resize the array to the nearest next power of 2
    nearestPowerOfTwo(n, A);

    vector<int> B = A;

    while (x <= n) {
        #pragma omp parallel for 
        for (int i = x - 1; i < n; i++) {
            B[i] += A[i - x];
        }
        A = B;
        x = x * 2;
    }
    return A;
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

    // Top-to-down phase
    #pragma omp parallel for
    for (int i = 1; i < n; i++) {
        L[i] = M[2*i];
    }

    // Final phase
    for (int i = 0; i < 2*n; i++) {
        if (i % 2 == 0) {
            R[i] = R[i/2];
        }
        else {
            R[i] = R[i/2] + L[i/2];
        }
    }

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        prefixsum[i] = M[i + n] + R[i + n];
    }

    // Resize prefixsum to the original size of A
    prefixsum.resize(t);
    return prefixsum;
}


vector<int> sequential_prefix_sum(vector<int> A) {
    int n = A.size();
    vector<int> prefixsum(n);
    prefixsum[0] = A[0];
    for (int i = 1; i < n; i++) {
        prefixsum[i] = prefixsum[i - 1] + A[i];
    }
    return prefixsum;
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

    vector<int> A(n);

    // for (int i = 0; i < n; i++) {
    //     A[i] = rand() % 20 + 1;
    // }
    for (int i = 0; i < n; i++) {
        A[i] = i + 1;
    }

    vector<int> prefixsum;
    prefixsum =  prefix_sum(A);
    for (int i = 0; i < n; i++) {
        cout << A[i] << " ";
    }
    cout << endl;
    // print out the array
    for (int i = 0; i < prefixsum.size(); i++) {
        cout << prefixsum[i] << " ";
    }
    cout << endl;

    return 0;
}
