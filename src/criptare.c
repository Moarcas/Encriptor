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
#include <stdlib.h>

#include "../headers/pozitii.h"
#include "../headers/generareCheie.h"

#endif

void cripitareCuvant(char cuvant[], int lungime_cuvant)
{
    // Generez cheia

    int key[lungime_cuvant];

    generarePermutare(key, lungime_cuvant);

    // Criptez cuvantul

}

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


    for(int nr_cuv = 0; nr_cuv < numar_cuvinte; nr_cuv++)
    {
        pid_t pid = fork();

        if(pid < 0)
            return 0;
        
        if(pid == 0)
        {
            int pozitie_start = pozitii[nr_cuv].pozitieStart;
            int lungime_cuvant = pozitii[nr_cuv].lungimeCuvant;

            char cuvant[lungime_cuvant + 1];

            for(int i = 0; i < lungime_cuvant; i++)
                cuvant[i] = file_in_memory[pozitie_start + i];

            cuvant[lungime_cuvant] = '\0';

            cripitareCuvant(cuvant, lungime_cuvant);

            for(int i = 0; i < lungime_cuvant; i++)
                file_in_memory[pozitie_start + i] = cuvant[i];
            
            exit(0);
        }

    }

    munmap(file_in_memory, sb.st_size);
    

    return 0;    
}