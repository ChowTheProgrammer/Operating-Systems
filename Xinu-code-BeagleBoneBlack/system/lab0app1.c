#include <xinu.h>
#include <stdio.h>

process lab0app1(void) {
    printf("I want to ace all the course and land a job this semester! Processed by process#: %d\n", getpid());
}
