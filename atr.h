#ifndef __ATR__
#define __ATR__

    typedef struct noeud{
        char lettre; /* étiquette du nœud */
        struct noeud *g; /* adresse du  gauche */
        struct noeud *d; /* adresse du droit */
        struct noeud *fils; /* adresse du fils  */
    } Noeud, * ATR;
    ATR creer_ATR_vide();
    void liberer_ATR(ATR * A);
    int inserer_dans_ATR(ATR * A, char * mot);
    void supprimer_dans_ATR(ATR * A, char * mot);
    void afficher_ATR(ATR A);
    ATR construire_ATR(char *chemin);
    int recherche_ATR(ATR A, char * mot);

#endif