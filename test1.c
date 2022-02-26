#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "mymalloc.h"
#include "test.h"


int main(int argc, char **argv)
{   set_free_stat();
    printf("Printing errorNoFree from test1.c %d\n", errorNoFree);
}