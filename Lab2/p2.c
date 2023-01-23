#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char* argv[]) {
    int n; // number of elements in the arrays
    int n_t; // number of threads
    int c;
    int* A;
    long long int sum = 0;

    // read in values of n and n_t
    printf("Enter the size of the arrays: ");
    scanf("%d", &n);
    printf("Enter the number of threads: ");
    scanf("%d", &n_t);
    c = n/n_t + (n%n_t!=0);

    A = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        A[i] = rand() % 20 + 1;
    }

    double start_time = omp_get_wtime();
    omp_set_num_threads(n_t);
    #pragma omp parallel for reduction( + : sum )
        for (int i = 0; i < n; i++) {
            sum += A[i];
    }

    double end_time = omp_get_wtime();

    // print the results
    printf("Sum: %lld\n", sum);
    printf("Time taken : %f s\n", end_time-start_time);

    // free allocated memory
    free(A);

    return 0;
}