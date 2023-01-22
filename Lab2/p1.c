#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char* argv[]) {
    int n; // number of elements in the arrays
    int n_t; // number of threads
    int* A;
    int* B;
    int* C;

    // read in values of n and n_t
    printf("Enter the size of the arrays: ");
    scanf("%d", &n);
    printf("Enter the number of threads: ");
    scanf("%d", &n_t);

    A = malloc(n * sizeof(int));
    B = malloc(n * sizeof(int));
    C = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        A[i] = rand() % 20 + 1;
        B[i] = rand() % 20 + 1;
    }

    double start_time = omp_get_wtime();
    omp_set_num_threads(n_t);
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        C[i] = A[i] + B[i];
    }
    double end_time = omp_get_wtime();

    // print the results
    // for (int i = 0; i < n; i++) {
    //     printf("%d + %d = %d\n", A[i], B[i], C[i]);
    // }
    printf("Time taken : %f s\n",end_time-start_time);

    // free allocated memory
    free(A);
    free(B);
    free(C);

    return 0;
}