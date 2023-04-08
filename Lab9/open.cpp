#include <iostream>
#include <vector>
#include <omp.h>
#include <math.h>
using namespace std;

void prefixsum(int n) {
    double start_time, end_time;
    start_time = omp_get_wtime();

    int M[2*n];

    #pragma omp parallel for
    for (int i = n; i < 2*n; i++) {
        M[i] = 1;
    }

    int start = n >> 1, end = n;
    #pragma omp parallel shared(M) 
    {
        #pragma omp single 
        {
            while (start < end) 
            {
                for (int i = start; i < end; i++) 
                {
                    #pragma omp task depend(in: M[ 2*i]) depend (in: M[2*i + 1]) depend (out: M[i])
                    {
                        // cout << "Calculating M[" << i << "] = M[" << 2*i << "] + M[" << (2*i + 1) << "]"
                        // << " by thread " << omp_get_thread_num() 
                        // << endl;
                        M[i] = M[2*i] + M[2*i + 1];
                        // cout << A[i] << endl;
                    }
                }
                start >>= 1;
                end >>= 1;
            }
        }
    }


    M[0] = 0;
    double middle_time = omp_get_wtime();
    // for (auto i : M)
    //     cout << i << " ";
    // cout << endl;


    int R[2*n];
    R[0] = R[1] = 0;
    int log2n = log2(n);
    int m = log2n + 1;

    int offsets[log2n + 2];
    int offset = 2*n;
    #pragma omp parallel for schedule(dynamic, 1024)
    for (int i = 0; i <= log2n + 1; i++) {
        offsets[i] = offset;
        offset = offset >> 1;
    }

    for (int r = 2; r <= log2n + 1; r++) {
        int start = offsets[m];
        int end = offsets[m - 1];

        #pragma omp parallel shared(R, M)
        {
            #pragma omp single
            {
                for (int i = start; i < end; i++) {
                    #pragma omp task depend(in: R[i], M[2*i]) depend(out: R[2*i], R[2*i+1])
                    {
                        R[2*i] = R[i];
                        R[2*i + 1] = R[i] + M[2*i];
                    }
                }
            }
        }

        m--;
    }

    int prefixsum[n];

    #pragma omp parallel for schedule(dynamic, 1024)
    for (int i = 0; i < n; i++) {
        prefixsum[i] = (M[i + n] + R[i + n]);
    }
    
    end_time = omp_get_wtime();


    if (n <= 100){
        cout << "Prefix sum: ";
        for (auto i : prefixsum)
            cout << i << " ";
        cout << endl;
    }
    cout << "Prefix sum of last element: " << prefixsum[n-1] << endl;
    cout << "Time taken for M: " << middle_time - start_time << endl;
    cout << "Time taken: " << end_time - start_time << endl;
}

void prefix_sum(int* input_array, int n){
    double start_time, end_time;
    start_time = omp_get_wtime();
    int *M = new int[n*2];
    int *L = new int[n];
    int *R = new int[2*n + 1];
    
    #pragma omp parallel
    {
        #pragma omp single
        {
            for (int i = n - 1; i < 2*n - 1; i++){
                #pragma omp task depend(out: *(M + i))
                {
                    M[i] = input_array[i - n + 1];
                    // printf("M[%d] is calculated by thread: %d\n", i, omp_get_thread_num());      
                }
            }
            
            for (int i = n - 2; i >= 0; i--){
                #pragma omp task depend(in: *(M + (i+1)*2), *(M + (i+1)*2 - 1)) depend(out: *(M + i))
                {
                    M[i] = M[(i + 1)*2] + M[(i + 1)*2 - 1];   
                    // printf("M[%d] is calculated by thread: %d\n", i, omp_get_thread_num());             
                }
            }
            
            for (int i = n - 2; i >= 0; i--){
                #pragma omp task depend(in: *(M + (i+1)*2 - 1)) depend(out: *(L + i))
                {
                    L[i] = M[(i + 1)*2 - 1];
                }
            }
            
            #pragma omp task depend(out: *R, *(R + 1))
            {
                R[0] = 0;
                R[1] = 0;
            }
            
            for (int i = 0; i < n; i++){
                #pragma omp task depend(in: *(R + i)) depend(out: *(R + (i + 1)*2 - 1))
                {
                    R[(i + 1)*2 - 1] = R[i];
                }
                
                #pragma omp task depend(in: *(R + i), *(L + i)) depend(out: *(R + (i + 1)*2))
                {
                    R[(i + 1)*2] = R[i] + L[i];
                }
            }
        }
        #pragma omp taskwait
    }
    int* prefixsum = new int[n];
    for (int i = n - 1; i < 2*n - 1; i++) {
        prefixsum[i - n + 1] = M[i] + R[i];
    }
    end_time = omp_get_wtime();
    
    // Print the results
    // cout << "Prefix sum of the input array: ";
    // for (int i = 0; i < 2*n; i++){
    //     cout << R[i] << " ";
    // }
    // cout << endl;
    
    // cout << "M array: ";
    // for (int i = 0; i < 2*n; i++){
    //     cout << M[i] << " ";
    // }
    // cout << endl;
    
    // cout << "L array: ";
    // for (int i = 0; i < n; i++){
    //     cout << L[i] << " ";
    // }
    // cout << endl;
    
    if (n <= 100) {
        cout << "Prefix sum: ";
        for (int i = 0; i < n; i++)
            cout << prefixsum[i] << " ";
    }
    cout << endl;
    cout << "Prefix sum of last element: " << prefixsum[n-1] << endl;
    cout << "Time taken: " << end_time - start_time << endl;
}



int main(int argc, char* argv[]) {
    int n;
    int n_t;

    // read in values of n and n_t
    if (argc < 3) {
        cout << "Enter the size of the arrays:ws ";
        cin >> n;
        cout << "Enter the number of threads: ";
        cin >> n_t;
    } 
    else {
        n = atoi(argv[1]);
        n_t = atoi(argv[2]);
    }
    omp_set_num_threads(n_t);

    int* input_array = new int[n];
    for (int i = 0; i < n; i++) 
        input_array[i] = 1;
    
    prefixsum(n);

    return 0;
}