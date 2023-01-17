#ifndef __A_H_INCLUDED__
#define __A_H_INCLUDED__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <wait.h>

#include "../headers/decriptare.h"
#include "../headers/pozitii.h"

#endif

void decriptareCuvant(char cuvant[], int lungime_cuvant, int *permutare)
{
    char *cuvant_decriptat = malloc((lungime_cuvant + 1) * sizeof(char));

    for(int i = 0; i < lungime_cuvant; i++)
        cuvant_decriptat[permutare[i]] = cuvant[i];
    
    cuvant_decriptat[lungime_cuvant] = '\0';
    strcpy(cuvant, cuvant_decriptat);
}

int decriptare(char **argv)
{
    printf("Sunt in sursa decriptare!\n");

    // Deschid fisierul de intrare

    int fd = open(argv[1], O_RDWR);
    struct stat sb;

    if(fstat(fd, &sb) == -1)
    {
        perror("Nu s-a putut abtine dimensiunea fisierului de intrare.\n");
        return errno;
    }

    // Mapez fisierul de intrare in memorie

    char *file_in_memory = mmap(NULL, sb.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    // Deschid fisierul de permutari

    int fd_permutari = open(argv[2], O_RDWR);

    if(fd_permutari < 0)
    {
        perror(NULL);
        return errno;
    }
    
    FILE *file = fdopen(fd_permutari, "r");

    if(file == NULL)
    {
        perror("Error: ");
        return errno;
    }

    char linie[100];

    int **permutari = (int **)malloc(100000 * sizeof(int*));

    int cheie[100];
    int numar_cuvinte = 0;

    while(fgets(linie, 100, file) != NULL)
    {
        char* numar = strtok(linie, " ");

        int index_cuvant = atoi(numar);

        numar = strtok(NULL, " \n");

        int numar_litere = 0;

        while(numar != NULL)
        {
            cheie[numar_litere++] = atoi(numar);
            numar = strtok(NULL, " \n");
        }
    
        permutari[index_cuvant] = (int *)malloc((numar_litere + 1) * sizeof(int));

  
        for(int i = 0; i < numar_litere; i++)
            permutari[index_cuvant][i] = cheie[i];

        permutari[index_cuvant][numar_litere] = -1;

        numar_cuvinte++;
    }

    struct pereche pozitii[100000];
    numar_cuvinte = 0;

    determinarePozitiiCuvinte(pozitii, file_in_memory, &numar_cuvinte);

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

            decriptareCuvant(cuvant, lungime_cuvant, permutari[nr_cuv]);

            for(int i = 0; i < lungime_cuvant; i++)
                file_in_memory[pozitie_start + i] = cuvant[i];

            exit(0);
        }
    }

    munmap(file_in_memory, sb.st_size);

    for(int i = 0; i < numar_cuvinte; i++)
        wait(NULL);

    ftruncate(fd_permutari, 0);

    return 0;
}