Mymalloc and Myfree project

1. ## Metadata Structure: struct header
    1. Our metadata is a 4 byte or 32 bits block.
    2. We assigned each attribute in the metadata struct certain amount of bits and thus the total number of bits add
    up to 32 bits.
    3. We allocated 13 bits for the curent_size of the block because the maximum size of any block can be 4096 bytes which is 2^13
    4. We allocated 13 bits for the prev_size of the block because the maximum size of any block can be 4096 bytes which is 2^13
    5. We allocated 1 bit ```last_node``` field in the metadata to indicate wether the current block is the last node or not. If the current block is the last node then ```last_node = 1``` else ```last_node = 0```.
    6. We allocated 1 bit ```first_node``` field in the metadata to indicate wether the current block is the first node or not. If the current block is the first node then ```first_node = 1``` else ```first_node = 0```.
    7. We had 2 bits as free space in the struct because we didn't have to store any data fields in that attribute, so preciesly, our struct only allocates 30 bits, however due to the total size of an int (4 bytes or 32 bits), we have to keep those 2 bits as padding.
    8. We allocated 1 bit ```free``` field in the metadata to indicate wether the current block is free or not. If the current block is free then ```free = 1``` else ```free = 0```.
    9. We allocated 1 bit ```prev_free``` field in the metadata to indicate wether the previous block is free or not. If the current block is free then ```prev_free = 1``` else ```prev_free = 0```.

2. ## Malloc Functionality
    1. We defined a function ```void *mymalloc(size_t requested_size, char *file, int line)``` that takes in 3 arguements in total. The first arguement specifies the ```payload size``` that the client requests, the second argument and third arguement specifies the filename.c and line number from ```where``` malloc was called by the client. 
    2. ```mymalloc()``` returns an ```address or pointer(void*)``` to memory where the requested payload size will be allocated in memory.
    3. The malloc functionality works around implict free lists, and the way it works is as follows:

    4. For the basic error checking, we first check whether the number of bytes requested by the client is allowed by checking if its a positive number and it is less than ```4096 bytes - sizeof(header).```
    5. Now if the number of bytes is valid, we proceed by getting the base pointer/address of the first index of memory. We creating a initial metadata by pointing to this base address of memory. This inital metadata consist of the initial block size which is 4096 bytes, and it's a free block initially.
    6. Now suppose the user requests ```n bytes``` and if ```n + sizeof(header)``` is less than ```current_block_size``` then we split the ```current_block``` into sub-blocks ``b_1`` and ``b_2`` where ```b_1``` will be ```allocated``` have size ```n + sizeof(header)``` and ```b_2``` will be ```freed``` and have a size of ```current_block_size - n - sizeof(header)```. Then we return the address to the  ```payload NOT the header``` of ```b_1``.
    7. Now suppose the user request ```n bytes``` and if ```n + sizeof(header)``` is equal to ```current_block_size``` then we just set ```current_block``` as ```allocated``` status because we have found the exact number of byte that is available in memory. Then we return the address to the ```payload NOT the header``` of ```current_block```.
    8. Now suppose the user request ```n bytes``` and there isn't space for n bytes after searching through the metadata linked list then we print out the error message indicating that the number of bytes requested isn't available in memory, and thus return ```NULL```.

3. ## Free Functionality
    1. We defined a function void ```*myfree(void* ptr, char* file, int line)``` that takes in 3 arguements in total. The first arguement specifies the pointer of the payload in memory that is to be freed, the second argument and third arguement specifies the filename.c and line number from where malloc was called by the client.
    2. First we check if the payload pointer is in memory by iterating through each element of memory less than or equal to 4096 times. The run time complexity of this search is ```O(1)``` because we are searching through a ```constant``` amount of memory. 
    3. If we don't find the address in memory then we can indicate an error saying that the address passed to memory is invalid.
    4. If the address is valid and found in memory, but the metadata pointing to the address is already freed then we specify a double free error.
    6. Since the metadata of each block of memory consist of the previous block size, previous free status, the current block size and the current free status, thus, we implement a ```boundary tag coalescing``` technique for constant time run time efficiency. 
    7. There are 4 cases of coalescing that we took care off when we were designing our coalescing mechanism. Let's assume that the ```current_node``` exists and is the node that we want to free and we also that the previous and next metadata nodes also exist. 
        -  To check if previous and next node exist, we check the ```first_node``` status and ```last_node``` status of the ```current_node``` 
        First to check whether previous and next metadata node exist, we also stored whether each node is the ```first_node``` and ```last_node```. If the current node that we are freeing
        Case 1: Assuming that the previous and next metadata nodes exist, 


4. ## Terminology
    1. ```sizeof(header)``` = size of the metadata
    2. ```Implicit Free Lists```: It's linked-list where each node is of type ```struct header```  (See Metadata Structure).










```To run mymalloc```

```make file=mymalloc && ./mymalloc```

```To pull the code from git```
1. ```git stash clear```
2. ```git stash```
3. ```git pull origin master```
4. ```git stash pop```


```To push the code to git```
Make sure to pull any code from git before adding to git.

1. ```git add .```
2. ```git commit -m "<some-descriptive-message>"```
3. ```git push origin master```


Test plan for PA1

1. How will I know my program is correct?
2. What does it mean for my profgram to be correct?

My programs should be free of bugs
    -> true, but vague.

Think about what should my code not do?

malloc() should allocate memory
free() should deallocate memory
    -> better, but still too vague.


If I allocate N bytes, a get back apointer, I should be able to write anything
    to those bytes without interfering with mallocs data structure (metadata).

    (b) any other allocated chunks

If I try to allocate more mem than exists, then return NULL;

If I allocate all of memory, I should not allocate more.

if I allocate all of memory, then free it. I should be able to allocate more

If I allocate all of memory in multiple chunks,
and free everything in a different order.

how can you detect metadata corruption?
-   Sanity check functions/ detect corrupt metadata(where possible)
-   shadow memory for testing


10524-10549