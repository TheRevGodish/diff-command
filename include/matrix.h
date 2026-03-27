#ifndef LCS_H
#define LCS_H

/**
 * @brief Calcule la distance d'édition entre deux séquences de hash via une matrice LCS.
 *
 * @param u          Séquence de hash de la première entrée
 * @param len_u      Longueur de u
 * @param v          Séquence de hash de la deuxième entrée
 * @param len_v      Longueur de v
 * @param dist_mat   Matrice (len_u+1) x (len_v+1) allouée par l'appelant,
 *                   remplie par la fonction. dist_mat[i][j] contient la distance
 *                   entre les i premiers éléments de u et les j premiers de v.
 * @return Distance d'édition entre u et v (= dist_mat[len_u][len_v])
 */
unsigned naive_dist(const unsigned long *u, unsigned len_u,
                    const unsigned long *v, unsigned len_v,
                    unsigned **dist_mat);

/**
 * @brief Reconstruit le script d'édition en remontant la matrice de distances.
 *
 * @param dist_mat   Matrice remplie par naive_dist, en lecture seule
 * @param lu         Longueur de la séquence u
 * @param lv         Longueur de la séquence v
 * @return Chaîne de caractères contenant le script d'édition
 *         ('m' = match, 'i' = insertion, 'd' = suppression).
 *         À libérer avec free().
 */
char *script_lcs(unsigned **dist_mat, unsigned lu, unsigned lv);

#endif
