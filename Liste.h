#ifndef __LISTE__
#define __LISTE__
#include <stdio.h>
#include "atr.h"
#include "Levenshtein.h"

#define MAX_LETTRE 40

    typedef struct liste{
        char mot[MAX_LETTRE];
        struct liste* suivant;
    } Cellule, *Liste;

    Cellule *  alloue_cellule(char * mot);
    int inserer_en_tete(Liste * L, char * mot);
    void liberer_Liste(Liste * L);
    void afficher_Liste(Liste L);
    Liste recherche_erreurs(ATR lexique, char *chemin);
    Liste force_brute(char *mot, FILE *texte);

#endif