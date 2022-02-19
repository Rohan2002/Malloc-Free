#ifndef MYMALLOC_H_INCLUDED
#define MYMALLOC_H_INCLUDED
#define MEM_SIZE 4096

typedef struct meta_data
{
    unsigned short block_size : 13; // block_size = payload_size + header_size
    unsigned short nothing : 2;     // ignore this for now.
    unsigned short free : 1;        // free = 1, means its block is freed. free = 0 means its block is allocated.

} t_meta_data;

typedef struct struct_meta_data
{
    unsigned short data;

} s_meta_data;

static int init = 0;
static char memory[MEM_SIZE] = {255};

void *mymalloc(size_t size, char *file, int line);
void myfree(void *ptr, char *file, int line);
void print_implicit_free_list();
#define o_malloc(s) old_mymalloc(s, __FILE__, __LINE__)
#define n_malloc(s) new_mymalloc(s, __FILE__, __LINE__)
#define free(p) myfree(p, __FILE__, __LINE__)

#endif // MYMALLOC_H_INCLUDED
