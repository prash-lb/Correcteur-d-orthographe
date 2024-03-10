#include "atr.h"
#include "Liste.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LETTRE 40

int main(int argc, char **argv){
    ATR lexique;
    Liste erreurs;
    if (argc != 3){
        printf("Usage: ./Prog fichier.txt dictionnaire.dico\n");
        exit(EXIT_FAILURE);
    }
    /* Chargement du dictionnaire dans l'arbre lexico.*/
    lexique = construire_ATR(argv[2]);
    /* On cherche les erreur du fichier (les mots qui sont dans le fichier texte mais pas present dans le dico)*/
    erreurs = recherche_erreurs(lexique, argv[1]);
    printf("Voici la liste des erreurs : \n");
    afficher_Liste(erreurs);
    liberer_Liste(&erreurs);
    liberer_ATR(&lexique);
    return 0;
}