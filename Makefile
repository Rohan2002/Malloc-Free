CFLAGS     = -std=c99 -Wall

build: 
	gcc $(CFLAGS) memgrind.c mymalloc.c -o mem.o && ./mem.o

clean:
	rm -rf *.o