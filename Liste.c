#include "Liste.h"
#include "Levenshtein.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


/**
 * @brief Alloue de la memoire pour le mot dans la liste
 * 
 * @param mot char
 * @return Cellule* 
 */
Cellule* alloue_cellule(char * mot){
    Liste new = malloc(sizeof(Cellule));
    if (new){
        strcpy(new->mot, mot);
        new->suivant = NULL;
    }
    return new;
}

/**
 * @brief On ajoute le mot dans L 
 * 
 * @param L pointeur (L)
 * @param mot  char
 * @return int (integer)
 */
int inserer_en_tete(Liste * L, char * mot){
    Liste mot_1 = alloue_cellule(mot);
    if(mot_1 == NULL){
        return 0;
    }
    if (*L == NULL){
        *L = mot_1;
        return 1;
    }
    else{
        mot_1->suivant = *L;
        *L = mot_1;
        return 1;
    }
}


/**
 * @brief libere tous les mot de la liste de la memoire 
 * 
 * @param L pointeur (Liste)
 */
void liberer_Liste(Liste * L){
    if((*L) == NULL){
        return;
    }
    liberer_Liste(&((*L)->suivant));
    free(*L);
    *L = NULL;
}


/**
 * @brief affiche les mot de la liste 
 * 
 * @param L (Liste)
 */
void afficher_Liste(Liste L){
    Liste mot_2 = L;
    if(L==NULL){
        printf("Liste vide\n");
        return;
    }
    while(mot_2){
        printf("%s\n",mot_2->mot);
        mot_2 = mot_2->suivant;
    }
}

/**
 * @brief Fonction qui va chercher les erreurs en regardant si le mot du texte est present dans l arbre ATR ou non.
 * 
 * @param A (ATR)
 * @param chemin  (char)
 * @return Liste (Liste)
 */
Liste recherche_erreurs(ATR A, char *chemin){
    Liste erreurs = NULL;
    char tab[MAX_LETTRE], *delimiteur;
    int i;
    FILE *texte = fopen(chemin, "r");
    if (!texte) {
        printf("Erreur de lecture\n");
        return NULL;
    }

    /* Recherche des erreurs.*/
    while (fscanf(texte, "%s", tab) != EOF){
        
        for (i = 0; tab[i] != '\0'; i++){
            /* on stope les mots aux ponctuation */
            if (tab[i] == '.' || tab[i] == '!' || tab[i] == '?' || tab[i] == ':' || tab[i] == ';' || tab[i] == ','){
                tab[i] = '\0';
            }
            else{/* On met le mot en minuscule.*/
            tab[i] = tolower(tab[i]);
            }
        }
        if ((delimiteur = strchr(tab, '-')) != NULL){
            if (strlen(tab) - strlen(delimiteur) == 1){
                if (!recherche_ATR(A, delimiteur + 1)){
                    inserer_en_tete(&erreurs, delimiteur + 1);
                }
            }
            else{
                if (!recherche_ATR(A, delimiteur + 1)){
                    inserer_en_tete(&erreurs, delimiteur + 1);
                }
                *delimiteur = '\0';
                if (!recherche_ATR(A, tab)){
                    inserer_en_tete(&erreurs, tab);
                }
            }
        
        }
        else if ((delimiteur = strchr(tab, '\'')) != NULL){
            if (strlen(tab) - strlen(delimiteur) == 1){
                if (!recherche_ATR(A, delimiteur + 1)){
                    inserer_en_tete(&erreurs, delimiteur + 1);
                }
            }
            else{
                if (!recherche_ATR(A, tab)){
                    inserer_en_tete(&erreurs, tab);
                }
            }
        }
        else {
            if (!recherche_ATR(A, tab)){
                inserer_en_tete(&erreurs, tab);
            }
        }
    }
    fclose(texte);
    return erreurs;
}

Liste force_brute(char *mot, FILE *texte){
    Liste corrections = NULL;
    int dmin = 10000, d;
    char buffer[40];
    while (fscanf(texte, "%s", buffer) != EOF){
        d = DistanceDeLevenshtein(buffer, mot);
        if (d <= dmin){
            if (d < dmin){
                dmin = d;
                liberer_Liste(&corrections);
            }
            inserer_en_tete(&corrections, buffer);
        }
    }
    return corrections;
}

