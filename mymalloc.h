#ifndef MYMALLOC_H_INCLUDED
#define MYMALLOC_H_INCLUDED
#define MEM_SIZE 4096

typedef struct meta_data
{
    unsigned short block_size : 13; // block_size = payload_size + header_size
    unsigned short nothing : 2;     // ignore this for now.
    unsigned short free : 1;        // free = 1, means its block is freed. free = 0 means its block is allocated.

} t_meta_data;

typedef struct header_metadata
{
    unsigned int block_size : 13;      // block_size = payload_size + header_size
    unsigned int prev_block_size : 13; // block_size = payload_size + header_size

    unsigned int last_node : 2;      // 1 = last_node or 0 = not last_node.
    unsigned int prev_nothing : 2; // ignore this for now.

    unsigned int free : 1;      // free = 1, means its block is freed. free = 0 means its block is allocated.
    unsigned int prev_free : 1; // free = 1, means its block is freed. free = 0 means its block is allocated.
} header;

typedef struct struct_meta_data
{
    unsigned short data;

} s_meta_data;

static int init = 0;
static char memory[MEM_SIZE] = {255};

void *mymalloc(size_t size, char *file, int line);
void myfree(void *ptr, char *file, int line);
void print_implicit_free_list();
#define malloc(s) mymalloc(s, __FILE__, __LINE__)
#define free(p) myfree(p, __FILE__, __LINE__)

#endif // MYMALLOC_H_INCLUDED
