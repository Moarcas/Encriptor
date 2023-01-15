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
        criptare(argv);
    }
    else if(argc == 3)
    {
        // Drecriptare
        decriptare(argv);
    }
    else
    {
        // Numarul argumentelor este gresit
        printf("Programul primeste minim un argument si maxim doua argumente!");
    }


    return 0;
}