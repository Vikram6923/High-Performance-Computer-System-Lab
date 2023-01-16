#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// global variables

int main(int argc, char* argv[]) {
    int n; // number of elements in the arrays
    int n_t; // number of threads
    int c;
    int* A;
    int* B;
    int* C;

    // read in values of n and n_t
    printf("Enter the size of the arrays: ");
    scanf("%d", &n);
    printf("Enter the number of threads: ");
    scanf("%d", &n_t);
    c = n/n_t + (n%n_t!=0);

    A = malloc(n * sizeof(int));
    B = malloc(n * sizeof(int));
    C = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        A[i] = rand() % 20 + 1;
        B[i] = rand() % 20 + 1;
    }

    #pragma omp for
    {
        for (int i = 0; i < n; i++) {
            C[i] = A[i] + B[i];
        }
    }

    // print the results
    for (int i = 0; i < n; i++) {
        printf("%d + %d = %d\n", A[i], B[i], C[i]);
    }

    // free allocated memory
    free(A);
    free(B);
    free(C);

    return 0;
}