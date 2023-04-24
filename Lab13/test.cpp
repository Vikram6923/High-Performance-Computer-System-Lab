#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>

using namespace std;
#include <omp.h>

int num_threads = 4;
vector<vector<pair<int, int>>> tiles(num_threads);

void func1(int A[10][32]) {
    #pragma omp parallel for
    #pragma omp tile sizes(5,16)
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 32; ++j) {
            A[i][j] = i*1000 + j;
            printf("A[%d][%d],  printed by thread %d\n", i, j, omp_get_thread_num());
            tiles[omp_get_thread_num()].push_back(make_pair(i, j));
        }
    }
}


int main() {
    int A[10][32];
    omp_set_num_threads(num_threads);

    



    func1(A);
    int t = 0;
    for (int i = 0; i < num_threads; ++i) {
        printf("Thread %d: \n", i);
        for (int j = 0; j < tiles[i].size(); ++j) {
            t++;
            printf("(%d, %d) ", tiles[i][j].first, tiles[i][j].second);
            if (t % 5 == 0) {
                printf("\n");
            }
        }
        printf("\n");
    }

    for (int i = 0; i < num_threads; ++i) {
        printf("Thread %d: ", i);
        printf("size: %ld\n", tiles[i].size());
        printf("\n");
    }


    return 0;
}
