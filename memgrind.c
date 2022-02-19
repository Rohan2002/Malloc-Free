#include <stdio.h>
#include "mymalloc.h"


int main(){
    char*z = (char*)malloc(sizeof(char));
    *z='c';
    printf("%c\n",*z);
    // int *x = (int*)malloc(sizeof(int)*3);
    // int y=5;
    // free(&y);
    // free(z);
    // char*z2 = (char*)malloc(sizeof(char)*2);
    // free(x+2);
    // free(x);
    print_heap();
}
