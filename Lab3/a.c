#include <stdio.h>

static int x = 10;

void func();

int main(int argc, char* argv[]) {
    func();
    printf("OUT: %d, ADD %p\n", x, &x);
    x = 7;
    printf("OUT: %d, ADD %p\n", x, &x);
    return 0;
} 