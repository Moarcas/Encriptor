#ifndef __A_H_INCLUDED__
#define __A_H_INCLUDED__

#include <stdio.h>
#include <string.h>

#include "../headers/pereche.h"

#endif

void determinarePozitiiCuvinte(struct pereche pozitii[], char* file_in_memory)
{ 
    char copie[strlen(file_in_memory)];

    strcpy(copie, file_in_memory);

    char *cuvant = strtok(copie, " ,.;!?\n");

    int numar_cuvinte = 0;

    while(cuvant != NULL)
    {
        pozitii[numar_cuvinte].pozitieStart = cuvant - copie;
        pozitii[numar_cuvinte].pozitieFinal = pozitii[numar_cuvinte].pozitieStart + strlen(cuvant);

        cuvant = strtok(NULL, " .,;!?\n");
    }
}


