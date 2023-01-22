#include <omp.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    // omp_set_num_threads(16);
    int numt, tid;

    #pragma omp parallel 
    {
        tid = omp_get_thread_num();
        if (tid == 0)
            numt = omp_get_num_threads();
    }

    #pragma omp parallel //private (tid) shared (numt)
    {
        tid = omp_get_thread_num();

        // for (int j = 0; j < 10000000; j++);
        printf("Hello World from thread %d of %d\n", tid + 1, numt);
    }
    return 0;
}