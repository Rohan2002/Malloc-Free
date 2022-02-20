#include <stdio.h>
#include "mymalloc.h"
#include <string.h>

// void meta_data_conversion_short_to_char(short chunksize, short chunkallocation, char *c1, char *c2 ) {
//         short meta_data = chunksize + 32768*chunk_allocation;
//         c1=meta_data >>8;
//         c2=meta_data % 256;
// }

void *old_mymalloc(size_t size, char *file, int line)
{
    // printf("hello malloc: %s %d\n",file,line);
    //  memory array is not initialized
    //  initlaize it with a free chunk of 4096 bytes
    s_meta_data *meta_data_header = (void *)memory;
    unsigned short free_bit = 1 << 15;
    if (memory[0] == -1)
    {
        for (int i = 0; i < MEM_SIZE; i++)
        {
            memory[i] = 0;
        }
        meta_data_header->data = MEM_SIZE + free_bit;
    }

    char *current = memory;
    while (current != NULL)
    {
        s_meta_data *c = (void *)current;
        unsigned short chunk_size = c->data;
        unsigned short payload_size = c->data - sizeof(s_meta_data);
        unsigned short meta_data_free = 0;

        if (c->data >= free_bit)
        {
            meta_data_free = 1;
            payload_size -= free_bit;
            chunk_size -= free_bit;
        }
        if (payload_size == size && meta_data_free == 1)
        { // same amount of demand
            c->data -= free_bit;
            current += sizeof(s_meta_data);
            return (void *)current;
        }
        else if (payload_size >= size + 1 + sizeof(s_meta_data) && meta_data_free == 1)
        { // split case

            unsigned short part1_chunksize = size + sizeof(s_meta_data);
            c->data = part1_chunksize + free_bit * 0;

            unsigned short part2_chunksize = chunk_size - part1_chunksize;
            current += part1_chunksize;
            c = (void *)current;
            printf("selin Address in method of header2 %p\n", c);
            c->data = part2_chunksize + free_bit * 1;
            current -= part1_chunksize;
            current += sizeof(s_meta_data);
            return (void *)current;
        }
        else
        {
            if (current - memory + chunk_size >= MEM_SIZE)
            {
                current = NULL;
            }
            else
            {
                current += chunk_size;
            }
        }
    }
    return NULL;
}

void *new_mymalloc(size_t requested_size, char *file, int line)
{
    if (init == 0)
    {
        t_meta_data *initial_header = (void *)memory;
        if (memory[0] == -1)
        {
            // Memset here is much cleaner: https://en.cppreference.com/w/cpp/string/byte/memset
            memset(memory, 0, MEM_SIZE);

            // Init initial_header with payload_size = 4096, free = 1
            initial_header->block_size = MEM_SIZE;
            initial_header->free = 1;
        }
        init = 1;
    }

    void *current_block_pointer = &memory[0]; // base address of memory.
    int index = 0;

    while (index < MEM_SIZE)
    {
        t_meta_data *metadata_of_current_block = current_block_pointer;

        unsigned short header_size = sizeof(t_meta_data);
        unsigned short requested_block_size = requested_size + header_size; // includes the header size with requested_size.

        unsigned short current_block_size = metadata_of_current_block->block_size;

        // same amount of demand
        if (metadata_of_current_block->free == 1 && requested_block_size == current_block_size)
        {
            // set metadata to allocated.
            metadata_of_current_block->free = 0;
            metadata_of_current_block->block_size = requested_size;

            current_block_pointer += header_size;
            return current_block_pointer;
        }
        // split case
        else if (metadata_of_current_block->free == 1 && requested_block_size < current_block_size)
        {

            // current block is split into two parts block1 and block2.
            // Two headers(header1 and header2) in the free list corrosponding to block1 and block2.

            // block1 and block2
            // |          |
            // header1 -> header2

            // Fill header1
            metadata_of_current_block->block_size = requested_block_size;
            metadata_of_current_block->free = 0;

            /*
                Compute the address of where header2 will be located
                because we don't have a t_meta_data "next" attribute.

                The way I thought about this address computation.
                Let me know what u think Selin.

                current_block
                    - block1 (allocated)
                        - header1
                        - payload1
                    - block2 (free)
                        - header2
                        - payload2

                metadata_of_current_block points to block1. then
                metadata_of_current_block = current_block + block1_size = block2_size

            */

            // Think of it like metadata_of_current_block = metadata_of_current_block->next;
            metadata_of_current_block = current_block_pointer + requested_block_size;

            // Fill header2
            metadata_of_current_block->block_size = current_block_size - requested_block_size;
            metadata_of_current_block->free = 1;

            // push the current_block_pointer to the start of payload (NOT the header) in the block.
            current_block_pointer += header_size;
            return current_block_pointer;
        }
        current_block_pointer += current_block_size;
        index += current_block_size;
    }
    return NULL;
}

void *newheader_mymalloc(size_t requested_size, char *file, int line)
{
    if (init == 0)
    {
        header *initial_header = (void *)memory;
        if (memory[0] == -1)
        {
            // Memset here is much cleaner: https://en.cppreference.com/w/cpp/string/byte/memset
            memset(memory, 0, MEM_SIZE);

            // Init initial_header with payload_size = 4096, free = 1
            initial_header->block_size = MEM_SIZE;
            initial_header->free = 1;

            initial_header->prev_block_size = 0;
            initial_header->prev_free = 1;
        }
        init = 1;
    }

    void *current_block_pointer = &memory[0]; // base address of memory.
    int index = 0;

    unsigned int prev_block_size = 0;
    unsigned int prev_free = 1;

    while (index < MEM_SIZE)
    {
        header *metadata_of_current_block = current_block_pointer;

        unsigned int header_size = sizeof(header);
        unsigned int requested_block_size = requested_size + header_size; // includes the header size with requested_size.

        unsigned int current_block_size = metadata_of_current_block->block_size;
        unsigned int current_block_free = metadata_of_current_block->free;

        // same amount of demand
        if (current_block_free == 1 && requested_block_size == current_block_size)
        {
            // set metadata to allocated.
            metadata_of_current_block->free = 0;
            metadata_of_current_block->block_size = requested_size;

            metadata_of_current_block->prev_block_size = prev_block_size;
            metadata_of_current_block->prev_block_size = prev_free;

            current_block_pointer += header_size;
            return current_block_pointer;
        }
        // split case
        else if (current_block_free == 1 && requested_block_size < current_block_size)
        {

            // current block is split into two parts block1 and block2.
            // Two headers(header1 and header2) in the free list corrosponding to block1 and block2.

            // block1 and block2
            // |          |
            // header1 -> header2

            // Fill header1
            metadata_of_current_block->block_size = requested_block_size;
            metadata_of_current_block->free = 0;

            metadata_of_current_block->prev_block_size = prev_block_size;
            metadata_of_current_block->prev_free = prev_free;
            /*
                Compute the address of where header2 will be located
                because we don't have a t_meta_data "next" attribute.

                The way I thought about this address computation.
                Let me know what u think Selin.

                current_block
                    - block1 (allocated)
                        - header1
                        - payload1
                    - block2 (free)
                        - header2
                        - payload2

                metadata_of_current_block points to block1. then
                metadata_of_current_block = current_block + block1_size = block2_size

            */

            // Think of it like metadata_of_current_block = metadata_of_current_block->next;
            metadata_of_current_block = current_block_pointer + requested_block_size;

            // Fill header2
            metadata_of_current_block->block_size = current_block_size - requested_block_size;
            metadata_of_current_block->free = 1;

            metadata_of_current_block->prev_block_size = requested_block_size;
            metadata_of_current_block->prev_free = 0;

            // push the current_block_pointer to the start of payload (NOT the header) in the block.
            current_block_pointer += header_size;
            return current_block_pointer;
        }

        prev_block_size = current_block_size;
        prev_free = current_block_free;

        current_block_pointer += current_block_size;
        index += current_block_size;
    }
    return NULL;
}

void print_implicit_free_list()
{
    printf("-------------------------------------\n");
    char *current = memory;
    int number = 0;
    int index = 0;
    while (index < MEM_SIZE)
    {
        number++;
        header *metadata_of_current_block = (void *)current;
        unsigned short chunk_size = metadata_of_current_block->block_size;
        unsigned short payload_size = chunk_size - sizeof(header);

        printf("CHUNK #%d\n", number);
        printf("payload_size : %hu\n", payload_size);
        printf("curr block size : %hu\n", chunk_size);
        printf("curr block is free : %s\n", metadata_of_current_block->free == 1 ? "yes" : "no");
        printf("prev block size : %hu\n", metadata_of_current_block->prev_block_size);
        printf("prev block is free : %s\n", metadata_of_current_block->prev_free == 1 ? "yes" : "no");

        printf("pointer of payload of start of the block: %p\n", current);

        index += chunk_size;
        current += chunk_size;
        printf("-------------------------------------\n");
    }

    printf("-------------------------------------\n");
}

void myfree(void *ptr, char *file, int line)
{
}
// void myfree(void *ptr, char *file, int line)
// {
//     // memory array is not initialized
//     // initlaize it with a free chunk of 4096 bytes
//     if (memory[0] == 255)
//     {

//         // error
//     }
//     if (ptr < memory || ptr > memory + MEM_SIZE)
//     {
//         printf("Error: trying to free an invalid address.\n");
//         printf("Error in %s at line %d\n", file, line);
//         return;
//     }
//     t_meta_data *meta_data_header = (void *)memory;
//     unsigned short free_bit = 1 << 15;

//     char *current = memory;
//     while (current != NULL)
//     {
//         if (current > ptr)
//         {
//             printf("Error: trying to free an invalid address.\n");
//             printf("Error in %s at line %d\n", file, line);
//             return;
//         }
//         t_meta_data *metadata_of_current_block = (void *)current;
//         unsigned short chunk_size = metadata_of_current_block->block_size;
//         unsigned short payload_size = metadata_of_current_block->block_size - sizeof(t_meta_data);
//         unsigned short meta_data_free = 0;
//         if (metadata_of_current_block->block_size >= free_bit)
//         {
//             meta_data_free = 1;
//             payload_size -= free_bit;
//             chunk_size -= free_bit;
//         }
//         if (current + sizeof(t_meta_data) == ptr)
//         {

//             if (meta_data_free == 0)
//             {
//                 metadata_of_current_block->block_size += free_bit;
//                 return;
//             }
//             else
//             {
//                 printf("Error: trying to free an already freed address.\n");
//                 printf("Error in %s at line %d\n", file, line);
//                 return;
//             }
//         }

//         if (current - memory + chunk_size >= MEM_SIZE)
//         {
//             current = NULL;
//         }
//         else
//         {
//             current += chunk_size;
//         }
//     }
//     if (current > ptr)
//     {
//         printf("Error: trying to free an invalid address.\n");
//         printf("Error in %s at line %d\n", file, line);
//         return;
//     }
// }
int main(int argc, char **argv)
{
    printf("Memory's base address: %p\n", &memory[0]);
    int *z1 = (int *)nh_malloc(2 * sizeof(int)); // 8. 12
    int *z2 = (int *)nh_malloc(4 * sizeof(int)); // 16, 20
    int *z3 = (int *)nh_malloc(6 * sizeof(int)); // 24, 28
    int *z4 = (int *)nh_malloc(11);              // 24, 28

    *z1 = 10;
    *z2 = 11;
    *z3 = 12;

    printf("Z1: Size %lu and address: %p and value: %d\n", sizeof(z1), z1, *z1);
    printf("Z2: Size %lu and address: %p and value: %d\n", sizeof(z2), z2, *z2);
    printf("Z3: Size %lu and address: %p and value: %d\n", sizeof(z3), z3, *z3);

    print_implicit_free_list();
}