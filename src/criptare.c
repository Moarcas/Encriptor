#ifndef __A_H_INCLUDED__
#define __A_H_INCLUDED__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>

#include "../headers/pereche.h"

#endif

int criptare(char **argv)
{
    printf("Sunt in sursa criptare!\n");

    // Deschid fisierul de intrare

    int fd = open(argv[1], O_RDWR);
    struct stat sb;

    if(fstat(fd, &sb) == -1)
    {
        perror("Nu s-a putut obtine dimensiunea fisierului.\n");
        return errno;
    }

    // Mapez fisierul de intrare in memorie

    char *file_in_memory = mmap(NULL, sb.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    // Acum orice modificare fac in file_in_memory va aparea si in fisier




    return 0;    
}