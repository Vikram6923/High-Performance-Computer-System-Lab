#include <iostream>
#include <vector>
#include <omp.h>
#include <math.h>
using namespace std;

int add_num(int x, int y) {
    return x + y;
}

int max_num(int x, int y) {
    return x > y ? x : y;
}

int* nearestPowerOfTwo(int &n, int* A) {
    int a = log2(n);
    if (pow(2, a) == n)
        return A;
    n = pow(2, a + 1);
    int* newA = new int[n]();
    std::copy(A, A + n, newA);
    return newA;
}

int* inplace_prefix_sum(int A[], int n) {
    int x = 2;
    // Resize the array to the nearest next power of 2
    A = nearestPowerOfTwo(n, A);

    while (x <= n) {
        #pragma pragma omp parallel for collapse(2)
        for (int i = x - 1; i < n; i += x) {
            for (int j = i - x/2; j < i; j++)
                A[j + 1] += A[i - x/2];
        }
        x = x * 2;
    }
    return A;
}

int* inplace_prefix_sum_p(int A[], int n) {
    int x = 1;
    // Resize the array to the nearest next power of 2
    A = nearestPowerOfTwo(n, A);

    int* B = new int[n];
    std::copy(A, A + n, B);

    while (x <= n) {
        #pragma omp parallel for 
        for (int i = x - 1; i < n; i++) {
            B[i] += A[i - x];
        }
        std::copy(B, B + n, A);
        x = x * 2;
    }
    return A;
}


int* prefix_sum(int A[], int n, int (*func)(int, int)) {
    // Resize the array to the nearest next power of 2
    A = nearestPowerOfTwo(n, A);

    int* M = new int[2*n];
    // int* L = new int[n];
    int* R = new int[2* n];
    int* prefixsum = new int[n];

    // Bottom-up phase
    M[0] = 0;
    #pragma omp parallel for
    for (int i = n; i < 2*n; i++) {
        M[i] = A[i - n];
    }
    // cout << "M: ";
    // for (auto x: M)
    //     cout << x << " ";
    // cout << endl;

    int offset = n;
    for (int r = 1; r <= log2(n); r++) {
        offset /= 2;
        #pragma omp parallel for
        for (int i = offset; i < 2* offset; i++) {
            M[i] = func(M[2*i], M[2*i + 1]);
        }
    }
    // cout << "M: ";
    // for (auto x: M)
    //     cout << x << " ";
    // cout << endl;

    // Top-to-down phase
    // L[0] = 0;
    // #pragma omp parallel for
    // for (int i = 1; i < n; i++) {
    //     L[i] = M[2*i];
    // }
    // cout << "L: ";
    // for (auto x: L)
    //     cout << x << " ";
    // cout << endl;

    // Final phase

    R[0] = 0;
    int m = 1;
    for (int r = 1; r <= log2(n) + 1; r++) {
        int j = 0;
        // #pragma omp parallel for 
        for (int i = m; i < 2 * m; i+=2) {
            R[i] = R[i/2];
            R[i+1] = func(R[i/2], M[i]);
            j++;
        }
        m *= 2;
    }
    // cout << "R: ";
    // for (auto x: R)
    //     cout << x << " ";
    // cout << endl;


    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        prefixsum[i] = func(M[i + n], R[i + n]);
    }

    // Resize prefixsum to the original size of A
    // prefixsum.resize(t);
    delete[] M;
    // delete[] L;
    delete[] R;
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
    omp_set_num_threads(n_t);

    int* A = new int[n]();

    // for (int i = 0; i < n; i++) {
    //     A[i] = rand() % 20 + 1;
    // }
    for (int i = 0; i < n; i++) {
        A[i] = i + 1;
    }
    if (n <= 256) {
        cout << "A: ";
        for (int i = 0; i < n; i++) {
            cout << A[i] << " ";
        }
        cout << endl;
    }

    int* prefixsum;
    double start_time = omp_get_wtime();
    prefixsum = prefix_sum(A, n, add_num);
    double end_time = omp_get_wtime();

    // print out the array
    if (n <= 256) {
        cout << "B: ";
        for (int i = 0; i < n; i++) {
            cout << prefixsum[i] << " ";
        }
        cout << endl;
    }
    printf("Time taken : %f s\n", end_time-start_time);
    delete[] A;
    delete[] prefixsum;

    return 0;
}
