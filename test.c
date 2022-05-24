#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


int main(){
    char* salut = "test.txt";
    char* chien = "r";
    FILE* fichier = fopen(salut,chien);
    fseek(fichier,0, SEEK_END);
    int len = ftell(fichier);
    fseek(fichier,0,SEEK_SET);
    char* texte = malloc(sizeof(char) * (len+1));
    fread(texte,1,len,fichier);
    printf("%s",texte);
}