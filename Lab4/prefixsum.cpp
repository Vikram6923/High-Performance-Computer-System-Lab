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
    prefixsum =  inplace_prefix_sum(A);
    for (int i = 0; i < n; i++) {
        cout << A[i] << " ";
    }
    cout << endl;
    // print out the array
    for (int i = 0; i < n; i++) {
        cout << prefixsum[i] << " ";
    }
    cout << endl;

    return 0;
}
