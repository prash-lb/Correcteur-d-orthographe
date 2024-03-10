#include "atr.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_LETTRE 40


/**
 * @brief Creé l'abre dans la memoire
 * 
 * @return ATR 
 */
ATR creer_ATR_vide(){
    ATR nv_arbre = (ATR)malloc(sizeof(Noeud));
    if (nv_arbre != NULL)
    {
        nv_arbre->lettre = ' ';
        nv_arbre->d = NULL;
        nv_arbre->g= NULL;
        nv_arbre->fils = NULL;
        
    }
    else{
        printf("ERREUR MEMOIRE NON ALLOUE");
    }
    return nv_arbre;
}


/**
 * @brief Fonction qui va liberer l'arbre de la memoire
 * 
 * @param A pointeur (ATR)
 */
void liberer_ATR(ATR * A){
    if(*A == NULL)
        return;
    liberer_ATR(&((*A)->g));
    liberer_ATR(&((*A)->d));
    liberer_ATR(&((*A)->fils));
    free(*A);
    *A = NULL;
}


/**
 * @brief Fonction qui ajoute un mot dans l'arbre
 * 
 * @param A pointeur (ATR)
 * @param mot char
 * @return int (integer)
 */
int inserer_dans_ATR(ATR * A, char * mot){
    ATR tmp;
    if(*A == NULL){/*Si l arbre est vide on la cree et on va inserer lettre par lettre le mot dans l arbre*/
        *A = creer_ATR_vide();
        if (!*A) return 0;
        if (strlen(mot) == 1){
            (*A)->lettre = mot[0];
            (*A)->fils = creer_ATR_vide();
            (*A)->fils->lettre = '\0';
            return 1;
        }
        (*A)->lettre = mot[0];
        inserer_dans_ATR(&((*A)->fils), mot + 1);
        return 1;
    }
    if((*A)->lettre  == '\0' && strlen(mot) == 0){/*Si on arrive a la fin du mot et que la lettre qui se situe dans le noeud est un \0 alors sa veut dire que le mot est deja present */
        return 0;
    }
    if((*A)->lettre  != '\0' && strlen(mot) == 0){/*Si on arrive a la fin du mot et que la lettre qui se situe dans le noeud n est pas  un \0 alors sa veut dire que le mot est present mais il manque le \0  donc on va le rajouter */
        tmp = creer_ATR_vide();
        if (!tmp) return 0;
        tmp->lettre = (*A)->lettre;
        tmp->g = (*A)->g;
        tmp->d = (*A)->d;
        tmp->fils = (*A)->fils;
        (*A)->fils = NULL;
        (*A)->g = NULL;
        (*A)->d = NULL;
        (*A)->d = tmp;
        (*A)->lettre = '\0';
        tmp = NULL;
        return 1;
    }
    /*appel recursive */
    if ((*A)->lettre == mot[0]){
        inserer_dans_ATR(&((*A)->fils), mot + 1);
    }
    if ((*A)->lettre > mot[0]){
        inserer_dans_ATR(&((*A)->g), mot);
    }
    if ((*A)->lettre < mot[0]){
        inserer_dans_ATR(&((*A)->d), mot);
    }
    return 1;
}


/**
 * @brief Supprime le mot de l'abre
 * 
 * @param A pointeur (ATR)
 * @param mot char
 */
void supprimer_dans_ATR(ATR * A, char * mot){
    ATR tmp = *A;
    ATR tmp2;
    if (*A == NULL){
        /* Si l 'arbre est vide */
        return;
    }
    if (strlen(mot) == 0 && (*A)->lettre != '\0'){
        /*Si le mot n est pas la */
        return;
    }
    if (strlen(mot) == 1 && (*A)->fils && (*A)->fils->lettre == '\0'&& (*A)->fils->d == NULL){
        /* Supprimer le cas ou on arrive avant  un \0 et qu'il n a aucun freres droites.*/
        tmp = (*A)->fils;
        (*A)->fils = NULL;
        free(tmp);
        return;
    }
    if (strlen(mot) == 0 && (*A)->lettre == '\0'){
        if ((*A)->d != NULL){
            // Supprimer le cas ou on arrive a un \0 et qu'il a des freres droites.
            tmp = (*A)->d;
            (*A)->lettre = tmp->lettre;
            (*A)->g = tmp->g;
            (*A)->d = tmp->d;
            (*A)->fils = tmp->fils; 
            tmp->fils = NULL;
            tmp->d = NULL;
            tmp->g= NULL;
            free(tmp);
            return;
        }
    }

    if ((*A)->fils == NULL && strlen(mot) == 0){
        /* Si il n y a plus de fils mais que on arrive a la fin du mot */
        if ((*A)->d == NULL && (*A)->g == NULL){
            /*Si il a n a pas de droite ni de gauche */
            free((*A));
            return;
        }
        else if ((*A)->d != NULL && (*A)->g != NULL){
            /* si il a un droite et gauche */
            tmp = (*A)->g;
            (*A)->lettre = tmp->lettre;
            tmp2 = tmp;
            while (tmp2->d){
                tmp2 = tmp2->d;
            }
            tmp2->d = (*A)->d;
            (*A)->g = tmp->g;
            (*A)->fils = tmp->fils;
            (*A)->d = tmp->d;
            tmp->d = NULL;
            tmp->g = NULL;
            tmp->fils = NULL;
            free(tmp);
            return;
        }
        else if ((*A)->d != NULL || (*A)->g != NULL){
            /* Si il n a que un frere doite ou un frere gauche*/
            tmp = (*A)->d != NULL? (*A)->d : (*A)->g;
            (*A)->lettre = tmp->lettre;
            (*A)->g = tmp->g;
            (*A)->d = tmp->d;
            (*A)->fils = tmp->fils; 
            tmp->fils = NULL;
            tmp->d = NULL;
            tmp->g= NULL;
            free(tmp);
            return;
        }
    }
    /*appel reccursive */
    if ((*A)->lettre == mot[0]){
        supprimer_dans_ATR(&((*A)->fils), mot + 1);
    }
    if ((*A)->lettre > mot[0]){
        supprimer_dans_ATR(&((*A)->g), mot);
    }
    if ((*A)->lettre < mot[0]){
        supprimer_dans_ATR(&((*A)->d), mot);
    }
}


/**
 * @brief affiche les mots de l'arbre 
 * 
 * @param A (ATR)
 * @param buffer (char)
 * @param indice (integer)
 */
void affiche_aux(ATR A, char buffer[], int indice){
    if (A != NULL){
        if (A->g)
            affiche_aux(A->g, buffer, indice);
        
        buffer[indice] = A->lettre;
        if (A->lettre == '\0')
            printf("%s \n",buffer);
        
        if (A->fils)
            affiche_aux(A->fils, buffer, indice + 1);
        if (A->d)
            affiche_aux(A->d, buffer, indice);
    }
}


/**
 * @brief Fonction qui va utiliser la fonction precedente a des fin d'affichage
 * 
 * @param A (ATR)
 */
void afficher_ATR(ATR A){
    char buffer[MAX_LETTRE];
    int courant = 0;
    if (!A){
        printf("Arbre vide !\n");
        return;
    }
    affiche_aux(A, buffer, courant);
}


/**
 * @brief Fonction qui va entrer les mot du dictionnaire qu on aurat mis en chemin dans l arbre
 * 
 * @param chemin (char)
 * @return ATR (ATR)
 */
ATR construire_ATR(char *chemin){
    ATR A = creer_ATR_vide();
    char tab[MAX_LETTRE];
    FILE *dictionnaire = fopen(chemin, "r");
    if (!dictionnaire) { 
        printf("Erreur de lecture\n");
        return NULL;
    }
    while (fscanf(dictionnaire, "%s", tab) != EOF){
        inserer_dans_ATR(&A, tab);
    }
    fclose(dictionnaire);
    return A;
}

/**
 * @brief Fonction qui va chercher si le mot est present dans l'abre 
 * 
 * @param A (ATR)
 * @param mot (char)
 * @return int (integer)
 */
int recherche_ATR(ATR A, char * mot){
    if (!A)
        return 0;
    if (strlen(mot) == 0 && A->lettre == '\0')
        return 1;
    if (strlen(mot) == 0 && A->lettre != '\0')
        return 0;

    if (A->lettre == mot[0])
        return recherche_ATR(A->fils, mot + 1);

    else if (A->lettre > mot[0]) 
        return recherche_ATR(A->g, mot);

    else 
        return recherche_ATR(A->d, mot);
}
