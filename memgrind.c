#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <ctype.h>
#include "mymalloc.h"
#include <stdbool.h>

double time_difference(struct timeval t1, struct timeval t2)
{
    double elapsed;
    elapsed = (double)(t2.tv_usec - t1.tv_usec) / 1000000 + (double)(t2.tv_sec - t1.tv_sec);
    return elapsed;
}

double test_one()
{
    /*
        Test 1:
        Malloc, free, Malloc, free... 120 times
    */
    int i = 0;
    struct timeval t1, t2;
    void *pointers[150];

    gettimeofday(&t1, NULL);

    for (i = 0; i < 150; i++)
    {
        void *add = (void *)malloc(1);

        pointers[i] = add;
        // printf("malloced %p\n", pointers[i]);

        free(pointers[i]);
        // iprintf("freed\n");
    }

    gettimeofday(&t2, NULL);
    return time_difference(t1, t2);
}

double test_two()
{
    /*
    Test 1:
    Malloc 120 times
    Free 120 times
*/
    int i = 0;
    struct timeval t1, t2;
    void *pointers[120];

    gettimeofday(&t1, NULL);

    for (i = 0; i < 120; i++)
    {
        void *add = (void *)malloc(1);
        pointers[i] = add;
        // printf("malloced %p\n", pointers[i]);
    }

    for (i = 0; i < 120; i++)
    {
        free(pointers[i]);
        // printf("freed\n");
    }

    gettimeofday(&t2, NULL);
    return time_difference(t1, t2);
}

double test_three()
{
    /*
        malloc(120)
        malloc(100)
        free(120) // free space of 120 bytes
        malloc()
    */
    struct timeval t1, t2;
    gettimeofday(&t1, NULL);
    for (int i = 120; i >= 1; i /= 2)
    {
        void *add = malloc(i);
        // printf("mallocing %d bytes and the address we get is %p\n", i, add);
        free(add);
        // printf("freeing %d bytes\n", i);
    }
    gettimeofday(&t2, NULL);

    return time_difference(t1, t2);
}
double test_four()
{
    // We are putt
    struct timeval t1, t2;
    gettimeofday(&t1, NULL);
    for (int iteration = 0; iteration < 1; iteration++)
    {
        void *adresses[160];
        int i = 0;
        for (int c = 2048; c >= 8; c = c / 2)
        {
            void* add =  malloc(c - 4);
            if(add != NULL){
                adresses[i] = add;
            }
            i++;
        }
        
        free(adresses[0]);

        // Split the 2048 byte block (Special case of malloc)
        /*
            Available bytes 2048 bytes.

            c - 4.

            c = 8; 8 - 2040             c-4->4
            c = 16; 8 - 16  - 2024      c-4->12
            c = 32; 8 - 16 - 32 - 1968  c-4->28
                                ...     c-4->1020 
        */
        for (int c = 8; c <= 1024; c = c * 2)
        {
            void* add_minus_four = malloc(c - 4);
            if(add_minus_four == NULL){
                printf("We cannot split the 2048 byte anymore. C-4 Error %d bytes\n", c-4);
            }
            else{
                adresses[i] = add_minus_four;
            }
            i++;
            
        }
        print_implicit_free_list();
        
        for (int ii = 1; ii < i; ii++)
        {
            if (adresses[ii] != NULL){
                free(adresses[ii]);
            }
        }    
    }
    gettimeofday(&t2, NULL);
    return time_difference(t1, t2);
}

double test_five()
{
    /*
        Randomly choose between
        • Allocating a 1-byte chunk and storing the pointer in an array
        • Deallocating one of the chunks in the array (if any)
        Repeat until you have called malloc() 120 times,
        then free all remaining allocated chunks.
    */
    int MAX = 120;
    void *pointers[120];
    memset(pointers, 0, MAX);
    struct timeval t1, t2;
    gettimeofday(&t1, NULL);

    int count_malloc = 0;
    int arraysize = 0;

    while (count_malloc < MAX)
    {
        int random_bit = rand() % 2;
        if (random_bit == 1)
        {
            void* add = malloc(1);
            if(add != NULL){
                pointers[arraysize] = add;
                count_malloc++;
                arraysize++;
            }
            else{
                printf("Can't allocate mem");
            }
        }
        else
        {
            if (arraysize > 0)
            {
                int random_index = rand() % (arraysize);
                if (pointers[random_index] != 0)
                {
                    free(pointers[random_index]);
                    pointers[random_index] = 0;
                    
                }
            }
        }
    }
    // free all remaining allocated blocks.
    for (int i = 0; i < MAX; i++)
    {
        if (pointers[i] != 0)
        {
            void* payload_address = pointers[i];
            void* block_address = pointers[i] - sizeof(header);
            header* metadata_to_block_address = block_address; 
            bool block_is_not_free = metadata_to_block_address->free != 1;
            
            if(block_is_not_free){
                printf("----------------------------\n");
                print_node(block_address);
                printf("Before Free: Block address %p and payload adress: %p\n", block_address, payload_address);
                
                free(payload_address); // In the back-end it will compute the block_adress of that payload_address and free that block.
                pointers[i] = 0;
                
                printf("After Free: Block address %p and payload adress: %p\n", block_address, payload_address);
                printf("----------------------------\n");
            }
        }
    }
    gettimeofday(&t2, NULL);
    return time_difference(t1, t2);
}
double test_six(int pattern)
{
    // coalescing test case.
    int random_mem = 4; // rand() % MEM_SIZE;

    // malloc random_mem byte array chunks into 4096 byte space.

    int block_size = random_mem + sizeof(header);
    int number_of_blocks = floor(MEM_SIZE / block_size);

    // printf("The number of blocks are %d\n", number_of_blocks);

    // allocated addresses
    void *pointers[number_of_blocks];

    // Memset here is much cleaner: https://en.cppreference.com/w/cpp/string/byte/memset
    memset(pointers, 0, number_of_blocks);

    // allocate the full memory.
    int index = 0;
    int accumulated_malloc_size = 0;

    // This will store address in pointers array only until pointers[number_of_blocks - 2] because last block may have a higher block size
    // than the previous blocks which have a block size of random_mem + sizeof(header).

    // However, if MEM_SIZE % block_size = 0 then pointers[0,1,2...number_of_blocks-1] will have addresses and every block size will be the same.
    while (1)
    {
        void *malloc_p = (void *)malloc(random_mem);
        if (malloc_p == NULL)
        {
            break;
        }
        pointers[index++] = malloc_p;
        accumulated_malloc_size += block_size;
    }
    int last_block_size = MEM_SIZE - accumulated_malloc_size - sizeof(header); // calculate last block size that is 4096 - accumulated_malloc_size - metadata_size
    void *allocate_last_block = (void *)malloc(last_block_size);
    if (allocate_last_block == NULL)
    {
        // printf("The last block size that will be allocated is %d\n", last_block_size);
    }
    else
    {
        // Store address at pointers[number_of_blocks - 1]
        pointers[index] = allocate_last_block;
    }

    struct timeval t1, t2;
    gettimeofday(&t1, NULL);

    // free the full memory.
    if (pattern == 1)
    {
        /*
            Pattern 1.
            Free bit status for sample memory: [0,0,0,0,0,0]
            [0,0,0,1,0,0]
            1. Free the middle bit. [0,0,0,1,0,0] (i.e case 1)
            2. Free all left bit. (i.e case 2)
                - [0,0,0,1,0,0] -> [1,1,1,1,0,0]
            3. Free all the right bit. (i.e case 3)
                - [0,0,1,0,0] -> [0,0,1,1,1]
        */
        // Case1;
        int midpoint = number_of_blocks / 2;
        free(pointers[midpoint]);
        pointers[midpoint] = 0;
        // Case 2;
        for (int i = midpoint - 1; i >= 0; i--)
        {
            free(pointers[i]);
            pointers[i] = 0;
        }
        // Case 3;
        for (int i = midpoint + 1; i < number_of_blocks; i++)
        {
            free(pointers[i]);
            pointers[i] = 0;
        }
    }
    else if (pattern == 2)
    {

        /*
            Pattern 2.
            Free bit status for sample memory: [0,0,0,0,0,0]
            1. Free every alternate bit (i.e case 1)
                - [1,0,1,0,1,0]
            2. [1,0,1,0,1,0]
                - [1, 0, 1, 0] (Case 4)
                - [1, 0] (Freeing last node) bc next node doesn't exist.
                - [1]
        */
        // Case1;
        for (int i = 0; i < number_of_blocks; i++)
        {
            if (i % 2 == 0)
            {
                free(pointers[i]);
                pointers[i] = 0;
            }
        }
        // Case4;
        for (int i = 0; i < number_of_blocks; i++)
        {
            if (i % 2 == 1)
            {
                free(pointers[i]);
                pointers[i] = 0;
            }
        }
    }
    else if (pattern == 3)
    {
        /*
            Pattern 3.
            Free bit status for sample memory: [0,0,0,0,0,0]
            1. Free every non-adjancent bit with random spacing (Previous pattern had spacing as 1 because [1,0,1,0...]) (i.e case 1)
                - [1,0,1,0,0,1,0,1,0,0,0,1]
        */
        // Case1;
        int i = 0;
        while (i < number_of_blocks)
        {
            int spacing = (rand() % 5) + 2; // +2 to avoid adjacent random numbers
            free(pointers[i]);
            pointers[i] = 0;
            i += spacing;
        }

        // Case2, Case3, Case4;
        for (int i = 0; i < number_of_blocks; i++)
        {
            if (pointers[i] != 0)
            {
                free(pointers[i]);
                pointers[i] = 0;
            }
        }
    }
    gettimeofday(&t2, NULL);

    return time_difference(t1, t2);
}

int main(int argv, char **argc)
{
    // average time

    double time_one = 0;
    double time_two = 0;
    double time_three = 0;
    double time_four = 0;
    double time_five = 0;
    double time_six_one = 0;
    double time_six_two = 0;
    double time_six_three = 0;

    int trials = 1;

    // for(int i = 0; i < trials; i++){
    //     time_one += test_one();
    // }
    // for(int i = 0; i < trials; i++){
    //     time_two += test_two();
    // }
    // for(int i = 0; i < trials; i++){
    //     time_three += test_three();
    // }
    for(int i = 0; i < trials; i++){
        time_four += test_four();
    }
    // for (int i = 0; i < trials; i++){
    //     time_five += test_five();
    // }
    // for (int i = 0; i < trials; i++)
    // {
    //     time_six_one += test_six(1);
    //     time_six_two += test_six(2);
    //     time_six_three += test_six(3);
    // }
     print_implicit_free_list();
    printf("Time it took to run test_one: %f seconds\n", time_one / trials);
    printf("Time it took to run test_two: %f seconds\n", time_two / trials);
    printf("Time it took to run test_three: %f seconds\n", time_three / trials);
    printf("Time it took to run test_four: %f seconds\n", time_four / trials);
    printf("Time it took to run test_five: %f seconds\n", time_five / trials);
    printf("Time it took to run test_six pattern 1: %f seconds\n", time_six_one / trials);
    printf("Time it took to run test_six pattern 2: %f seconds\n", time_six_two / trials);
    printf("Time it took to run test_six pattern 3: %f seconds\n", time_six_three / trials);

    return EXIT_SUCCESS;
}