#include <iostream>
#include <vector>
#include <omp.h>
#include <math.h>
using namespace std;

// static int glb_tp ;
// #pragma omp threadprivate( glb_tp )
// int main( int arg , char* argv[] )
// {
//     int sh = 11 ;
//     glb_tp = 13;
//     printf( "Hello sh=%d , glb_tp=%d\n" , sh , glb_tp );
//     #pragma omp parallel copyin(glb_tp)
//     {
//         printf( "Hello from %d of %d . sh=%d , glb_tp=%d\n" ,
//         omp_get_thread_num() , omp_get_num_threads() , sh , ++glb_tp );
//     }
//     printf( "Hello sh=%d , glb_tp=%d\n" , sh , glb_tp );
//     return 0 ;
// }

// int main(int argc, char* argv[]) {
//     omp_set_num_threads(4);
//     int numt;
//     #pragma omp parallel default(shared)
//     {
//         int j, tid;
//         #pragma omp master
//         {
//             for ( j = 0 ; j < 10000000 ; j++);
//             numt = omp_get_num_threads();
//         }
//         tid = omp_get_thread_num();
//         printf("Hello World from thread %d of %d\n", tid + 1, numt);
//     }

//     #pragma omp parallel for collapse(2)
//     for (int i = 0; i < 2; i++) {
//         for (int j = 0; j < 4; j++) {
//             int tid = omp_get_thread_num();
//             printf("Hello World from thread %d of \n", tid);
//         }
//     }
//     return 0;
// }

int main(int argc, char* argv[]) {
    int n; // number of elements in the arrays
    int n_t; // number of threads
    long long int sum = 0;

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
    for (int i = 0; i < n; i++) {
        A[i] = rand() % 20 + 1;
    }
    // for (int i = 0; i < n; i++) {
    //     A[i] = 1;
    // }
    omp_set_num_threads(n_t);
    // double tick = omp_get_wtick();
    double start_time = omp_get_wtime();
    #pragma omp parallel 
    {
        int psum = 0;
        #pragma omp for schedule (dynamic, 1024)
        for (int i = 0; i < n; i++) {
            if (A[i] <= 10)
                psum += A[i];
        }
        #pragma omp critical
        sum += psum;
    }
    double end_time = omp_get_wtime();

    // print the results
    printf("Sum: %lld\n", sum);
    printf("Time taken : %lf s\n", end_time-start_time);

    return 0;
}
