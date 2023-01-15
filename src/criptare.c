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
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#include "../headers/pozitii.h"

#endif

int criptare(char **argv)
{
    printf("Sunt in sursa criptare!\n");

    // Deschid fisierul de intrare

    const int dimensiune_pagina = getpagesize();
    int fd = open(argv[1], O_RDWR);
    struct stat sb;

    if(fstat(fd, &sb) == -1)
    {
        perror("Nu s-a putut obtine dimensiunea fisierului.\n");
        return errno;
    }

    off_t dimensiune_criptata = 0;

    while(dimensiune_criptata + dimensiune_pagina <= sb.st_size)
    {
        pid_t pid = fork();

        if(pid < 0)
            return errno;
        
        if(pid == 0)
        {
            char* file_in_memory = mmap(NULL, dimensiune_pagina, PROT_READ|PROT_WRITE, MAP_SHARED, fd, dimensiune_criptata);


            exit(0);
        }

        dimensiune_criptata += dimensiune_pagina;
    }

    return 0;    
}