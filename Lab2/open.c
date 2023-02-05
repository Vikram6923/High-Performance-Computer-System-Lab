#include <omp.h>
#include <stdio.h>

int tid, x;
#pragma omp threadprivate(tid, x)
int main(int argc, char* argv[]) {
    omp_set_num_threads(4);
    int numt;
    int a[4];
    #pragma omp parallel default(shared)
    {
        int j;
        tid = omp_get_thread_num();
        x = 2 * tid;
        // printf("Hello World from thread %d of %d\n", tid + 1, numt);
        if (tid == 0) {
            for ( j = 0 ; j < 10000000 ; j++);
            numt = omp_get_num_threads();
        }
        a[4] = &tid;
    }
    printf("\n");

    #pragma omp parallel 
    {
        printf("from thread %d of %d addresses: A(numt)=%d , A(tid)=%d , x = %d .\n" , tid , numt , &numt , &tid , x );
    }
    printf("\n");
    //  omp_set_num_threads(6);
    #pragma omp parallel
    {
        printf("from thread %d of %d addresses: A(numt)=%d , A(tid)=%d , x = %d .\n" , tid , numt , &numt , &tid , x );
    }

    return 0;
}