Mymalloc and Myfree project

## Authors
Rohan Deshpande: ryd4
Selin Denise Altiparmak: sda81

1. ## Metadata Structure: struct header
    1. Our metadata is a 4 byte or 32 bits block.
    2. We assigned each field in the metadata struct certain amount of bits and thus the total number of bits add up to 32 bits. (This type of data structure is called bit fields)
    3. We assigned 13 bits for the ```block_size``` field in the metadata of the current block because the maximum size of any block can be 4096 bytes which is 2^13
    4. We assigned 13 bits for the ```prev_block_size``` field in the metadata of the current block because the maximum size of any block can be 4096 bytes which is 2^13. The  ```prev_block_size``` has the size of previous block stored in the current block which is useful during coalescing blocks.
    5. We assigned 1 bit ```last_node``` field in the metadata to indicate whether the current block is the last node or not. If the current block is the last node then ```last_node = 1``` else ```last_node = 0```.
    6. We assigned 1 bit ```first_node``` field in the metadata to indicate whether the current block is the first node or not. If the current block is the first node then ```first_node = 1``` else ```first_node = 0```.
    7. We had 2 bits as free space in the struct because we didn't have to store any data fields in that attribute, so preciesly, our struct only allocates 30 bits, however due to the total size of an int (4 bytes or 32 bits), we have to keep those 2 bits as padding.
    8. We assigned 1 bit ```free``` field in the metadata to indicate whether the current block is free or not. If the current block is free then ```free = 1``` else ```free = 0```.
    9. We assigned 1 bit ```prev_free``` field in the metadata to indicate whether the previous block is free or not. If the previous block is free then ```prev_free = 1``` else ```prev_free = 0```.

2. ## Malloc Functionality
    1. We defined a function ```void *mymalloc(size_t requested_size, char *file, int line)``` that takes in 3 arguements in total. The first arguement specifies the ```payload size``` that the client requests, the second argument and third arguement specifies the filename.c and line number from ```where``` malloc was called by the client. 
    2. ```mymalloc()``` returns an ```address or pointer(void*)``` to memory where the requested payload size will be allocated in memory.
    3. The malloc functionality works around implict free lists, and the way it works is as follows:

    4. For the basic error checking, we first check whether the number of bytes requested by the client is allowed by checking if its a positive number and it is less than ```4096 bytes - sizeof(header).```
    5. Now if the number of bytes is valid, we proceed by getting the base pointer/address of the first index of memory. We creating a initial metadata by pointing to this base address of memory. This inital metadata consist of the initial block size which is 4096 bytes, and it's a free block initially.
    6. Now suppose the user requests ```n bytes``` and if ```n + sizeof(header)``` is less than ```current_block_size``` then we split the ```current_block``` into sub-blocks ``b_1`` and ``b_2`` where ```b_1``` will be ```allocated``` have size ```n + sizeof(header)``` and ```b_2``` will be ```freed``` and have a size of ```current_block_size - n - sizeof(header)```. Then we return the address to the  ```payload NOT the header``` of ```b_1``.
    7. Now suppose the user request ```n bytes``` and if ```n + sizeof(header)``` is equal to ```current_block_size``` then we just set ```current_block``` as ```allocated``` status because we have found the block of memory that has the exact number of bytes that we requested. Then we return the address to the ```payload NOT the header``` of ```current_block```.
    8. Now suppose the user request ```n bytes``` and there isn't space for n bytes after searching through the implicit free list then we print out the error message indicating that the number of bytes requested isn't available in memory, and thus return ```NULL```.

3. ## Free Functionality
    1. We defined a function void ```*myfree(void* ptr, char* file, int line)``` that takes in 3 arguements in total. The first arguement specifies the pointer of the payload in memory that is to be freed, the second argument and third arguement specifies the filename.c and line number from where malloc was called by the client.
    2. First we check if the payload pointer is in memory by iterating through each element of memory less than or equal to 4096 times. The run time complexity of this search is ```O(1)``` because we are searching through a ```constant``` amount of memory. 
    3. If we don't find the address in memory then we can indicate an error saying that the address passed to memory is invalid.
    4. If the address is valid and found in memory, but the metadata pointing to the address indicates that it is freed (```free``` bit set to 1) then we throw a double free error because the free method is trying to free an already freed block.
    5. Since the metadata of each block of memory consist of the previous block size, previous free status, the current block size and the current free status, thus, we implement a ```boundary tag coalescing``` technique for constant time run time efficiency. 
    6. Short note: to compute the address of ```next_block_pointer``` or ```prev_block_pointer``` from the perspective of ```current_block_pointer```, we created a general formula for the address computation as follows: 

    - ```prev_block_pointer = current_block_pointer metadata_of_current_block_pointer->prev_block_size```
    
    - ```next_block_pointer = current_block_pointer + metadata_of_current_block_pointer->block_size```
    
        We did this because each node in the implicit free list is seperated by the ```block_size``` field in the node.

    7. There are 4 cases of coalescing that we took care off when we were designing our ```boundary tag coalescing``` mechanism. Let's assume that the ```current_block_pointer``` exists in the implicit free list and is the node that is pointing to the memory location that we want to free. We also need the ```next_block_pointer``` and ```prev_block_pointer``` of the implicit free list from the perspective of the ```current_block_pointer``` also exist. 
        -  To check if previous and next node exist, we check the ```first_node``` status and ```last_node``` status of the ```current_block_pointer```'s metadata. 
        - To check whether ```next_block_pointer``` and ```prev_block_pointer``` exist, we also stored whether each node is the ```first_node``` and ```last_node```. If the ```current_block_pointer```'s metadata indicates that the ```last_node=1``` then we can assume that the ```current_block_pointer``` is the last node, else if ```last_node=0``` then we can assume that there exists a ```next_block_pointer```. Similarly, if the ```current_block_pointer```'s metadata indicates that the ```first_node=1``` then we can assume that the ```current_block_pointer``` is the first node, else if ```first_node=0``` then we can assume that there exists a ```prev_block_pointer```.
        - Now assuming we are freeing ```current_block_pointer```,  ```prev_block_pointer``` and ```next_block_pointer``` exist, so we handled 4 cases of coalescing.
            
            - ```Case 1```: ```prev_block_pointer``` is allocated (i.e. ```metadata_of_prev_block_pointer```'s ```free``` bit is set to 0) and ```next_block_pointer``` allocated (i.e. ```metadata_of_next_block_pointer```'s ```free``` bit is set to 0). We just set ```current_block_pointer``` to free by setting its ```metadata_of_current_block_pointer``` ```free``` status to 1.
            
            - ```Case 2```: 
            ```prev_block_pointer``` is allocated (i.e. ```metadata_of_prev_block_pointer```'s ```free``` bit is set to 0) and ```next_block_pointer``` is free (i.e. ```metadata_of_next_block_pointer```'s ```free``` bit is set to 1). We coalesce ```current_block_pointer``` and ```next_block_pointer``` by combining the ```block_size``` of ```current_block_pointer``` with the block_size of the ```next_block_pointer```. Then we set the ```current_block_pointer``` to free (i.e its metadata's ```free``` bit is set to 1), and we update the ```metadata_of_current_block_pointer```'s ```last_node``` status with the ```metadata_of_next_block_pointer```'s ```last_node``` status and complete the coalescing of the ```current_block_pointer``` block and ```next_block_pointer```. 
            
                Finally, we get the ```next_to_next_block_pointer``` from the perspective of the ```current_block_pointer```. To do that we first check if ```metadata_of_current_block_pointer```'s ```last_node ```status is not set to 1 (meaning ```current_block_pointer``` is not the last node). Hence by that check, ```next_dot_next_block_pointer``` exists. Then we update the ```metadata_of_next_to_next_block_pointer```'s ```prev_size``` and ```prev_free``` status of with the latest ```prev_size``` and ```prev_free```, which is the ```metadata_of_current_block_pointer```'s ```block_size``` and ```free``` status. 
                
                In short, case 2 just combines the ```prev_block_pointer``` with ```current_block_pointer``` to achieve coalescing and updates the ```next_dot_next_block_pointer``` with the parameters of ```prev_block_pointer``` to maintain the implicit free list after coalescing has been performed. 
            
            - ```Case 3```: 
                ```prev_block_pointer``` is free (i.e. ```metadata_of_prev_block_pointer```'s ```free``` bit is set to 1) and ```next_block_pointer``` is allocated (i.e. ```metadata_of_next_block_pointer```'s ```free``` bit is set to 0). We coalesce ```prev_block_pointer``` and ```current_block_pointer``` by combining the ```block_size``` of ```prev_block_pointer``` with the block_size of the ```current_block_pointer```. Then we set the ```metadata_of_current_block_pointer```'s to free by setting ```free=1```, and we update the ```metadata_of_prev_block_pointer```'s ```last_node``` status with the ```metadata_of_current_block_pointer```'s ```last_node``` status and complete the coalescing of the ```prev_block_pointer``` block with the ```current_block_pointer```. 
                
                Finally, we update the ```next_block_pointer``` from the perspective of the ```current_block_pointer```. Since ```next_block_pointer``` already exist, we update the ```metadata_of_next_block_pointer```'s ```prev_size``` and ```prev_free``` status of with the latest ```prev_size``` and ```prev_free```, which is the ```metadata_of_prev_block_pointer```'s ```block_size``` and ```free``` status.
                
                In short, this case 3 just combines the ```prev_block_pointer``` with the ```current_block_pointer``` to achieve coalescing and updates the ```next_block_pointer``` with the parameters of ```prev_block_pointer``` to maintain the implicit free list after coalescing has been performed.

            - ```Case 4```: 
                ```prev_block_pointer``` is free (i.e. ```metadata_of_prev_block_pointer```'s ```free``` bit is set to 1) and ```next_block_pointer``` is free (i.e. ```metadata_of_next_block_pointer```'s ```free``` bit is set to 1). We coalesce ```prev_block_pointer``` and ```current_block_pointer``` and  ```next_block_pointer``` by combining the ```block_size``` of ```prev_block_pointer``` with the block_size of the ```current_block_pointer``` and ```next_block_pointer```. Then we set the ```metadata_of_current_block_pointer```'s to free by setting ```free=1```, and we update the ```metadata_of_prev_block_pointer```'s ```last_node``` status with the ```metadata_of_current_block_pointer```'s ```last_node``` status and complete the coalescing of the ```prev_block_pointer``` block with the ```current_block_pointer``` and ```next_block_pointer```. 
                
                Finally, we get the ```next_to_next_block_pointer``` from the perspective of the ```previous_block_pointer```. To do that we first check if ```metadata_of_previous_block_pointer```'s ```last_node ```status is not set to 1 (meaning ```current_block_pointer``` is not the last node). Hence by that check, ```next_dot_next_block_pointer``` exists. Then we update the ```metadata_of_next_to_next_block_pointer```'s ```prev_size``` and ```prev_free``` status of with the latest ```prev_size``` and ```prev_free```, which is the ```metadata_of_previous_block_pointer```'s ```block_size``` and ```free``` status. 
                
                In short, case 4 just combines the ```prev_block_pointer``` with ```current_block_pointer``` and ```next_block_pointer``` to achieve coalescing and updates the ```next_dot_next_block_pointer``` with the parameters of ```prev_block_pointer``` to maintain the implicit free list after coalescing has been performed. 
 

# Test Cases

## Test 1 (Given Test Case Idea)
In or first test case ```test_one``` we are allocate 1 byte and immediately free it 120 time. We run this code 50 times to find the average time of how long the task runs in our test case. We also have the capabilities to handle if there any kind of error of double freeing or out of memory space allocation and deallocation in the fixed size memory

## Test 2 (Given Test Case Idea)

In our second test case ```test_two``` we allocate 1 byte 120 times and then we free the allocated blocks. We run this code 50 times to find the average time of how long the task runs in our test case. We also have the capabilities to handle if there any kind of error of double freeing or out of memory space allocation and deallocation in the fixed size memory

## Test 3 (Custom Made Test Case Idea)
In our third test case, we test the split case of mallocing. In the split case, if we malloc ```n bytes``` of data in ```k byte``` space then the ```k byte``` space ```(n + sizeof(metadata) < k)``` gets split into two blocks b_1 and b_2 where ```b_1``` has payload size of ```n bytes``` and ```b_2``` has payload size of ```k - n - sizeof(header)```.

To test this theory out, we malloced  120 bytes and freed it immediately. Then we allocate 60 bytes into the free 120 byte space which will get split up into 60 bytes (allocated) and 120- 60 - 4 = 56 (sizeof(header) = 4).  Now we free the 60 byte block and allocate 30 bytes and so on. This will go on until we reach 1 byte block. We run this iteration 1000 times to get the work load time. 


## Test 4 (Given Test Case Idea)
1. In our fourth case ```test_four``` we are ```randomly``` allocating 1-byte chunks and storing into the fixed size array. And randomly deallocating the chunks that is allocated beforehand.
2. The remaining allocated blocks that were not randomly freed are freed at the end to avoid memory leaks.
3. We run this code 50 times to find the average time of how long the task runs in our test case. We also have the capabilities to handle if there any kind of error of double freeing or out of memory space allocation and deallocation in the fixed size memory.
    
## Test 5 (Custom Made Test Case Idea)

In our fifth case ```test_five``` we are testing the 4 different cases of ```coalescing``` (see the free mechanism section for more details).

Firstly we initialized the memory completely(4096 bytes) with 4 byte blocks. So every node in the implicit free list has the block size of 4 + 4 (metadata) bytes.

Now to test out coalescing free the entire memory using various coalescing cases. So we developed 3 sub-tests to test out these ```coalescing``` cases. 

## Sub-test 1
Initially since the memory is completely allocated, each metadata node corrosponding to each block of memory has the ```free``` set as 0. So initially the implicit free list's statuses will look something like this: ```free_statuses = [0,0,0,0....,0,0]```.
Note: 0 indicates that the block is allocated and 1 indicates that the block is free.

So the first sub-test will choose the midpoint of ```free_statuses``` and setting it only to free by ```case 1```, so ```free_statuses = [0,0,0,0,0....1....0,0,0,0]```.

Now we run a loop from midpoint_index - 1 to 0 to test our ```case 2``` of our coalescing. So after coalescing ```free_statuses=[1,0,0,0,0,0...0]```

Then we run a loop from midpoint_index + 1 to ```length of free_statuses``` to test our ```case 3``` of our coalescing. So after coalescing ```free_statuses=[1]```.

This sub-test just shows how coalescing works when ```case 1``` ```case 2``` condition and ```case 3``` condition is satisfied.

## Sub-test 2
So the second sub-test will alternatively set the blocks to free with ```spacing=1``` and ```free_statuses = [1,0,1,0,1...0,1,0,1]```.

Now we run a loop from 0 to ```length of free_statuses``` to test our ```case 4``` of our coalescing. So after coalescing ```free_statuses=[1]```

This sub-test just shows how coalescing works when ```case 1``` condition and ```case 4``` condition is satisfied.

## Sub-test 3
So the third sub-test will randomly set the blocks to free with ```spacing=rand() % 5 + 2```. The ```+2``` is added to avoid freeing adjacent blocks because if we don't do that it will defeat the purpose of case 1 coalescing which only works if the previous and next block are allocated and the current block is freed. The randomness spacing is introduced because we can show how all case 2, 3 and 4 work together at once. 

After randomly setting the blocks to free with random spacing through only case 1, it will look like ```free_statuses = [1,0,0,1,0,1,0,0,0,1,0...,0,0,1]```.

Now we run a loop from 0 to ```length of free_statuses``` to test our ```case 4``` of our coalescing. So after coalescing with case 2, case 3, case 4 (notice order of case won't matter due to random placement of the blocks) we get  ```free_statuses=[1]```.

## Test Results
```
Time it took to run test_one: 0.000010 seconds
Time it took to run test_two: 0.000261 seconds
Time it took to run test_three: 0.000418 seconds
Time it took to run test_four: 0.000083 seconds
Time it took to run test_five pattern 1: 0.002394 seconds
Time it took to run test_five pattern 2: 0.002329 seconds
Time it took to run test_five pattern 3: 0.002333 seconds
```

## Terminology
1. ```sizeof(header)``` = size of the metadata
2. ```Implicit Free Lists```: It's linked-list where each node is of type ```struct header```  (have the capabilities to handle Metadata Structure).
3. ```payload_size``` is defined as the bytes that the client has requested.
4. ```block_size``` is defined as the ```payload_size``` + sizeof(header)


# Steps to run 
1. Just run ```make``` from current working directory.
2. Just clean build files run ```make clean```.
