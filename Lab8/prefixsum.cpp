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

int red_func(int x) {
    return x != -1 ? 1 : 0;
}

vector<int> nearestPowerOfTwo(int& n, vector<int> A) {
    int a = log2(n);
    if (pow(2, a) == n)
        return A;
    n = pow(2, a + 1);
    vector<int> newA(n, 0);
    copy(A.begin(), A.end(), newA.begin());
    return newA;
}

vector<int> inplace_prefix_sum(vector<int> A, int n, int (*f2)(int, int)) {
    int x = 2;
    // Resize the array to the nearest next power of 2
    A = nearestPowerOfTwo(n, A);

    while (x <= n) {
        #pragma pragma omp parallel for collapse(2)
        for (int i = x - 1; i < n; i += x) {
            for (int j = i - x/2; j < i; j++) {
                // A[j + 1] += A[i - x/2];
                A[j + 1] = f2(A[j + 1], A[i - x/2]);
            }
        }
        x = x * 2;
    }
    return A;
}

vector<int> prefix_sum(vector<int> A, int n, int (*f1)(int), int (*f2)(int, int)) {
    // Resize the array to the nearest next power of 2
    int a = log2(n);
    if (pow(2, a) != n)
        A = nearestPowerOfTwo(n, A);
    int log2n = log2(n);

    vector<int> M(2*n);
    //vector<int> L(n);
    vector<int> R(2*n);
    vector<int> prefixsum(n);

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

    int offsets[log2n + 2];
    int offset = 2*n;
    #pragma omp parallel for schedule(dynamic, 1024)
    for (int i = 0; i <= log2n + 1; i++) {
        offsets[i] = offset;
        offset = offset >> 1;
    }

    // cout << "offsets: ";
    // for (int i = 0; i <= log2n; i++) {
    //     cout << offsets[i] << " ";
    // }
    // cout << endl;

    for (int r = 1; r <= log2n; r++) {
        #pragma omp parallel for schedule(dynamic, 1024)
        for (int i = offsets[r + 1]; i < offsets[r]; i += 2) {
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
    // int m = log2n - 1;
    int m = log2n + 1;
    for (int r = 2; r <= log2n + 1; r++) {
        #pragma omp parallel for schedule(dynamic, 1024)
        for (int i = offsets[m]; i < offsets[m - 1]; i++) {
            R[2*i] = R[i];
            R[2*i + 1] = f2(R[i], M[2*i]);
        }
        // cout << offsets[m] << " ";
        m--;
    }
    // cout << endl;
    // cout << "R: ";
    // for (auto x: R)
    //     cout << x << " ";
    // cout << endl;


    #pragma omp parallel for schedule(dynamic, 1024)
    for (int i = 0; i < n; i++) {
        prefixsum[i] = f2(M[i + n], R[i + n]);
    }

    return prefixsum;
}


vector<int> sequential_prefix_sum(vector<int> A, int n, int (*f2)(int, int)) {
    // int* prefixsum = new int[n];
    vector<int> prefixsum(n);
    prefixsum[0] = A[0];
    for (int i = 1; i < n; i++) {
        prefixsum[i] = f2(prefixsum[i - 1], A[i]);
    }
    return prefixsum;
}

void compact_array(int n) {
    // Initialize array A with random values
    vector<int> A(n);
    for (int i = 0; i < n; i++) {
        A[i] = rand() % 2 - 1;
    }
    for (int i = 0; i < n; i++) {
        if (A[i] != -1) {
            A[i] = rand() % 20;
        }
    }
    if (n <= 256) {
        cout << "A: ";
        for (auto x: A)
            cout << x << " ";
        cout << endl;
    }

    // compact the array A
    double start_time, end_time;
    start_time = omp_get_wtime();
    vector<int> B(n);
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        B[i] = (A[i] != -1) ? 1 : 0;
    }
    vector<int> C = prefix_sum(B, n, [](int x) {return x;}, [](int x, int y) {return x + y;});
    vector<int> compacted_array(C[n - 1]);

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        if (B[i]) {
            compacted_array[C[i] - 1] = A[i];
        }
    }
    end_time = omp_get_wtime();

    cout << "Size of compacted array: " << C[n - 1] << endl;
    if (n <= 256) {
        cout << "compacted array: ";
        for (auto x: compacted_array)
            cout << x << " ";
        cout << endl;
    }
    cout << "Time taken: " << end_time - start_time << " s" << endl;
    
}

void doprefixsum(int n, int option) {
    vector<int> A(n);
    if (option == 1) {
        for (int i = 0; i < n; i++) {
            A[i] = rand() % 20;
        }
    } else if (option == 2) {
        for (int i = 0; i < n; i++) {
            A[i] = 1;
        }
    } else {
        for (int i = 0; i < n; i++) {
            A[i] = i + 1;
        }
    }

    if (n <= 256) {
        cout << "A: ";
        for (auto x: A)
            cout << x << " ";
        cout << endl;
    }

    vector<int> B;
    double start_time, end_time;
    start_time = omp_get_wtime();
    B = prefix_sum(A, n, [](int x) {return x;}, [](int x, int y) {return x + y;});
    // B = inplace_prefix_sum(A, n, [](int x, int y) {return x + y;});
    end_time = omp_get_wtime();
    if (n <= 256) {
        cout << "B: ";
        for (int i = 0; i < n; i++)
            cout << B[i] << " ";
        cout << endl;
    }
    cout << "Last element of B: " << B[n-1] << endl;
    cout << "Time taken : " << end_time - start_time << " s" << endl;
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

    // compact_array(n);
    doprefixsum(n, 2);

    return 0;
}
