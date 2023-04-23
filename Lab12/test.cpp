#include <stdio.h>
#include <omp.h>

void process_item(int item) {
    // printf("Processing item %d, printed by thread %d\n", item, omp_get_thread_num());
    if (item % 2 == 0) {
        
        // printf("Item %d is divisible by 2, printed by thread %d\n", item, omp_get_thread_num());
    }
    else {
        // printf("Item %d is not divisible by 2, printed by thread %d\n", item, omp_get_thread_num());
    }

}

void process_items(int* items, int n) {
    #pragma omp parallel
    {
        int i;
        #pragma omp for
        for (i = 0; i < n; i++) {
            for (int i = 0; i < 100000000; i++) {
                int a = 1;
            }
            printf("Processing item %d, printed by thread %d\n", items[i], omp_get_thread_num());
            #pragma omp taskyield
            #pragma omp critical 
            {
                printf("Processint (2) %d, printed by thread %d\n", items[i], omp_get_thread_num());
                process_item(items[i]);
            }
        }
    }
}

int main() {
    int items[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int n = 10;
    process_items(items, n);

    return 0;
}
