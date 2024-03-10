#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "atr.h"
#include "Liste.h"
#include "Levenshtein.h"

int main(int argc, char ** argv){
    ATR lexique;
    Liste erreurs, tmp, corrections;
    FILE * texte;
    if (argc != 3){
        printf("Usage: ./Prog fichier.txt dictionnaire.dico\n");
        exit(EXIT_FAILURE);
    }
    /* Chargement du dictionnaire dans l'arbre lexico.*/
    lexique = construire_ATR(argv[2]);
    /* On cherche les erreur du fichier (les mots qui sont dans le fichier texte mais pas present dans le dico)*/
    erreurs = recherche_erreurs(lexique, argv[1]);

    texte = fopen(argv[2], "r");
    if (!texte) 
        exit(EXIT_FAILURE);
    tmp = erreurs;
    while (tmp){
        fseek(texte, 0, SEEK_SET);
        corrections = force_brute(tmp->mot, texte);
        printf("Pour le mot '%s' voici les corrections possibles : \n", tmp->mot);
        afficher_Liste(corrections);
        printf("\n------------------------\n");
        tmp = tmp->suivant;
        liberer_Liste(&corrections);
    }
    liberer_ATR(&lexique);
    liberer_Liste(&erreurs);
    return 0;
}