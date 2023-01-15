#ifndef __A_H_INCLUDED__
#define __A_H_INCLUDED__

#include <stdio.h>
#include <string.h>

#include "../headers/pereche.h"

#endif

void determinarePozitiiCuvinte(struct pereche pozitii[], char* file_in_memory, int *numar_cuvinte)
{ 
    char copie[strlen(file_in_memory)];

    strcpy(copie, file_in_memory);

    char *cuvant = strtok(copie, " ,.;!?\n");

    while(cuvant != NULL)
    {
        pozitii[*numar_cuvinte].pozitieStart = cuvant - copie;
        pozitii[*numar_cuvinte].lungimeCuvant = strlen(cuvant);

        (*numar_cuvinte)++;

        cuvant = strtok(NULL, " .,;!?\n");
    }
}