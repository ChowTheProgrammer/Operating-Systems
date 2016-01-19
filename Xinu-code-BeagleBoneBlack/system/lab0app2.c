#include <xinu.h>
#include <stdio.h>

process lab0app2(void) {
    int a, b;
    a = 1;
    b = 2;
    printf("a / b = %d. Processed by process#: %d\n", a / b, getpid());
}
