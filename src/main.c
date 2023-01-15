//read
#include <unistd.h>

// open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//printf
#include <stdio.h>

//errno
#include <errno.h>

//stat
#include <sys/stat.h>
#include <sys/types.h>

//mmap
#include <sys/mman.h>

#include <string.h>

//shm_open
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */


int main(int argc, char *argv[]) 
{
    printf("Hello World!\n");
    return 0;
}