#include <stdlib.h>
#include <stdio.h>

typedef struct meta_data {
    unsigned short data;
} t_meta_data;

int main(int argv, char **argc){
    static char memory[2]={'a', 'b'};

    char* current = memory;
    t_meta_data *meta_data_header = (t_meta_data*)memory;
    
    // printf("%c\n", memory[0]);
    // printf("%c\n", memory[1]);
     // printf("%c\n", memory[2]);
    current+=2;
    printf("Array %c\n", current[0]); 

    printf("%hu\n", meta_data_header->data);
}