#include <stdio.h>
#include "mymalloc.h"
#include <string.h>

// void meta_data_conversion_short_to_char(short chunksize, short chunkallocation, char *c1, char *c2 ) {
//         short meta_data = chunksize + 32768*chunk_allocation;
//         c1=meta_data >>8;
//         c2=meta_data % 256;
// }

void *mymalloc(size_t requested_size, char *file, int line)
{
    // free = 1, means its block is freed. free = 0 means its block is allocated.
    t_meta_data *initial_header = (void *)memory;
    unsigned short free_bit = 1 << 15;
    if (memory[0] == -1)
    {
        // Memset here is much cleaner: https://en.cppreference.com/w/cpp/string/byte/memset
        memset(memory, 0, MEM_SIZE);

        // Init initial_header with payload_size = 4096, free = 1
        initial_header->block_size = MEM_SIZE;
        initial_header->free = 1;
    }

    char* current_block_pointer = &memory[0]; // base address of memory.
    int index = 0;

    while (index < MEM_SIZE)
    {
        t_meta_data *metadata_of_current_block = (void *)current_block_pointer;

        unsigned short header_size = sizeof(t_meta_data);
        unsigned short current_payload_size = metadata_of_current_block->block_size - header_size;
        unsigned short requested_block_size = requested_size + header_size; // includes the header size with requested_size.

        unsigned short current_block_size = metadata_of_current_block->block_size;

        // same amount of demand 
        if (metadata_of_current_block->free == 1 && requested_block_size == current_block_size)
        {
            // set metadata to allocated.
            metadata_of_current_block->free = 0; 
            metadata_of_current_block->block_size = requested_size;

            // push the current_block_pointer to the start of payload (NOT the header) in the block.
            current_block_pointer += header_size;

            return (void *)current_block_pointer;
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
                metadata_of_current_block = current_block + block1_size

            */

            // Think of it like metadata_of_current_block = metadata_of_current_block->next;
            metadata_of_current_block = current_block_pointer + requested_block_size;

            // Fill header2
            metadata_of_current_block->block_size = current_block_size - requested_block_size;
            metadata_of_current_block->free = 1;

            // push the current_block_pointer to the start of payload (NOT the header) in the block.
            current_block_pointer += header_size;
            return (void *)current_block_pointer;
        }
        else
        {   
            current_block_pointer += current_block_size;
            index += current_block_size;
        }
    }
}

void print_head(){

}
void print_heap()
{
    printf("-------------------------------------\n");
    t_meta_data *meta_data_header = (void *)memory;
    unsigned short free_bit = 1 << 15;

    char *current = memory;
    int number = 0;
    while (current != NULL)
    {
        number++;
        t_meta_data *metadata_of_current_block = (void *)current;
        unsigned short chunk_size = metadata_of_current_block->data;
        unsigned short payload_size = metadata_of_current_block->data - sizeof(t_meta_data);
        unsigned short meta_data_free = 0;
        if (metadata_of_current_block->data >= free_bit)
        {
            meta_data_free = 1;
            payload_size -= free_bit;
            chunk_size -= free_bit;
        }

        printf("CHUNK #%d\n", number);
        printf("chunk encoded data : %hu\n", metadata_of_current_block->data);
        printf("chunk_size : %hu\n", chunk_size);
        printf("payload_size : %hu\n", payload_size);
        printf("is free : %hu\n", meta_data_free);
        printf("current - memory + chunk_size : %d\n", current - memory + chunk_size);

        if (current - memory + chunk_size >= MEM_SIZE)
        {
            current = NULL;
        }
        else
        {
            current += chunk_size;
        }
    }

    printf("-------------------------------------\n");
}

void myfree(void *ptr, char *file, int line)
{
    // memory array is not initialized
    // initlaize it with a free chunk of 4096 bytes
    if (memory[0] == 255)
    {

        // error
    }
    if (ptr < memory || ptr > memory + MEM_SIZE)
    {
        printf("Error: trying to free an invalid address.\n");
        printf("Error in %s at line %d\n", file, line);
        return;
    }
    t_meta_data *meta_data_header = (void *)memory;
    unsigned short free_bit = 1 << 15;

    char *current = memory;
    while (current != NULL)
    {
        if (current > ptr)
        {
            printf("Error: trying to free an invalid address.\n");
            printf("Error in %s at line %d\n", file, line);
            return;
        }
        t_meta_data *metadata_of_current_block = (void *)current;
        unsigned short chunk_size = metadata_of_current_block->data;
        unsigned short payload_size = metadata_of_current_block->data - sizeof(t_meta_data);
        unsigned short meta_data_free = 0;
        if (metadata_of_current_block->data >= free_bit)
        {
            meta_data_free = 1;
            payload_size -= free_bit;
            chunk_size -= free_bit;
        }
        if (current + sizeof(t_meta_data) == ptr)
        {

            if (meta_data_free == 0)
            {
                metadata_of_current_block->data += free_bit;
                return;
            }
            else
            {
                printf("Error: trying to free an already freed address.\n");
                printf("Error in %s at line %d\n", file, line);
                return;
            }
        }

        if (current - memory + chunk_size >= MEM_SIZE)
        {
            current = NULL;
        }
        else
        {
            current += chunk_size;
        }
    }
    if (current > ptr)
    {
        printf("Error: trying to free an invalid address.\n");
        printf("Error in %s at line %d\n", file, line);
        return;
    }
}
