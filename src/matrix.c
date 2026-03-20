#include "matrix.h"
#include "utils.h"

#include <string.h>
#include <stdlib.h>

// renvoie la distance entre les deux hash 'u' et 'v' grâce à leur matrice 'dist_mat' correspondante
unsigned naive_dist(const unsigned long* u, const unsigned long* v, unsigned **dist_mat) {
    unsigned len_u = 0;
    unsigned len_v = 0;
    while (u[len_u] != 0) len_u++;
    while (v[len_v] != 0) len_v++;

    // on initialise notre matrice
    if (dist_mat != NULL) {
        for (unsigned i = 0; i <= len_u; i++) {
            dist_mat[i][0] = i;
        }
        for (unsigned j = 0; j <= len_v; j++) {
            dist_mat[0][j] = j;
        }
    }

    // on remplit la matrice en comparant lettre à lettre
    for (unsigned i = 1; i <= len_u; i++) {
        for (unsigned j = 1; j <= len_v; j++) {
            if (u[i-1] == v[j-1]) {
                dist_mat[i][j] = dist_mat[i-1][j-1];
            } else {
                dist_mat[i][j] = 1 + min(dist_mat[i][j-1], dist_mat[i-1][j]);
            }
        }
    }
    return dist_mat[len_u][len_v];
}

// retourne le script de l'algo LCS en remontant 'dist_mat'
char* script_lcs(unsigned **dist_mat, const unsigned lu, const unsigned lv){
    char *script = malloc((lu + lv + 1) * sizeof(char));
    int i = lu;
    int j = lv;
    int pos = 0;

    while (i > 0 || j > 0) {
        // il ne reste que des insertions
        if (i == 0) {
            script[pos++] = 'i';
            j--;
        }
        // il ne reste que des suppressions
        else if (j == 0) {
            script[pos++] = 'd';
            i--;
        } else {
            // on regarde les valeurs en haut, à gauche, et en diagonale en haut à gauche
            const int top  = dist_mat[i-1][j];
            const int left = dist_mat[i][j-1];
            const int cur  = dist_mat[i][j];

            // on compare les valeurs pour connaître la bonne opération
            // suppression
            if (top < cur) {
                script[pos++] = 'd';
                i--;
            }
            // insertion
            else if (left < cur) {
                script[pos++] = 'i';
                j--;
            }
            // match
            else {
                script[pos++] = 'm';
                i--;
                j--;
            }
        }
    }
    script[pos] = '\0';
    return script;
}