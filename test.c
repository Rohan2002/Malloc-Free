#include <stdio.h>
#include <string.h>
#include "mymalloc.h"

int main(int argc, char **argv){
    int SIZE = 5;
    static char mem[5];
    
    memset(mem, 0, 5);
    mem[0] = 1;
    mem[1] = 2;

    char* pointer_to_mem = &mem[0];
    int index = 0;
    while(index != 100)
        pointer_to_mem[index] = 9;
        pointer_to_mem+=2;
        index+=2;
    }

    // char* simple_syntax_pointer_to_mem = mem;
    
    // printf("Simple Syntax Memory Address: %d\n", simple_syntax_pointer_to_mem);
    // printf("Memory Address: %d\n", pointer_to_mem);
    // printf("Value at Memory Address: %d\n", *pointer_to_mem);
    
    // pointer_to_mem+=1;
    // simple_syntax_pointer_to_mem = &mem[1];

    // printf("Simpler Memory Address: %d\n", simple_syntax_pointer_to_mem);
    // printf("Simpler Value at Memory Address: %d\n", *simple_syntax_pointer_to_mem);

    // printf("Memory Address: %d\n", pointer_to_mem);
    // printf("Value at Memory Address: %d\n", *pointer_to_mem);
    
    // printf("Size of the memory %lu\n", sizeof(mem));

    // char* current = mem;
    // t_meta_data* c = (void *)current;

    // printf("Sizeof t_meta_data: %lu", sizeof(t_meta_data));
    // printf("Sizeof c: %lu", sizeof(c));

}