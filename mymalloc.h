#ifndef MYMALLOC_H_INCLUDED
#define MYMALLOC_H_INCLUDED
#define MEM_SIZE 4096

// 4 bytes
typedef struct header_metadata
{
    unsigned int block_size : 13;      // block_size = payload_size + header_size
    unsigned int prev_block_size : 13; // block_size = payload_size + header_size

    unsigned int last_node : 1;    // 1 = last_node or 0 = not last_node.
    unsigned int first_node : 1;   // 1 = first_node or 0 = not first_node.
    unsigned int prev_nothing : 2; // ignore this for now.

    unsigned int free : 1;      // free = 1, means its block is freed. free = 0 means its block is allocated.
    unsigned int prev_free : 1; // free = 1, means its block is freed. free = 0 means its block is allocated.
} header;

static int init = 0;
static char memory[MEM_SIZE] = {255};

void *mymalloc(size_t size, char *file, int line);
void myfree(void *ptr, char *file, int line);
void print_implicit_free_list();
#define malloc(s) mymalloc(s, __FILE__, __LINE__)
#define free(p) myfree(p, __FILE__, __LINE__)

#endif // MYMALLOC_H_INCLUDED
