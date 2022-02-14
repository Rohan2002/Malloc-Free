#include <stdio.h>
#include "mymalloc.h"


//void meta_data_conversion_short_to_char(short chunksize, short chunkallocation, char *c1, char *c2 ) {
//        short meta_data = chunksize + 32768*chunk_allocation;
//        c1=meta_data >>8;
//        c2=meta_data % 256;
//}

void *mymalloc(size_t size, char *file, int line){
    //printf("hello malloc: %s %d\n",file,line);
    // memory array is not initialized
    // initlaize it with a free chunk of 4096 bytes
    t_meta_data *meta_data_header = (void*)memory;
    unsigned short free_bit = 1 << 15;
    if (memory[0] == -1) {
        for (int i=0;i<MEM_SIZE; i++) {
            memory[i]=0;
        }
        meta_data_header->data = MEM_SIZE + free_bit;
    }

    char *current = memory;
    while (current!=NULL) {
        t_meta_data *c = (void*)current;
        unsigned short chunk_size = c->data ;
        unsigned short payload_size = c->data - sizeof(t_meta_data );
        unsigned short meta_data_free=0;

        if (c->data >= free_bit ) {
            meta_data_free= 1;
            payload_size -=free_bit ;
            chunk_size -=free_bit ;
        }
        if ( payload_size == size && meta_data_free==1) { // same amount of demand
            c->data -=free_bit;
            current+=sizeof(t_meta_data );
            return (void *)current;

        } else if (payload_size >= size + 1 + sizeof(t_meta_data ) && meta_data_free==1) { // split case
            unsigned short part1_chunksize = size + sizeof(t_meta_data );
            c->data = part1_chunksize ;

            unsigned short part2_chunksize = chunk_size -part1_chunksize;
            current += part1_chunksize;
            c = (void*)current;
            c->data = part2_chunksize + free_bit;
            current -= part1_chunksize;
            current+=sizeof(t_meta_data );
            return (void *)current;

        } else {
            if (  current - memory + chunk_size >=MEM_SIZE ) {
                current=NULL;
            } else {
                current+=chunk_size ;
            }
        }
    }
}

void print_heap(){
    printf("-------------------------------------\n");
    t_meta_data *meta_data_header = (void*)memory;
    unsigned short free_bit = 1 << 15;


    char *current = memory;
    int number = 0;
    while (current!=NULL) {
            number++;
        t_meta_data *c = (void*)current;
        unsigned short chunk_size = c->data ;
        unsigned short payload_size = c->data - sizeof(t_meta_data );
        unsigned short meta_data_free=0;

                printf("CHUNK #%d\n",number);
        printf("chunk encoded data : %hu\n",c->data);
        printf("chunk_size : %hu\n",chunk_size );
        printf("payload_size : %hu\n",payload_size );
        printf("is free : %hu\n",meta_data_free );

            if (  current - memory + chunk_size >=MEM_SIZE ) {
                current=NULL;
            } else {
                current+=chunk_size ;
            }

    }

    printf("-------------------------------------\n");
}

void myfree(void *ptr, char *file, int line){
    printf("hello free\n");
        // memory array is not initialized
    // initlaize it with a free chunk of 4096 bytes
    if (memory[0] == 255) {

        char c1,c2;
        short chunksize = MEM_SIZE;
        short chunk_allocation = 0;
        short meta_data = chunksize + 32768*chunk_allocation;
        memory[0]=meta_data >>8;
        memory[1]=meta_data % 256;
        for (int i=2;i<MEM_SIZE; i++){
            memory[i]=0;
        }
    }
}

int main(){
    print_heap();
    mymalloc(20,"df",23);
    print_heap();
}

