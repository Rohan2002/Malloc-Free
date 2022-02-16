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

    // is this robust? can we guarantee an empty array will have -1 has the starting index.
    // MEM_SIZE = 4096
    if (memory[0] == -1) {
        // All values in memory[i] = 0
        for (int i=0;i<MEM_SIZE; i++) {
            memory[i]=0;
        }
        
        // data = 2^12 + 2^15 >= 2^15
        meta_data_header->data = MEM_SIZE + free_bit;
    }
    printf("meta_data_header->data is %hu\n",meta_data_header->data);
    char *current = memory;
    while (current!=NULL) {
        t_meta_data *c = (void*)current;
        unsigned short chunk_size = c->data ; // 2^12+2^15 = 36864
        unsigned short payload_size = c->data - sizeof(t_meta_data ); // (2^12+2^15) - 2 = 36862
        unsigned short meta_data_free=0; // 0 = Allocated, 1  = freed
        
        // 2^15 + 2^12 >= 2^15
        // Inititally c->data is always 36864 to this point
        // payload = payload - 2^15
        // chunk = chunk - 2^15
        if (c->data >= free_bit ) {
            printf("Size of c->data %hu\n", c->data);
            printf("Size of payload size %hu\n", payload_size);
            meta_data_free= 1; // why are we freeing?
            payload_size -=free_bit ; // 4096 - 2 = 4094
            chunk_size -=free_bit ; // 4096
            printf("Size of payload after free %hu\n", payload_size);
            printf("Size of chunk after free %hu\n", chunk_size);
        }

        int size_of_true_chunk = size + 1 + sizeof(t_meta_data );
        printf("Size of true chunk = %d\n", size_of_true_chunk); // 23
        if ( payload_size == size && meta_data_free==1) { // same amount of demand

            /*
                int* alloc_int = malloc(5); //
            
            */
            c->data -=free_bit; 
            current+=sizeof(t_meta_data ); // memory[2]
            return (void *)current;

        } else if (payload_size >= size + 1 + sizeof(t_meta_data ) && meta_data_free==1) { // split case
            unsigned short part1_chunksize = size + sizeof(t_meta_data );
            c->data = part1_chunksize ;
            printf("c->data allocation: %d\n", c->data);
            unsigned short part2_chunksize = chunk_size -part1_chunksize;
            printf("free chunk %d\n", part2_chunksize);
            printf("b4 current value: %c\n", current[0]);
            current += part1_chunksize;
            printf("after current value: %c\n", current[0]);
            c = (void*)current;
            c->data = part2_chunksize;//  + free_bit;
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
    return current; // idk if it will reach here..
}

void print_heap(){
    printf("-------------------------------------\n");
    // t_meta_data *meta_data_header = (void*)memory;
    // unsigned short free_bit = 1 << 15;


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

// void myfree(void *ptr, char *file, int line){
//     printf("hello free\n");
//         // memory array is not initialized
//     // initlaize it with a free chunk of 4096 bytes
//     if (memory[0] == 255) {

//         char c1,c2;
//         short chunksize = MEM_SIZE;
//         short chunk_allocation = 0;
//         short meta_data = chunksize + 32768*chunk_allocation;
//         memory[0]=meta_data >>8;
//         memory[1]=meta_data % 256;
//         for (int i=2;i<MEM_SIZE; i++){
//             memory[i]=0;
//         }
//     }
// }

int main(){
    // print_heap();
    mymalloc(20,"df",23);
    // print_heap();
}

