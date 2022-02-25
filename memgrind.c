#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <ctype.h>
#include "mymalloc.h"

unsigned int DEBUG = 0;

double time_difference(struct timeval t1, struct timeval t2)
{
    double elapsed;
    elapsed = (double)(t2.tv_usec - t1.tv_usec) / 1000000 + (double)(t2.tv_sec - t1.tv_sec);
    return elapsed;
}

double test_one()
{
    int i = 0;
    double time_taken = 0;
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
    int i = 0;
    double time_taken = 0;
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

int main(int argv, char **argc)
{
    // average time
    double time_one = test_one();
    double time_two = test_two();

    printf("Time it took to run test_one: %f seconds\n", time_one);
    printf("Time it took to run test_two: %f seconds\n", time_two);
    
    return 0;
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

//     // printf("Testing multiple of 3 chunk alloc and release\n");
//     // int *allocate_block_one =  (int *)malloc(50);
//     // int *allocate_block_two =  (int *)malloc(25);

//     // printf("Allocating 50, 25 byte blocks\n");
//     // print_implicit_free_list();

//     // printf("After freeing 50 byte blocks\n");
//     // free(allocate_block_one);
//     // // free(allocate_block_one);

//     // // printf("After freeing 50 byte blocks\n");
//     // print_implicit_free_list();

//     // printf("Allocating 30 byte block\n");
//     // int *allocate_block_three =  (int *)malloc(30);
//     // print_implicit_free_list();

//     // printf("Allocating 20 byte block\n");
//     // int *allocate_block_four =  (int *)malloc(24);
//     // print_implicit_free_list();

//     //  printf("After freeing 30 byte blocks\n");
//     // free(allocate_block_three);
//     // print_implicit_free_list();

//     // printf("After freeing 25 byte blocks\n");
//     // free(allocate_block_two);
//     // print_implicit_free_list();

// }