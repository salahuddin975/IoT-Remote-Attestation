#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>

#include <limits.h>    /* for PAGESIZE */
#ifndef PAGESIZE
#define PAGESIZE 4096
#endif


void main(int argc, char **argv)
{
    char *p;
    unsigned int value; 
    
    scanf("%x", &value);
    
    p = (char *)(((int) value + 4096-1) & ~(4096-1));
    printf("Aligned pos: %p\n", p);
    printf("Aligned pos: %d\n", p);
}

