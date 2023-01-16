#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// global variables
int n; // number of elements in the arrays
int n_t; // number of threads
int c;
int* A;
int* B;
int* C;

// thread function
void* add(void* arg) {
    int start = *((int*) arg);
    int end = start + c;

    // check if this is the last thread
    if (end > n) {
        end = n;
    }

    for (int i = start; i < end; i++) {
        C[i] = A[i] + B[i];
    }

    return NULL;
}

int main(int argc, char* argv[]) {
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

    pthread_t threads[n_t];

    // create variables to store start
    int indices[n_t];

    // create threads
    for (int i = 0; i < n_t; i++) {
        indices[i] = i * c;
        pthread_create(&threads[i], NULL, add, &indices[i]);
    }

    // wait for all threads to complete
    for (int i = 0; i < n_t; i++) {
        pthread_join(threads[i], NULL);
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