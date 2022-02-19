#ifndef MYMALLOC_H_INCLUDED
#define MYMALLOC_H_INCLUDED
#define MEM_SIZE 4096


typedef struct meta_data {
    unsigned short block_size: 12; // block_size = payload_size + header_size
    unsigned short nothing : 3;
    unsigned short free: 1;
    
} t_meta_data;

static char memory[MEM_SIZE]={255};

void *mymalloc(size_t size, char *file, int line);
void myfree(void *ptr, char *file, int line);
void print_heap();
#define malloc(s) mymalloc(s, __FILE__, __LINE__)
#define free(p) myfree(p, __FILE__, __LINE__)


#endif // MYMALLOC_H_INCLUDED
