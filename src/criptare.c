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
#include <pthread.h>

#include "../headers/pozitii.h"
#include "../headers/generareCheie.h"

#endif

pthread_mutex_t *mutex;

void cripitareCuvant(char cuvant[], int lungime_cuvant, int nr_cuv, char* file_permutari)
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

    pthread_mutex_lock(mutex);

    // // Pe prima pozitie pun numarul cuvantului, apoi cheia asociata acestuia

    int length = snprintf(NULL, 0, "%d", nr_cuv);
    char *numar = malloc(length + 1);
    snprintf(numar, length + 1, "%d", nr_cuv);
    strcat(file_permutari, numar);
    free(numar);

    for(int i = 0; i < lungime_cuvant; i++)
    {
        length = snprintf(NULL, 0, "%d", key[i]);
        numar = malloc(length + 1);
        snprintf(numar, length + 1, "%d", key[i]);
        strcat(file_permutari, " ");
        strcat(file_permutari, numar);
        free(numar);
    }
    strcat(file_permutari, "\n");

    pthread_mutex_unlock(mutex);
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

    int fd_permutari = open("permutari.txt", O_RDWR);

    if(fd_permutari < 0)
    {
        perror(NULL);
        return errno;
    }

    ftruncate(fd_permutari, 4096);

    char *file_permutari = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd_permutari, 0);

    if(file_permutari == MAP_FAILED) 
    {
        perror(NULL);
        munmap(file_permutari, 10000);
        return errno;
    }
    
    // Acum orice modificare fac in file_in_memory va aparea si in fisier

    struct pereche pozitii[100000];
    int numar_cuvinte = 0;

    // Pentru fiecare cuvant din fisier determin pozitia la care incepe si pozitia la care se termina

    determinarePozitiiCuvinte(pozitii, file_in_memory, &numar_cuvinte);

    // Mapez mutexul

    mutex = mmap(NULL, sizeof(pthread_mutex_t), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);

    pthread_mutex_init(mutex, NULL);

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

            cripitareCuvant(cuvant, lungime_cuvant, nr_cuv, file_permutari);

            for(int i = 0; i < lungime_cuvant; i++)
                file_in_memory[pozitie_start + i] = cuvant[i];
            
            exit(0);
        }

    }

    pthread_mutex_destroy(mutex);
    munmap(mutex, sizeof(pthread_mutex_t));

    munmap(file_in_memory, sb.st_size);
    
    for(int i = 0; i < numar_cuvinte; i++)
        wait(NULL); 

    ftruncate(fd_permutari, strlen(file_permutari));
    munmap(file_permutari, 4096);
    close(fd_permutari);

    return 0;    
}

// sa inchid fisierele