TARGET     = $(file1)
CC         = clang
SANITIZERS = -fsanitize=address 
OPT        =
CFLAGS     = -g -std=c99 -Wall -Wvla -Werror $(SANITIZERS) $(OPT)

fc: gcc memgrind.c mymalloc.c -o mem && ./mem

clean:
	rm -rf $(TARGET) *.o *.a *.dylib *.dSYM