#include <stdio.h>
#include "mymalloc.h"
#include <string.h>
#include <stdbool.h>

int errorNoFree;

void *mymalloc(size_t requested_size, char *file, int line)
{
    if (requested_size == 0)
    {
        return NULL;
    }
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

        initial_header->first_node = 1;
        initial_header->last_node = 1;
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
            // printf("Same amoumt of demand case: %d bytes\n", current_block_size);
            // set metadata to allocated.
            metadata_of_current_block->free = 0;
            current_block_pointer += header_size;

            return current_block_pointer;
        }
        // split case
        else if (current_block_free == 1 && requested_block_size + header_size < current_block_size)
        {

            // current block is split into two parts block1 and block2.
            // Two headers(header1 and header2) in the free list corrosponding to block1 and block2.

            // block1 and block2
            // |          |
            // header1 -> header2

            // Fill header1
            int old_current_block_free_status = metadata_of_current_block->last_node;
            // lastnode: 0

            metadata_of_current_block->block_size = requested_block_size;
            metadata_of_current_block->free = 0;
            metadata_of_current_block->last_node = 0;
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
            void *next_pointer = current_block_pointer + metadata_of_current_block->block_size;
            header *metadata_of_next_block = next_pointer;

            // Fill header2
            metadata_of_next_block->block_size = current_block_size - requested_block_size;
            ;
            metadata_of_next_block->free = 1;

            metadata_of_next_block->last_node = old_current_block_free_status;
            // metadata_of_next_block->last_node = 0;
            metadata_of_next_block->first_node = 0;
            metadata_of_next_block->prev_block_size = requested_block_size;
            metadata_of_next_block->prev_free = 0;
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

void print_node(void *current)
{
    header *metadata_of_current_block = current;
    unsigned short chunk_size = metadata_of_current_block->block_size;
    unsigned short payload_size = chunk_size - sizeof(header);
    printf("payload_size : %hu\n", payload_size);
    printf("curr block size : %hu\n", chunk_size);
    printf("curr block is free : %s\n", metadata_of_current_block->free == 1 ? "yes" : "no");
    printf("prev block size : %d\n", metadata_of_current_block->prev_block_size);
    printf("prev block is free : %s\n", metadata_of_current_block->prev_free == 1 ? "yes" : "no");
    printf("is last node: : %s\n", metadata_of_current_block->last_node == 1 ? "yes" : "no");
    printf("is first node: : %s\n", metadata_of_current_block->first_node == 1 ? "yes" : "no");
}
void print_implicit_free_list()
{
    printf("-------------------------------------\n");
    char *current = &memory[0];
    int number = 0;
    int index = 0;
    while (index < MEM_SIZE)
    {
        number++;
        header *metadata_of_current_block = (void *)current;
        unsigned short chunk_size = metadata_of_current_block->block_size;

        printf("CHUNK #%d\n", number);
        print_node(current);
        printf("Address of block: %p\n", current + sizeof(header));

        index += chunk_size;
        current += chunk_size;
        printf("-------------------------------------\n");
    }

    printf("-------------------------------------\n");
}

void myfree(void *ptr, char *file, int line)
{
    // O(1) loop. Notice it will iterate until MEM_SIZE.
    int mem_index;
    for (mem_index = 0; mem_index < MEM_SIZE; mem_index++)
    {
        void *mem_add = &memory[mem_index];
        if (mem_add == ptr)
        {
            break;
        }
    }

    // handle address not found error.
    if (mem_index == MEM_SIZE)
    {
        printf("Error: trying to free an invalid address %p.\n", &memory[mem_index]);
        printf("Error in %s at line %d\n", file, line);
        errorNoFree = 1;
        return;
    }

    // current node in free list
    void *current_block_pointer = &memory[mem_index] - sizeof(header);
    header *metadata_of_current_block_pointer = current_block_pointer;

    if (metadata_of_current_block_pointer->free)
    {
        printf("Error: trying to free the same block twice at address %p.\n", current_block_pointer);
        printf("Error in %s at line %d\n", file, line);
        errorNoFree = 2;
        return;
    }

    // previous node in free list
    void *prev_block_pointer = current_block_pointer - metadata_of_current_block_pointer->prev_block_size;
    header *metadata_of_prev_block_pointer = prev_block_pointer;

    // next node in free list
    void *next_block_pointer = current_block_pointer + metadata_of_current_block_pointer->block_size;
    header *metadata_of_next_block_pointer = next_block_pointer;

    // if the current pointer is not the first node then prev node exists.
    bool prev_node_exists = metadata_of_current_block_pointer->first_node != 1;

    // if the current pointer is not the last node then next node exists.
    bool next_node_exists = metadata_of_current_block_pointer->last_node != 1;

    if (prev_node_exists && next_node_exists)
    {
        bool prev_block_is_free = metadata_of_prev_block_pointer->free == 1;
        bool next_block_is_free = metadata_of_next_block_pointer->free == 1;

        // Case 1: The previous and next blocks are both allocated. No coalescing.
        if (!prev_block_is_free && !next_block_is_free)
        {
            // printf("In case 1\n");
            metadata_of_current_block_pointer->free = 1;
        }

        // Case 2: The previous block is allocated and the next block is free
        else if (!prev_block_is_free && next_block_is_free)
        {
            // printf("In case 2\n");

            // Take current block and set it to free and update current size with next_free block.
            metadata_of_current_block_pointer->block_size += metadata_of_next_block_pointer->block_size;
            metadata_of_current_block_pointer->free = 1;

            // whatever the last node of next pointer is stored into to the last node of current pointer
            metadata_of_current_block_pointer->last_node = metadata_of_next_block_pointer->last_node;

            // Update prev size of next to next block pointer to current block size and update prev_free status to free.
            // check if next next exists, if so update its prev values
            void *next_to_next_block_pointer = current_block_pointer + metadata_of_current_block_pointer->block_size;
            header *metadata_of_next_to_next_block_pointer = next_to_next_block_pointer;
            metadata_of_next_to_next_block_pointer->prev_block_size = metadata_of_current_block_pointer->block_size;
            metadata_of_next_to_next_block_pointer->prev_free = metadata_of_current_block_pointer->free;
        }
        // Case 3: The previous block is free and the next block is allocated
        else if (prev_block_is_free && !next_block_is_free)
        {
            // printf("In case 3\n");
            // set current block to free
            metadata_of_current_block_pointer->free = 1;

            // combine size: curr block size with prev block size
            metadata_of_prev_block_pointer->block_size += metadata_of_current_block_pointer->block_size;
            // whatever the last node of current pointer is stored into to the last node of prev pointer
            metadata_of_prev_block_pointer->last_node = metadata_of_current_block_pointer->last_node;

            // update prev_size and prev_free status of next_block_pointer
            metadata_of_next_block_pointer->prev_block_size = metadata_of_prev_block_pointer->block_size;
            metadata_of_next_block_pointer->prev_free = metadata_of_prev_block_pointer->free;
        }
        // Case 4: The previous block is free and the next block is free
        else
        {
            // printf("In case 4\n");
            // set current block to free
            metadata_of_current_block_pointer->free = 1;

            // combine size: prev block size with curr block size + next block size.
            metadata_of_prev_block_pointer->block_size += metadata_of_current_block_pointer->block_size + metadata_of_next_block_pointer->block_size;
            // whatever the last node of next pointer is stored into to the last node of prev pointer
            metadata_of_prev_block_pointer->last_node = metadata_of_next_block_pointer->last_node;

            // update next to next prev values
            void *next_to_next_block_pointer = prev_block_pointer + metadata_of_prev_block_pointer->block_size;
            header *metadata_of_next_to_next_block_pointer = next_to_next_block_pointer;
            metadata_of_next_to_next_block_pointer->prev_block_size = metadata_of_prev_block_pointer->block_size;
            metadata_of_next_to_next_block_pointer->prev_free = metadata_of_prev_block_pointer->free;
        }
    }
    else if (prev_node_exists && !next_node_exists)
    {
        // This means we are freeing the last node.

        // printf("Freeing the last node\n");
        bool prev_node_is_free = metadata_of_prev_block_pointer->free == 1;

        metadata_of_current_block_pointer->free = 1;

        if (prev_node_is_free)
        {
            metadata_of_prev_block_pointer->block_size += metadata_of_current_block_pointer->block_size;
            metadata_of_prev_block_pointer->last_node = metadata_of_current_block_pointer->last_node;
        }
    }
    else if (!prev_node_exists && next_node_exists)
    {
        // This means we are freeing the first node.
        // printf("Freeing the first node\n");
        bool next_node_is_free = metadata_of_next_block_pointer->free == 1;

        metadata_of_current_block_pointer->free = 1;

        if (next_node_is_free)
        {
            metadata_of_current_block_pointer->block_size += metadata_of_next_block_pointer->block_size;
            metadata_of_current_block_pointer->last_node = metadata_of_next_block_pointer->last_node;

            void *next_to_next_block_pointer = current_block_pointer + metadata_of_current_block_pointer->block_size;
            header *metadata_of_next_to_next_block_pointer = next_to_next_block_pointer;
            metadata_of_next_to_next_block_pointer->prev_block_size = metadata_of_current_block_pointer->block_size;
            metadata_of_next_to_next_block_pointer->prev_free = metadata_of_current_block_pointer->free;
        }
    }
    else
    {
        // This means that linked list only has one node that is 4096 block size.
        metadata_of_current_block_pointer->free = 1;
    }
    errorNoFree = 0;
}