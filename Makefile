TARGET     = $(file)
CC         = clang
SANITIZERS = -fsanitize=address
OPT        =
CFLAGS     = -g -std=c99 -Wall -Wvla -Werror $(SANITIZERS) $(OPT)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(TARGET) *.o *.a *.dylib *.dSYM