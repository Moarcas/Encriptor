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
#include <sys/types.h>
#include <sys/wait.h>

#include "../headers/pozitii.h"

#endif

// Aceasta functie cripeaza doar un singur cuvant
void cripitareCuvant(char cuvant[])
{
    // aici fac permutarea

}

// Aceasta functie cripiteaza doar o parte din fisier, de dimensiunea paginii
void criptarePartiala(char *file_in_memory)
{   
    struct pereche pozitii[10000];
    int numar_cuvinte = 0;

    determinarePozitiiCuvinte(pozitii, file_in_memory, &numar_cuvinte);

    for(int nr_cuvant = 0; nr_cuvant < numar_cuvinte; nr_cuvant++)
    {
        int lungime_cuvant = pozitii[nr_cuvant].lungimeCuvant;
        int pozitie_start = pozitii[nr_cuvant].pozitieStart;

        // Copiez cuvantul din fisier intr-o alta variabila 

        char cuvant[lungime_cuvant + 1];

        for(int i = 0; i < lungime_cuvant; i++)
            cuvant[i] = file_in_memory[pozitie_start + i];
        
        cuvant[lungime_cuvant] = '\0';

        cripitareCuvant(cuvant);

        // Copiez cuvantul modificat inapoi in fisier

        for(int i = 0; i < lungime_cuvant; i++)
            file_in_memory[pozitie_start + i] = cuvant[i];
    }
}

int criptare(char **argv)
{
    printf("Sunt in sursa criptare!\n");

    // Deschid fisierul de intrare

    const int dimensiune_pagina = getpagesize();
    int fd = open(argv[1], O_RDWR);
    struct stat sb;
    int nr_procese_create = 0;

    if(fstat(fd, &sb) == -1)
    {
        perror("Nu s-a putut obtine dimensiunea fisierului.\n");
        return errno;
    }

    off_t dimensiune_criptata = 0;

    while(dimensiune_criptata + dimensiune_pagina <= sb.st_size)
    {
        nr_procese_create++;
        pid_t pid = fork();

        if(pid < 0)
            return errno;
        
        if(pid == 0)
        {
            char* file_in_memory = mmap(NULL, dimensiune_pagina, PROT_READ|PROT_WRITE, MAP_SHARED, fd, dimensiune_criptata);

            criptarePartiala(file_in_memory);
            
            munmap(file_in_memory, dimensiune_pagina);
            exit(0);
        }

        dimensiune_criptata += dimensiune_pagina;
    }

    if(dimensiune_criptata < sb.st_size)
    {
        nr_procese_create++;
        pid_t pid = fork();

        if(pid < 0)
            return errno;
        
        if(pid == 0)
        {
            char *file_in_memory = mmap(NULL, sb.st_size - dimensiune_criptata, PROT_READ|PROT_WRITE, MAP_SHARED, fd, dimensiune_criptata);

            criptarePartiala(file_in_memory);           

            munmap(file_in_memory, sb.st_size - dimensiune_criptata);
            exit(0);
        }
    }

    for(int i = 0; i < nr_procese_create; i++)
        wait(NULL);

    printf("Numarul proceselor create este: %d\n", nr_procese_create);

    return 0;    
}