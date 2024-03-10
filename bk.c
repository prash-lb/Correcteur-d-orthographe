#include "bk.h"


/**
 * @brief Creé l'abre dans la memoire
 * 
 * @return ArbreBK 
 */
ArbreBK creer_BK_vide(char * mot){
    ArbreBK nv_arbre = (ArbreBK)malloc(sizeof(NoeudBK));
    if (nv_arbre != NULL)
    {
        nv_arbre->filsG= NULL;
        nv_arbre->frereD = NULL;
        nv_arbre->mot = (char *)malloc(sizeof(char) * strlen(mot) + 1);
        strcpy(nv_arbre->mot, mot);
        nv_arbre->valeur = 0;
    }
    else{
        printf("ERREUR MEMOIRE NON ALLOUE\n");
    }
    return nv_arbre;
}
/**
 * @brief Fonction qui ajoute le mot dans l arbre
 * 
 * @param A pointeur
 * @param mot char
 * @return int integer
 */
int inserer_dans_ArbreBK(ArbreBK * A, char * mot){
    ArbreBK tmp, nv_noeud;
    int distanceMot;

    if (*A == NULL){
        nv_noeud = creer_BK_vide(mot);
        *A = nv_noeud;
        return 1;
    }

    tmp = (*A)->filsG;
    distanceMot = DistanceDeLevenshtein((*A)->mot, mot);

    if (!tmp){
        nv_noeud = creer_BK_vide(mot);
        (*A)->filsG = nv_noeud;
        (*A)->filsG->valeur = distanceMot;
        return 1;
    }
    /*Il a deja un fils donc on regarde si un noeud a deja la meme valeur ou non*/
    if (tmp->valeur > distanceMot){
        /*Le premier fils est plus grand donc on doit mettre le nv noeud en premier*/
        nv_noeud = creer_BK_vide(mot);
        nv_noeud->frereD = tmp;
        nv_noeud->valeur = distanceMot;
        (*A)->filsG = nv_noeud;
        return 1;
    }

    if (tmp->valeur == distanceMot){
        /*Le premier fils est de meme distance donc on doit mettre le nv noeud en premier*/
        inserer_dans_ArbreBK(&(tmp), mot);
        return 1;
    }

    while(tmp->frereD && tmp->frereD->valeur < distanceMot)
        tmp = tmp->frereD;
    
    if (!tmp->frereD){
        /*c est le noeud avec la plus grande distance donc a la fin de la liste*/
        nv_noeud = creer_BK_vide(mot);
        tmp->frereD = nv_noeud;
        tmp->frereD->valeur = distanceMot;
        return 1;
    }

    if (tmp->frereD->valeur == distanceMot){
        /*Il y a un noeud ayant la meme distance on l insere donc dans ce sous arbre*/
        inserer_dans_ArbreBK(&(tmp->frereD), mot);
        return 1;
    }

    if (tmp->frereD->valeur > distanceMot){
        nv_noeud = creer_BK_vide(mot);
        nv_noeud->valeur = distanceMot;
        nv_noeud->frereD = tmp->frereD;
        tmp->frereD = nv_noeud;
        return 1;
    }
    return 0;
}
        
/**
 * @brief Fonction qui va verifier si le mot est dans l arbre
 * 
 * @param A ArbreBK
 * @param mot char
 * @return int integer
 */
int est_dans_ArbreBK(ArbreBK A, char * mot){
    int distance;
    ArbreBK tmp;
    if (!A) 
        return 0;
    if (strcmp(A->mot, mot) == 0) 
        return 1;
    distance = DistanceDeLevenshtein(A->mot, mot);
    tmp = A->filsG;
    while(tmp){
        if (tmp->valeur == distance) 
            return est_dans_ArbreBK(tmp, mot);
        else if (tmp->valeur > distance)
            break;
        tmp = tmp->frereD;
    }
    return 0;
}


/**
 * @brief Fonction  qui va liberer de la memoire l arbre
 * 
 * @param A pointeur 
 */
void liberer_ArbreBK(ArbreBK * A){
    if (!(*A)) return;
    if ((*A)->frereD){
        free((*A)->frereD);
        (*A)->frereD = NULL;
    }
    if ((*A)->filsG){
        free((*A)->filsG);
        (*A)->filsG = NULL;
    }
    free((*A));
    (*A) = NULL;
}


/**
 * @brief Fonction qui va creer l arbre selon les mot du dico
 * 
 * @param dico FILE
 * @return ArbreBK 
 */
ArbreBK creer_ArbreBK(FILE * dico){
    ArbreBK A = NULL;
    char tab[MAX_LETTRE];
    if (!dico) { 
        printf("Erreur de lecture\n");
        return NULL;
    }
    while (fscanf(dico, "%s", tab) != EOF)
        inserer_dans_ArbreBK(&A, tab);
    return A;
}


/**
 * @brief Fonction qui Va cherhcer les mot de correction possible par rapport au mot donner
 * 
 * @param A ArbreBK
 * @param mot char
 * @param corrections pointeur
 * @param seuil int
 * @return Liste 
 */
Liste cherche_corrections_ArbreBK(ArbreBK A, char * mot, Liste *corrections, int *seuil){
    ArbreBK tmp;
    int distance, valeurArbre;
    if (!A) return *corrections;
    distance = DistanceDeLevenshtein(A->mot, mot);
    if (distance == *seuil){
        inserer_en_tete(corrections, A->mot);
    }
    else if (distance < *seuil){
        liberer_Liste(corrections);
        inserer_en_tete(corrections, A->mot);
        *seuil = distance;
    }
    tmp = A->filsG;
    while (tmp){
        valeurArbre = tmp->valeur > distance ? tmp->valeur - distance : distance - tmp->valeur;
        if (valeurArbre <= *seuil){
            *corrections = cherche_corrections_ArbreBK(tmp, mot, corrections, seuil);
        }
        tmp = tmp->frereD;
    }
    return *corrections;
}

/**
 * @brief Fonction qui va initialiser la recherche de correction
 * 
 * @param A ArbreBK
 * @param mot char
 * @return Liste 
 */
Liste rechercher_dans_ArbreBK(ArbreBK A, char * mot){
    Liste corrections = NULL;
    int seuil = 100;
    return cherche_corrections_ArbreBK(A, mot, &corrections, &seuil);
}


/**
 *@brief Fonction qui va afficher le debut de l ecriture d un arbre dans un fichier
 *
 *@param fichier (FILE)
 */
void ecrirDebut(FILE *fichier){
    fprintf(fichier,"digraph ArbreBK {\n");
    fprintf(fichier,"node [ shape = record , height = .1 ]\n");
    fprintf(fichier,"edge [ tailclip = false , arrowtail = dot , dir = both ];\n");
}


/**
 *@brief Fonction qui va afficher le milieu de l ecriture d un arbre dans un fichier
 *
 *@param fichier (FILE)
 *@param A (ArbreBK)
 */
void ecrireArbreBK(FILE *fichier, ArbreBK A){
    if (A != NULL){
        fprintf(fichier,"n%p [label=\"<gauche> | <valeur> %s| <valeur> %d| <droite>\"];\n",(void*)A,A->mot, A->valeur); /*racine noeud + valeur*/
        if (A->filsG){
            fprintf(fichier,"n%p: gauche : c -> n%p: valeur  ;\n",(void*)A,(void*)A->filsG); 
            ecrireArbreBK(fichier,A->filsG);
        }
        if (A->frereD){

            fprintf(fichier,"n%p: droite : c -> n%p: valeur;\n",(void*)A,(void*)A->frereD); /*affiche frereD si existe*/
            ecrireArbreBK(fichier,A->frereD);
        }
    }
}


/**
 *@brief Fonction qui va afficher la fin  de l ecriture d un arbre dans un fichier
 *
 *@param fichier (pointeur)
 */
void ecrireFin(FILE *fichier){
    fprintf(fichier,"}");
}


/**
 *@brief Fonction qui va regrouper toute les fonction qui vont elle ecrire dans un fichier mis en parametre 
 *
 *@param fichier (pointeur)
 */
void dessine(FILE *fichier, ArbreBK A){
    ecrirDebut(fichier);
    ecrireArbreBK(fichier,A);
    ecrireFin(fichier); 
}



/**
 *@brief Fonction qui va transformer un fichier en PDF
 *
 *@param dot ( char pointeur)
 *@param pdf ( char pointeur)
 *@param a (Arbre)
 */
void creePDF (char * dot, char * pdf, ArbreBK a) {
    FILE *out =fopen(dot,"w");
    dessine(out,a);
    fclose(out);
    int longeur = strlen(dot)+strlen(pdf)+15;
    char lst[longeur];
    strcpy(lst,"dot -Tpdf ");
    strcat(lst,dot);
    strcat(lst," -o ");
    strcat(lst,pdf);
    system(lst);
    
}



/**
 * @brief Fonction qui va initialise la cration du pdf qui comptorte l'affichage de l arbre et l affiche 
 * 
 * @param A ArbreBK
 */
void afficher_ArbreBK(ArbreBK A){
    creePDF("visualise.dot","visualise.pdf", A);
    system ("evince visualise.pdf");
}
