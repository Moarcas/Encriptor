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
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */

#include "../headers/pozitii.h"
#include "../headers/generareCheie.h"

#endif

void cripitareCuvant(char cuvant[], int lungime_cuvant, int nr_cuv)
{
    // Generez cheia

    int key[lungime_cuvant];

    generarePermutare(key, lungime_cuvant);

    // Criptez cuvantul

    char *cuvant_criptat = malloc((lungime_cuvant + 1) * sizeof(char));

    for(int i = 0; i < lungime_cuvant; i++)
    {
        int index = key[i] % lungime_cuvant;
        cuvant_criptat[i] = cuvant[index];
    }

    cuvant_criptat[lungime_cuvant] = '\0';
    strcpy(cuvant, cuvant_criptat);
    free(cuvant_criptat);

    // Salvez cheia 

}

int criptare(char **argv)
{
    printf("Sunt in sursa criptare!\n");

    // Deschid fisierul de intrare

    int fd = open(argv[1], O_RDWR);
    struct stat sb;

    if(fstat(fd, &sb) == -1)
    {
        perror("Nu s-a putut obtine dimensiunea fisierului de intrare.\n");
        return errno;
    }

    // Mapez fisierul de intrare in memorie

    char *file_in_memory = mmap(NULL, sb.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    // Deschid fisierul de permutari

    int fd_permutari = shm_open("permutari.txt", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);

    if(fd_permutari < 0)
    {
        perror(NULL);
        return errno;
    }
    if(ftruncate(fd_permutari, 1000) == -1)
    {
        perror(NULL);
        shm_unlink("permutari.txt");
        return errno;
    }

    char *file_permutari = mmap(NULL, 1000, PROT_READ|PROT_WRITE, MAP_SHARED, fd_permutari, 0);

    if(file_permutari == MAP_FAILED) 
    {
        perror(NULL);
        shm_unlink("permutari.txt");
        return errno;
    }

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

            cripitareCuvant(cuvant, lungime_cuvant, nr_cuv);

            for(int i = 0; i < lungime_cuvant; i++)
                file_in_memory[pozitie_start + i] = cuvant[i];
            
            exit(0);
        }

    }

    munmap(file_in_memory, sb.st_size);
    
    for(int i = 0; i < numar_cuvinte; i++)
        wait(NULL); 

    return 0;    
}