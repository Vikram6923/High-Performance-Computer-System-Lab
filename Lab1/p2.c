#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// global variables
int n; // number of elements in the arrays
int n_t; // number of threads
int c;
int* A;
long long int sum;

// thread function
void* add(void* arg) {
    int start = *((int*) arg);
    int end = start + c;

    // check if this is the last thread
    if (end > n) {
        end = n;
    }

    long long local_sum = 0;
    for (int i = start; i < end; i++) {
        local_sum += A[i];
    }

    return (void*) local_sum;
}

int main(int argc, char* argv[]) {
    printf("Enter the size of the arrays: ");
    scanf("%d", &n);
    printf("Enter the number of threads: ");
    scanf("%d", &n_t);
    c = n/n_t + (n%n_t!=0);

    A = malloc(n * sizeof(int));

    // generate random values for A and B
    for (int i = 0; i < n; i++) {
        A[i] = rand() % 20 + 1;
    }
    pthread_t threads[n_t];

    // create variables to store start
    int indices[n_t];

    // create threads
    for (int i = 0; i < n_t; i++) {
        indices[i] = i * c;
        pthread_create(&threads[i], NULL, add, &indices[i]);
    }

    // wait for all threads to complete and sum their local sums
    for (int i = 0; i < n_t; i++) {
        long long local_sum;
        pthread_join(threads[i], (void**) &local_sum);
        sum += local_sum;
    }

    // print the result
    printf("Sum: %lld\n", sum);

    // free allocated memory
    free(A);

    return 0;
}