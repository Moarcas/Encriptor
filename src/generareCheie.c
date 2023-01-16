#ifndef __A_H_INCLUDED__
#define __A_H_INCLUDED__

#include "../headers/generareCheie.h"
#include <openssl/rand.h>

void generarePermutare(int *key, int lungimeCuvant)
{
    for(int i = 0; i < lungimeCuvant; i++)
        key[i] = i;

    for(int i = lungimeCuvant - 1; i > 0; i--)
    {
        int j;
        RAND_bytes((unsigned char*)j, sizeof(j));
        j = j % (i + 1);

        int temp = key[i];
        key[i] = key[j];
        key[j] = temp;
    }
}

#endif