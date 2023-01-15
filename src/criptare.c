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

#include "../headers/pozitii.h"

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

    struct pereche pozitii[100000];
    int numar_cuvinte = 0;

    // Pentru fiecare cuvant din fisier determin pozitia la care incepe si pozitia la care se termina

    determinarePozitiiCuvinte(pozitii, file_in_memory, &numar_cuvinte);

    // Incep sa creez procesele

    

    return 0;    
}