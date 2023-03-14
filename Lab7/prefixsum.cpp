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

int identity(int x) {
    return x;
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


int* prefix_sum(int A[], int n, int (*f1)(int), int (*f2)(int, int)) {
    // Resize the array to the nearest next power of 2
    int a = log2(n);
    // cout << "a: " << a << endl;
    if (pow(2, a) != n)
        A = nearestPowerOfTwo(n, A);
    int log2n = log2(n);
    // cout << "Test: " << log2n << endl;

    int* M = new int[2*n];
    // int* L = new int[n];
    int* R = new int[2* n];
    int* prefixsum = new int[n];

    // Bottom-up phase
    M[0] = 0;
    #pragma omp parallel for schedule(dynamic, 1024)
    for (int i = n; i < 2*n; i++) {
        M[i] = f1( A[i-n] );
    }
    // cout << "M: ";
    // for (auto x: M)
    //     cout << x << " ";
    // cout << endl;

    int offsets[log2n + 1];
    int offset = n;
    for (int i = 0; i <= log2n; i++) {
        offsets[i] = offset;
        offset = offset >> 1;
    }

    for (int r = 1; r <= log2n; r++) {
        #pragma omp parallel for schedule(dynamic, 1024)
        for (int i = offsets[r]; i < offsets[r-1]; i += 2) {
            M[i] = f2(M[2 * i], M[2 * i + 1]);
            M[i + 1] = f2(M[2 * (i + 1)], M[2 * (i + 1) + 1]);
        }
        // for (int i = offset; i < offlim; i++) {
        //     // M[i] = M[2 * i] + M[2 * i + 1];
        //     M[i] = f2(M[2*i], M[2*i + 1]);
        // }
    }
    // cout << "M: ";
    // for (auto x: M)
    //     cout << x << " ";
    // cout << endl;

    // Final phase

    R[0] = R[1] = 0;
    int m = 2;
    for (int r = 2; r <= log2n + 1; r++) {
        #pragma omp parallel for schedule(dynamic, 1024)
        for (int i = m; i < 2 * m; i+=2) {
            R[i] = R[i/2];
            R[i+1] = f2(R[i/2], M[i]);
        }
        m *= 2;
    }
    // cout << "R: ";
    // for (auto x: R)
    //     cout << x << " ";
    // cout << endl;


    #pragma omp parallel for schedule(dynamic, 1024)
    for (int i = 0; i < n; i++) {
        prefixsum[i] = f2(M[i + n], R[i + n]);
    }

    // Resize prefixsum to the original size of A
    // prefixsum.resize(t);
    delete[] M;
    // delete[] L;
    delete[] R;
    return prefixsum;
}


int* sequential_prefix_sum(int A[], int n, int (*f2)(int, int)) {
    int* prefixsum = new int[n];
    prefixsum[0] = A[0];
    for (int i = 1; i < n; i++) {
        prefixsum[i] = prefixsum[i - 1] + A[i];
    }
    return prefixsum;
}

pair<int*, size_t> reduce_array(int A[], int n) {
    int* B = new int[n]();
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        if (A[i] != -1) {
            B[i] = 1;
        }
        else {
            B[i] = 0;
        }
    }
    int* C = prefix_sum(B, n, [](int x) {return x;}, [](int x, int y) {return x + y;});
    // vector<int> reduced_array(C[n - 1]);
    int* reduced_array = new int[C[n - 1]];

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        if (B[i]) {
            reduced_array[C[i] - 1] = A[i];
        }
    }
    return make_pair(reduced_array, C[n-1]);
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
    //     A[i] = rand() % 20 - 10;
    // }
    for (int i = 0; i < n; i++) {
        A[i] = 1;
    }
    if (n <= 256) {
        cout << "A: ";
        for (int i = 0; i < n; i++) {
            cout << A[i] << " ";
        }
        cout << endl;
    }
    cout << "Array is initialized of size " << n << endl;

    int* prefixsum;
    double start_time = omp_get_wtime();
    prefixsum = prefix_sum(A, n, [](int x) {return x;}, [](int x, int y) {return x + y;});
    // pair<int*, size_t> result = reduce_array(A, n);
    // prefixsum = result.first;
    // size_t k = result.second;
    double end_time = omp_get_wtime();

    // cout << "k: " << k << endl;
    // n = k < n ? k : n;

    // print out the array
    if (n <= 256) {
        cout << "B: ";
        for (int i = 0; i < n; i++) {
            cout << prefixsum[i] << " ";
        }
        cout << endl;
    }
    printf("Last number : %d\n", prefixsum[n-1]);
    printf("Time taken : %f s\n", end_time-start_time);
    delete[] A;
    delete[] prefixsum;

    return 0;
}
