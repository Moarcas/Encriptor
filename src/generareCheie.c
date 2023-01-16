#ifndef __A_H_INCLUDED__
#define __A_H_INCLUDED__

#include "../headers/generareCheie.h"
#include <time.h>
#include <stdlib.h>

#endif

void generarePermutare(int *key, int lungimeCheie)
{
    for(int i = 0; i < lungimeCheie; i++)
        key[i] = i;

    srand(time(NULL));

    for(int i = lungimeCheie - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        
        int temp = key[i];
        key[i] = key[j];
        key[j] = temp;
    }
}
