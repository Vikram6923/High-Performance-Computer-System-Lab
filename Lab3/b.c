#include <stdio.h>

static int x = 2;

void func() {
    printf("OUT: %d, ADD %p\n", x, &x);
}
