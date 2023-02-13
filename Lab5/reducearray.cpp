#include <iostream>
#include <vector>
#include <omp.h>
#include <math.h>
#include "prefixsum.h"
using namespace std;

vector<int> reduce_array(vector<int> A) {
    int n = A.size();
    vector<int> B(n, 0);

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        if (A[i] != -1) {
            B[i] = 1;
        }
    }

    vector<int> C;
    C = prefix_sum(B);
    vector<int> reduced_array(C[n - 1]);

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        if (B[i]) {
            reduced_array[C[i] - 1] = A[i];
        }
    }
    return reduced_array;
}

int main(int argc, char* argv[]) {
    int n; // size of the array

    vector<int> A = {1, 2, -1, 4, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, 15, -1, 17, -1, 19, 20};
    n = A.size();

    cout << "A: ";
    for (int i = 0; i < n; i++) {
        cout << A[i] << " ";
    }
    cout << endl;

    vector<int> reduced_array;
    reduced_array = reduce_array(A);

    cout << "Reduced array: ";
    for (int i = 0; i < reduced_array.size(); i++) {
        cout << reduced_array[i] << " ";
    }
    cout << endl;

    return 0;
}