#ifndef __A_H_INCLUDED__
#define __A_H_INCLUDED__

#include <stdio.h>
#include "../headers/criptare.h"
#include "../headers/decriptare.h"

#endif

int main(int argc, char *argv[]) 
{
    
    if(argc == 2)
    {
        // Ciptare
        if(criptare(argv) == 1)
            printf("Mesajul a fost criptat cu succes!\n");
        else
            printf("Eroare la criptarea mesajului!\n");
    }
    else if(argc == 3)
    {
        // Drecriptare
        if(decriptare(argv) == 1)
            printf("Mesajul a fost decriptat cu succes!\n");
        else
            printf("Eroare la decriptarea mesajului!\n");
    }
    else
    {
        // Numarul argumentelor este gresit
        printf("Programul primeste minim un argument si maxim doua argumente!\n");
    }


    return 0;
}