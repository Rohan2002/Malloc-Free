#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <ctype.h>
#include "mymalloc.h"

unsigned int DEBUG = 1;

double time_difference(struct timeval t1, struct timeval t2)
{
    double elapsed;
    elapsed = (double)(t2.tv_usec - t1.tv_usec) / 1000000 + (double)(t2.tv_sec - t1.tv_sec);
    return elapsed;
}

double test_one()
{
    /*
        Test 1:
        Malloc, free, Malloc, free... 120 times
    */
    int i = 0;
    struct timeval t1, t2;
    void *pointers[150];

    gettimeofday(&t1, NULL);

    for (i = 0; i < 150; i++)
    {
        void *add = (void *)malloc(1);

        pointers[i] = add;
        if (DEBUG)
        {
            printf("malloced %p\n", pointers[i]);
        }

        free(pointers[i]);
        if (DEBUG)
        {
            printf("freed\n");
        }
    }

    gettimeofday(&t2, NULL);
    return time_difference(t1, t2);
}

double test_two()
{
    /*
    Test 1:
    Malloc 120 times
    Free 120 times
*/
    int i = 0;
    struct timeval t1, t2;
    void *pointers[120];

    gettimeofday(&t1, NULL);

    for (i = 0; i < 120; i++)
    {
        void *add = (void *)malloc(1);
        pointers[i] = add;
        if (DEBUG)
        {
            printf("malloced %p\n", pointers[i]);
        }
    }

    for (i = 0; i < 120; i++)
    {
        free(pointers[i]);
        if (DEBUG)
        {
            printf("freed\n");
        }
    }

    gettimeofday(&t2, NULL);
    return time_difference(t1, t2);
}

double test_three()
{
    /*
        malloc(120)
        malloc(100)
        free(120) // free space of 120 bytes
        malloc()
    */
    struct timeval t1, t2;
    gettimeofday(&t1, NULL);
    for (int i = 120; i >= 1; i /= 2)
    {
        void *add = malloc(i);
        if (DEBUG)
        {
            printf("mallocing %d bytes and the address we get is %p\n", i, add);
        }
        free(add);
        if (DEBUG)
        {
            printf("freeing %d bytes\n", i);
        }
    }
    gettimeofday(&t2, NULL);

    return time_difference(t1, t2);
}
double test_4()
{
    struct timeval t1, t2;

    gettimeofday(&t1, NULL);
    printf("Testing 50 and 25 byte allocation and release\n");
    int *allocate_block_one = (int *)malloc(50);
    int *allocate_block_two = (int *)malloc(25);

    printf("Allocating 50, 25 byte blocks\n");
    print_implicit_free_list();

    printf("After freeing 50 byte blocks\n");
    free(allocate_block_one);
    // free(allocate_block_one);

    // printf("After freeing 50 byte blocks\n");
    print_implicit_free_list();

    printf("Allocating 30 byte block\n");
    int *allocate_block_three = (int *)malloc(30);
    print_implicit_free_list();

    printf("Allocating 20 byte block\n");
    int *allocate_block_four = (int *)malloc(24);
    print_implicit_free_list();

    printf("After freeing 30 byte blocks\n");
    free(allocate_block_three);
    print_implicit_free_list();

    printf("After freeing 25 byte blocks\n");
    free(allocate_block_two);
    print_implicit_free_list();

    free(allocate_block_four); // here

    gettimeofday(&t2, NULL);

    return time_difference(t1, t2);
}
double test_5()
{
    /*
        Randomly choose between
        • Allocating a 1-byte chunk and storing the pointer in an array
        • Deallocating one of the chunks in the array (if any)

        Repeat until you have called malloc() 120 times,
        then free all remaining allocated chunks.
    */
    int MAX = 120;
    void *pointers[120];
    struct timeval t1, t2;
    gettimeofday(&t1, NULL);

    int count_malloc = 0;
    int arraysize = 0;

    while (count_malloc < MAX)
    {
        int random_bit = rand() % 2;
        if (random_bit == 1)
        {
            // malloc
            // 1, 1, 0
            pointers[arraysize] = malloc(1);
            if (DEBUG)
            {
                printf("Malloced address %p at array index %d\n", pointers[count_malloc], count_malloc);
            }
            count_malloc++;
            arraysize++;
        }
        else
        {
            if (arraysize > 0)
            {
                int random_index = rand() % (arraysize);
                void *tmp = pointers[random_index];
                pointers[random_index] = pointers[arraysize - 1];
                pointers[arraysize - 1] = tmp;
                free(pointers[arraysize - 1]);
                // free(pointers[random_index]);
                if (errorNoFree == 1)
                {
                    printf("Invalid address from test_5....\n");
                }
                if (errorNoFree == 2)
                {
                    printf("Freeing the same address twice from test_5....\n");
                }

                if (DEBUG)
                {
                    printf("Freed address %p at array index %d\n", pointers[random_index], random_index);
                }
                if (errorNoFree == 0)
                {
                    pointers[arraysize - 1] = 0;
                    arraysize--;
                }
            }
        }
    }

    // free all remaining allocated chunks.
    for (int i = 0; i < arraysize; i++)
    {
        if (pointers[i] != 0)
        {
            free(pointers[i]);
            if (DEBUG)
            {
                printf("Freeing remaining address %p at index %d\n", pointers[i], i);
            }
            pointers[i] = 0;
        }
    }
    gettimeofday(&t2, NULL);
    return time_difference(t1, t2);
}
double test_7(int pattern)
{
    // coalescing test case.
    int random_mem = 4; // rand() % MEM_SIZE;

    // malloc random_mem byte array chunks into 4096 byte space.

    int block_size = random_mem + sizeof(header);
    int number_of_blocks = floor(MEM_SIZE / block_size);

    printf("The number of blocks are %d\n", number_of_blocks);

    // allocated addresses
    void *pointers[number_of_blocks];

    // Memset here is much cleaner: https://en.cppreference.com/w/cpp/string/byte/memset
    memset(pointers, 0, number_of_blocks);

    // allocate the full memory.
    int index = 0;
    int accumulated_malloc_size = 0;

    // This will store address in pointers array only until pointers[number_of_blocks - 2] because last block may have a higher block size
    // than the previous blocks which have a block size of random_mem + sizeof(header).

    // However, if MEM_SIZE % block_size = 0 then pointers[0,1,2...number_of_blocks-1] will have addresses and every block size will be the same.
    while (1)
    {
        void *malloc_p = (void *)malloc(random_mem);
        if (malloc_p == NULL)
        {
            break;
        }
        pointers[index++] = malloc_p;
        accumulated_malloc_size += block_size;
    }
    int last_block_size = MEM_SIZE - accumulated_malloc_size - sizeof(header); // calculate last block size that is 4096 - accumulated_malloc_size - metadata_size
    void *allocate_last_block = (void *)malloc(last_block_size);
    if (allocate_last_block == NULL)
    {
        printf("The last block size that will be allocated is %d\n", last_block_size);
        printf("Couldn't allocate last block\n");
        // No need to store a pointer in pointers because pointers[0,1,2...number_of_blocks-1] has all addresses including the last block size.
    }
    else
    {
        // Store address at pointers[number_of_blocks - 1]
        pointers[index] = allocate_last_block;
    }

    struct timeval t1, t2;
    gettimeofday(&t1, NULL);

    // free the full memory.
    if (pattern == 1)
    {
        /*
            Pattern 1.
            Free bit status for sample memory: [0,0,0,0,0,0]

            [0,0,0,1,0,0]

            1. Free the middle bit. [0,0,0,1,0,0] (i.e case 1)
            2. Free all left bit. (i.e case 2)
                - [0,0,0,1,0,0] -> [1,1,1,1,0,0]

            3. Free all the right bit. (i.e case 3)
                - [0,0,1,0,0] -> [0,0,1,1,1]
        */
        // Case1;
        int midpoint = number_of_blocks / 2;
        free(pointers[midpoint]);
        pointers[midpoint] = 0;
        // Case 2;
        for (int i = midpoint - 1; i >= 0; i--)
        {
            free(pointers[i]);
            pointers[i] = 0;
        }
        // Case 3;
        for (int i = midpoint + 1; i < number_of_blocks; i++)
        {
            free(pointers[i]);
            pointers[i] = 0;
        }
    }
    else if (pattern == 2)
    {

        /*
            Pattern 2.
            Free bit status for sample memory: [0,0,0,0,0,0]

            1. Free every alternate bit (i.e case 1)
                - [1,0,1,0,1,0]
            2. [1,0,1,0,1,0]
                - [1, 0, 1, 0] (Case 4)
                - [1, 0] (Freeing last node) bc next node doesn't exist.
                - [1]

        */
        // Case1;
        for (int i = 0; i < number_of_blocks; i++)
        {
            if (i % 2 == 0)
            {
                free(pointers[i]);
                pointers[i] = 0;
            }
        }
        // Case4;
        for (int i = 0; i < number_of_blocks; i++)
        {
            if (i % 2 == 1)
            {
                free(pointers[i]);
                pointers[i] = 0;
            }
        }
    }
    else if (pattern == 3)
    {
        /*
            Pattern 3.
            Free bit status for sample memory: [0,0,0,0,0,0]

            1. Free every non-adjancent bit with random spacing (Previous pattern had spacing as 1 because [1,0,1,0...]) (i.e case 1)
                - [1,0,1,0,0,1,0,1,0,0,0,1]
        */
        // Case1;
        int i =0;
        while(i < number_of_blocks)
        {
            int spacing = (rand() % 5) + 2; // +2 to avoid adjacent random numbers
            free(pointers[i]);
            pointers[i] = 0;
            i+=spacing;
        }
        // Case2, Case3,Case4;
        int left = 0;
        int right = number_of_blocks - 1;
        while(left < right){
            int rand_bit = rand() % 2;
            if(rand_bit){
                if (pointers[left]!=0)
                {
                    free(pointers[left]);
                    pointers[left] = 0;
                }
                left+=1;
            }
            else{
                if (pointers[right]!=0)
                {
                    free(pointers[right]);
                    pointers[right] = 0;
                }
                right-=1;
            }
        }
    }
    gettimeofday(&t2, NULL);

    return time_difference(t1, t2);
}
double  test_eight(){
    struct timeval t1,t2;
    gettimeofday(&t1,NULL);
    for (int iteration=0; iteration<1000; iteration++) {
        void * adresses[160];
        int i=0;
        for (int c=2048; c>=8; c=c/2) {
            adresses[i] = malloc(c-4);
            i++;
        }
        free(adresses[0]);
        for (int c=8; c<=1024; c=c*2) {
            int random_index = rand() % 2;
            //printf("random_index :  %d\n",random_index);
            if (random_index ==0 ) {
                adresses[i] = malloc(c-4);
            } else {
                adresses[i] = malloc(c+4);
            }
            i++;
        }
        for (int ii=1;ii<i;ii++) {
            if (adresses[ii]!=NULL)
                free(adresses[ii]);

        }


    }

//print_implicit_free_list();
    gettimeofday(&t2,NULL);
    return time_difference(t1,t2);

}

int main(int argv, char **argc)
{
    // average time
    // double time_one = test_one();
    // double time_two = test_two();
    // double time_three = test_three();
    // double time_five = test_5();
    // double time_six = test_6();
     double time_eight = test_eight();
    //  test_4();
    // printf("Time it took to run test_one: %f seconds\n", time_one);
    // printf("Time it took to run test_two: %f seconds\n", time_two);
    // printf("Time it took to run test_three: %f seconds\n", time_three);
    // printf("Time it took to run test_five: %f seconds\n", time_five);
     printf("Time it took to run test_six: %f seconds\n", time_eight);
    
    // double time_seven_pattern_one = test_7(1);
    // double time_seven_pattern_two = test_7(2);
    //double time_seven_pattern_three = test_7(3);
    print_implicit_free_list();
    // printf("Time it took to run test seven part 1: %f seconds\n", time_seven_pattern_one);
    // printf("Time it took to run test seven part 2: %f seconds\n", time_seven_pattern_two);
    //printf("Time it took to run test seven part 3: %f seconds\n", time_seven_pattern_three);
    return EXIT_SUCCESS;
}
// double workloadA(){
// int i = 0;
// double time_taken = 0;
// struct timeval tv1, tv2;
// void* testarr[5000];
// gettimeofday(&tv1, NULL);
// for(i = 0; i < 150; i++){
// 	testarr[i] = malloc(1);
// 	if(DEBUG){
// 		printf("malloced %p\n", &testarr[i]);
// 	}
// 	free(testarr[i]);
// 	if(DEBUG){
// 		printf("freed\n");
// 	}
// }

// gettimeofday(&tv2, NULL);
// time_taken = (double) (tv2.tv_usec - tv1.tv_usec)/1000000 + (double) (tv2.tv_sec - tv1.tv_sec);

// 	if(DEBUG){
// 		printf("In workloadA(), time taken: %f microseconds\n", time_taken*1000000);
// 	}

// 	return time_taken;
// }

// double workloadB(){
// 	int i = 0;
// 	double time_taken = 0;
// 	struct timeval tv1, tv2;
// 	void* testarr[5000];
// 	gettimeofday(&tv1, NULL);
// 	for(i = 0; i < 150; i++){
// 		testarr[i] = malloc(1);
// 		if(DEBUG){
// 			printf("malloced %p\n", &testarr[i]);
// 		}
// 	}

// 	for(i = 0; i < 150; i++){
// 		free(testarr[i]);
// 		if(DEBUG){
// 			printf("freed\n");
// 		}
// 	}

// 	gettimeofday(&tv2, NULL);
// 	time_taken = (double) (tv2.tv_usec - tv1.tv_usec)/1000000 + (double) (tv2.tv_sec - tv1.tv_sec);

// 	if(DEBUG){
// 		printf("In workloadB(), time taken: %f microseconds\n", time_taken*1000000);
// 	}

// 	return time_taken;
// }

// double workloadC(){
// 	int i = 0;
// 	double time_taken = 0;
// 	struct timeval tv1, tv2;
// 	void* testarr[5000];
// 	int malloc_counter = 0;
// 	int free_counter = 0;
// 	int random_bit = 0;
// 	int size_occupied = 0;
// 	int random_index = 0;
// 	gettimeofday(&tv1, NULL);
// 	while(malloc_counter < 150){
// 		random_bit = (rand() % 2);
//         printf("Random bit: %d\n", random_bit);
// 		if(random_bit || size_occupied  == 0){//malloc a new byte
// 			void * add_one_byte = malloc(1);
//             testarr[size_occupied] = add_one_byte;
// 			malloc_counter++;
// 			if(DEBUG){
// 				printf("malloced %p\n", testarr[size_occupied]);
// 			}
//             size_occupied++;
//             print_implicit_free_list();
// 		}
// 		else{
// 			random_index = rand() % size_occupied;
// 			if(random_index == (size_occupied -1)){
// 				free(testarr[random_index]);
// 				testarr[random_index] = 0;
// 				free_counter++;
// 				if(DEBUG){
// 					printf("last_index: freed\n");
// 				}
// 			}
// 			else{
// 				free(testarr[random_index]);
// 				testarr[random_index] = testarr[size_occupied-1];
// 				testarr[size_occupied-1] = 0;
// 				free_counter++;
// 				if(DEBUG){
// 					printf("any index: freed\n");
// 				}
//                 print_implicit_free_list();
// 			}

// 			size_occupied--;
// 		}
// 	}

// 	for(i = size_occupied-1; i >= 0; i--){
// 		free(testarr[i]);
// 		free_counter++;
// 		if(DEBUG){
// 			printf("freed\n");
// 		}
// 	}
// 	gettimeofday(&tv2, NULL);
// 	time_taken = (double) (tv2.tv_usec - tv1.tv_usec)/1000000 + (double) (tv2.tv_sec - tv1.tv_sec);

// 	if(DEBUG){
// 		printf("In workload C: malloc_counter: %d, free_counter: %d\n", malloc_counter, free_counter);
// 		printf("In workloadC(), time taken: %f microseconds\n", time_taken*1000000);
// 	}

// 	return time_taken;
// }

// double workloadD(){
// 	int i = 0;
// 	double time_taken = 0;
// 	struct timeval tv1, tv2;
// 	void* testarr[5000];
// 	int malloc_counter = 0;
// 	int free_counter = 0;
// 	int random_bit = 0;
// 	int random_size = 0;
// 	int size_occupied = 0;
// 	int random_index = 0;
// 	gettimeofday(&tv1, NULL);
// 	while(malloc_counter < 150){
// 		random_bit = (rand() % 2);
// 		if(random_bit || size_occupied  == 0){//malloc a new byte
// 			random_size = ((rand() % 64) + 1);
// 			testarr[size_occupied] = malloc(random_size);
// 			malloc_counter++;
// 			size_occupied++;
// 			if(DEBUG){
// 				printf("malloced %p random_size %d \n", &testarr[size_occupied], random_size);
// 			}
// 		}
// 		else{
// 			random_index = rand() % size_occupied;
// 			if(random_index == (size_occupied -1)){
// 				free(testarr[random_index]);
// 				testarr[random_index] = 0;
// 				free_counter++;
// 				if(DEBUG){
// 					printf("freed\n");
// 				}
// 			}
// 			else{
// 				free(testarr[random_index]);
// 				testarr[random_index] = testarr[size_occupied-1];
// 				testarr[size_occupied-1] = 0;
// 				free_counter++;
// 				if(DEBUG){
// 					printf("freed\n");
// 				}
// 			}

// 			size_occupied--;
// 		}
// 	}

// 	for(i = size_occupied-1; i >= 0; i--){
// 		free(testarr[i]);
// 		free_counter++;
// 		if(DEBUG){
// 			printf("freed\n");
// 		}
// 	}
// 	gettimeofday(&tv2, NULL);
// 	time_taken = (double) (tv2.tv_usec - tv1.tv_usec)/1000000 + (double) (tv2.tv_sec - tv1.tv_sec);

// 	if(DEBUG){
// 		printf("In workloadD: malloc_counter %d, free_counter: %d\n", malloc_counter, free_counter);
// 		printf("In workloadD(), time taken: %f microseconds\n", time_taken*1000000);
// 	}

// 	return time_taken;
// }

// double workloadE(){
// 	int i = 0;
// 	double time_taken = 0;
// 	struct timeval tv1, tv2;
// 	void* testarr[5000];

// 	int malloc_counter = 0;
// 	int free_counter = 0;
// 	int random_bit = 0;
// 	int random_size = 0;
// 	int size_occupied = 0;
// 	int random_index = 0;
// 	gettimeofday(&tv1, NULL);
// 	/*
// 	while(malloc_counter < 50){
// 		random_bit = (rand() % 2);
// 		if(random_bit || size_occupied  == 0){//malloc a new byte
// 			random_size = (rand() > RAND_MAX/2) ? 2 : 3;
// 			testarr[size_occupied] = malloc(random_size);
// 			malloc_counter++;
// 			size_occupied++;
// 			if(DEBUG){
// 				printf("malloced %p random_size %d \n", &testarr[size_occupied], random_size);
// 			}
// 		}
// 		else{
// 			random_index = rand() % size_occupied;
// 			if(random_index == (size_occupied -1)){
// 				free(testarr[random_index]);
// 				testarr[random_index] = 0;
// 				free_counter++;
// 				if(DEBUG){
// 					printf("freed\n");
// 				}
// 			}
// 			else{
// 				free(testarr[random_index]);
// 				testarr[random_index] = testarr[size_occupied-1];
// 				testarr[size_occupied-1] = 0;
// 				free_counter++;
// 				if(DEBUG){
// 					printf("freed\n");
// 				}
// 			}
// 			size_occupied--;
// 		}
// 	}

// 	for(i = size_occupied-1; i >= 0; i--){
// 		free(testarr[i]);
// 		free_counter++;
// 		if(DEBUG){
// 			printf("freed\n");
// 		}
// 	}
// 	*/

// 	while(malloc_counter < 100){
// 		random_bit = (rand() % 3);
// 		if(random_bit == 0 || size_occupied  == 0){//malloc a new byte
// 			random_size = 2 * ((rand() % 20) + 1);

//             void *allocate_block = (void *) malloc(random_size);

// 			malloc_counter++;
// 			size_occupied++;
// 			if(DEBUG){
// 				printf("malloced %p random_size %d malloc_counter: %d\n", &testarr[size_occupied], random_size,malloc_counter);
// 			}
// 		}
// 		else if(random_bit == 1){
// 			random_index = rand() % size_occupied;
// 			if(random_index == (size_occupied -1)){
// 				free(testarr[random_index]);
// 				testarr[random_index] = 0;
// 				free_counter++;
// 				if(DEBUG){
// 					printf("freed, free_counter:%d\n", free_counter);
// 				}
// 			}
// 			else{
// 				free(testarr[random_index]);
// 				testarr[random_index] = testarr[size_occupied-1];
// 				testarr[size_occupied-1] = 0;
// 				free_counter++;
// 				if(DEBUG){
// 					printf("freed, freE_counter: %d\n", free_counter);
// 				}
// 			}

// 			size_occupied--;
// 		}
// 		else{
// 			for(i = 0; i < 50; i++){
// 				testarr[size_occupied] = malloc(1);
// 				malloc_counter++;
// 				free(testarr[i]);
// 				free_counter++;
// 				if(DEBUG){
// 					printf("In for loop, Malloc: %d, Free: %d\n", malloc_counter, free_counter);
// 				}
// 			}
// 		}
// 	}

// 	for(i = size_occupied-1; i >= 0; i--){
// 		free(testarr[i]);
// 		free_counter++;
// 		if(DEBUG){
// 			printf("freed, free_counter: %d\n", free_counter);
// 		}
// 	}

// 	gettimeofday(&tv2, NULL);
// 	time_taken = (double) (tv2.tv_usec - tv1.tv_usec)/1000000 + (double) (tv2.tv_sec - tv1.tv_sec);

// 	if(DEBUG){
// 		printf("In workloadE: malloc_counter: %d, free_counter: %d\n", malloc_counter, free_counter);
// 		printf("In workloadE(), time taken: %f microseconds\n", time_taken*1000000);
// 	}

// 	return time_taken;
// }

// double workloadF(){
// 	int i = 0;
// 	double time_taken = 0;
// 	struct timeval tv1, tv2;
// 	void* testarr[5000];
// 	int malloc_counter = 0;
// 	int free_counter = 0;
// 	int random_bit = 0;
// 	int random_size = 0;
// 	int size_occupied = 0;
// 	int random_index = 0;
// 	gettimeofday(&tv1, NULL);

// 	for(i = 0; i < 50; i++){
// 		testarr[i] = malloc(5);
// 		malloc_counter++;
// 		if(DEBUG){
// 			printf("malloced %p 5\n", &testarr[i]);
// 		}
// 	}

// 	while(malloc_counter < 150){
// 		random_bit = (rand() % 2);
// 		if(random_bit || size_occupied  == 0){//malloc a new byte
// 			random_size = ((rand() % 64) + 1);
// 			testarr[size_occupied] = malloc(random_size);
// 			malloc_counter++;
// 			size_occupied++;
// 			if(DEBUG){
// 				printf("malloced %p random_size %d \n", &testarr[size_occupied], random_size);
// 			}
// 		}
// 		else{
// 			random_index = rand() % size_occupied;
// 			if(random_index == (size_occupied -1)){
// 				free(testarr[random_index]);
// 				testarr[random_index] = 0;
// 				free_counter++;
// 				if(DEBUG){
// 					printf("freed\n");
// 				}
// 			}
// 			else{
// 				free(testarr[random_index]);
// 				testarr[random_index] = testarr[size_occupied-1];
// 				testarr[size_occupied-1] = 0;
// 				free_counter++;
// 				if(DEBUG){
// 					printf("freed\n");
// 				}
// 			}

// 			size_occupied--;
// 		}
// 	}

// 	for(i = size_occupied-1; i >= 0; i--){
// 		free(testarr[i]);
// 		free_counter++;
// 		if(DEBUG){
// 			printf("freed\n");
// 		}
// 	}

// 	for(i = 0; i < 50; i++){
// 		free(testarr[i]);
// 		free_counter++;
// 		if(DEBUG){
// 			printf("freed.\n");
// 		}
// 	}

// 	gettimeofday(&tv2, NULL);
// 	time_taken = (double) (tv2.tv_usec - tv1.tv_usec)/1000000 + (double) (tv2.tv_sec - tv1.tv_sec);

// 	if(DEBUG){
// 		printf("In workloadF: malloc_counter: %d, free_counter: %d\n", malloc_counter, free_counter);
// 		printf("In workloadF(), time taken: %f microseconds\n", time_taken*1000000);
// 	}

// 	return time_taken;
// }

// int main(){
// 	//Variable declaration

// 	double time_A = 0;
// 	double time_B = 0;
// 	double time_C = 0;
// 	double time_D = 0;
// 	double time_E = 0;
// 	double time_F = 0;

// 	int i = 0;

// 	for(; i < 100; i++){
// 		// //Workload A
// 		// time_A += workloadA();
// 		// //printf("time_A: %f seconds\n", time_A);

// 		// //Workload B
// 		// time_B += workloadB();
// 		// //printf("time_B: %f seconds\n", time_B);

// 		//Workload C
// 		time_C += workloadC();
// 		//printf("time_C: %f seconds\n", time_C);

// 		// //Workload D
// 		// time_D += workloadD();
// 		// //printf("time_D: %f seconds \n", time_D);

// 		// //Workload E
// 		// time_E += workloadE();
// 		// //printf("time_E: %f seconds\n", time_E);

// 		// //Workload F
// 		// time_F += workloadF();
// 		// //printf("time_F: %f seconds\n", time_F);
// 	}

// 	printf("average time_A: %f seconds\n", time_A/100);
// 	printf("average time_B: %f seconds\n", time_B/100);
// 	printf("average time_C: %f seconds\n", time_C/100);
// 	printf("average time_D: %f seconds\n", time_D/100);
// 	printf("average time_E: %f seconds\n", time_E/100);
// 	printf("average time_F: %f seconds\n", time_F/100);

// 	return 0;
// }

// int main(int argc, char **argv)
// {

//     /*
//         Test 1: repeated memory request and free pattern 120 times.
//     */
//     int i = 0;
//     int max = 120;
//     // for (i = 0; i < max; i++)
//     // {
//     //     int *allocate_block = (int *)malloc(1);
//     //     free(allocate_block);
//     // }

//     // print_implicit_free_list();
//     int *pointers[120];
//     for (i = 0; i < max; i++)
//     {
//         int *allocate_block = (int *)malloc(10);
//         if (allocate_block == NULL)
//         {
//             printf("Couldn't allocate memory!\n");
//         }
//         pointers[i] = allocate_block;
//         printf("Allocated %p\n", pointers[i]);
//     }
//     printf("List after 120 allocation\n");
//     print_implicit_free_list();
//     for (i = 0; i < max; i++)
//     {
//         free(pointers[i]);
//     }
//     // }
//     // printf("List after 120 deallocation\n");
//     // print_implicit_free_list();

//     // for (i = 0; i < max; i++)
//     // {
//     //     int *allocate_block = (int *)malloc(i);
//     //     free(allocate_block);
//     // }
//     // print_implicit_free_list();

//     // for (i = 0; i < max; i++)
//     // {
//     //     int *allocate_block = (int *)malloc(i);
//     //     free(allocate_block);
//     // }
//     // print_implicit_free_list();

// }