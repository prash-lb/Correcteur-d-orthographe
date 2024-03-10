#include "atr.h"
#include "Levenshtein.h"
#include "bk.h"
#include "Liste.h"
#include <ctype.h>

int main(int argc, char ** argv){
    if (argc != 3){
        printf("Usage: ./Prog fichier.txt dictionnaire.dico\n");
        exit(EXIT_FAILURE);
    }
    FILE *dico = fopen(argv[2], "r");
    ArbreBK lexique = creer_ArbreBK(dico);
    Liste erreurs = NULL, corrections = NULL, tmp, tmp2;
    int i;
    char tab[MAX_LETTRE];
    FILE *texte = fopen(argv[1], "r");
    if (!texte) {
        printf("Erreur de lecture\n");
        exit(EXIT_FAILURE);
    }
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
        if(!est_dans_ArbreBK(lexique, tab))
            inserer_en_tete(&erreurs, tab);
    }
    tmp = erreurs;
    while (tmp){
        corrections = rechercher_dans_ArbreBK(lexique, tmp->mot);
        printf("Pour le mot '%s' voici les corrections possibles : \n", tmp->mot);
        tmp2 = corrections;
        while (tmp2->suivant){
            printf("%s, ", tmp2->mot);
            tmp2 = tmp2->suivant;
        }
        printf("%s\n", tmp2->mot);
        tmp = tmp->suivant;
        liberer_Liste(&corrections);
    }
    /*afficher_ArbreBK(lexique);*/
    liberer_ArbreBK(&lexique);
    fclose(texte);
    fclose(dico);
    return 0;
}
