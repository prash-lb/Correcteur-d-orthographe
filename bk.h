#ifndef __BK__
#define __BK__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Levenshtein.h"
#include "atr.h"
#include "Liste.h"

    typedef struct noeudBK {
        char * mot;
        int valeur;
        struct noeudBK * filsG;
        struct noeudBK * frereD;
    } NoeudBK, * ArbreBK;

    int inserer_dans_ArbreBK(ArbreBK * A, char * mot);
    Liste rechercher_dans_ArbreBK(ArbreBK A, char * mot);
    int est_dans_ArbreBK(ArbreBK A, char * mot);
    ArbreBK creer_ArbreBK(FILE * dico);
    void liberer_ArbreBK(ArbreBK * A);
    void afficher_ArbreBK(ArbreBK A);
    void creePDF (char * dot, char * pdf, ArbreBK a);
    void dessine(FILE *fichier, ArbreBK A);
    void ecrireFin(FILE *fichier);
    void ecrireArbreBK(FILE *fichier, ArbreBK A);
    void ecrirDebut(FILE *fichier);



#endif