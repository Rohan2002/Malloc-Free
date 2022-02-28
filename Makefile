SANITIZERS = -fsanitize=address 
CFLAGS     = -g -std=c99 -Wall -Wvla -Werror $(SANITIZERS)

build: 
	gcc $(CFLAGS) memgrind.c mymalloc.c -o mem.o && ./mem.o

clean:
	rm -rf $(TARGET) *.o *.a *.dylib *.dSYM