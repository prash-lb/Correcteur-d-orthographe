#include "Levenshtein.h"
#include <string.h>

#define min(a,b) (a<=b?a:b)

int DistanceDeLevenshtein(char * chaine1,char * chaine2 ){
    int len1 =strlen(chaine1) ,len2 = strlen(chaine2);
    int D[len1+1][len2+1];
    int i,j,cout;
    for(i = 0;i <= len1 ; i++){
        D[i][0] = i;
    }
    for(j = 0;j <= len2 ; j++){
        D[0][j] = j;
    }
    for(i=1;i<=len1;i++){
        for(j=1;j<=len2;j++){
            if(chaine1[i-1] == chaine2[j-1]){
                cout = 0;
            }
            else{
                cout = 1;
            }
            D[i][j] = min(min(D[i-1][j]+1,D[i][j-1]+1),D[i-1][j-1]+cout);
        }
    }
    return D[len1][len2];
}


