Mymalloc and Myfree project


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
