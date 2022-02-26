/*

This is just an experiment file with C. 
You can playaround in this file :)

*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "mymalloc.h"
#include "test.h"


int errorNoFree;

void set_free_stat(){
    errorNoFree = 2;
    printf("Printing errorNoFree from test.c %d\n", errorNoFree);
}

// {
//     typedef struct header_metadata
//     {
//         unsigned int block_size : 13;      // block_size = payload_size + header_size
//         unsigned int prev_block_size : 13; // block_size = payload_size + header_size

//         unsigned int nothing : 2;      // ignore this for now.
//         unsigned int prev_nothing : 2; // ignore this for now.

//         unsigned int free : 1;      // free = 1, means its block is freed. free = 0 means its block is allocated.
//         unsigned int prev_free : 1; // free = 1, means its block is freed. free = 0 means its block is allocated.
//     } header;
//     // typedef struct header_metadata
//     // {
//     //     unsigned short block_size : 13; // block_size = payload_size + header_size

//     //     unsigned short nothing : 2; // ignore this for now

//     //     unsigned short free : 1; // free = 1, means its block is freed. free = 0 means its block is allocated.
//     // } header;

//     static char mem[4098]; // mem[0] -> mem[1]
//     memset(mem, 0, 4098);

//     void *pointer_to_mem = &mem[0];
//     unsigned short index = 0;

//     printf("sizeof(header): %lu\n", sizeof(header));
//     while (index < 4096 + sizeof(header))
//     {
//         header *write = pointer_to_mem;
//         write->block_size = index;
//         write->prev_block_size = index;
//         write->free = 0;
//         pointer_to_mem += sizeof(header);
//         index += sizeof(header);
//     }

//     index = 0;
//     pointer_to_mem = &mem[0];

//     while (index < 4096 + sizeof(header))
//     {
//         header *read = pointer_to_mem;
//         printf("print header block_size : %d\n", read->block_size);
//         printf("print header prev_block_size : %d\n", read->prev_block_size);
//         printf("print header free : %d\n", read->free);
//         printf("pointer to memory location %p\n", pointer_to_mem);
//         pointer_to_mem += sizeof(header);
//         index += sizeof(header);
//     }

    // while (index < 4096)
    // {
    //     header *h = pointer_to_mem;

    //     h->block_size = 5 + sizeof(header);
    //     // h->prev_block_size = index;

    //     pointer_to_mem += h->block_size;
    //     index += 1;
    // }

    // index = 0;
    // pointer_to_mem = &mem[0];
    // while (index < 4096)
    // {
    //     header *h = pointer_to_mem;
    //     printf("-------------------------------------\n");
    //     printf("print header block_size : %d\n", h->block_size);
    //     // printf("print header prev_block_size : %d\n", h->prev_block_size);
    //     printf("Add : %p\n", pointer_to_mem);

    //     printf("-------------------------------------\n");
    //     index += 1;
    //     pointer_to_mem += h->block_size;
    // }
// }

// char* simple_syntax_pointer_to_mem = mem; 16843009

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
