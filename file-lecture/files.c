#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define OUT "out.txt"

#define wrap(X) if ((X) == -1) { perror("FATAL ERROR")}