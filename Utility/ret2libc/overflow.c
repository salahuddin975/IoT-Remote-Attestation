#include<string.h>
#include<stdio.h>
#include<stdlib.h>


void return_input (char *arg)
{ 
       char array[30]; 
       printf("Addr: %p\n", array);
       strcpy (array, arg); 
       printf("%s\n", array); 
}


int  main(int argc, char **argv) 
{ 
       return_input(argv[1]); 
       return 0; 
}
