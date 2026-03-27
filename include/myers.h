#ifndef MYERS_H
#define MYERS_H

/**
 * @brief Représente une action effectuée dans l'algo Myers à un instant donné.
 */
struct step {
    int k;               /**< Position sur la diagonale */
    int d;               /**< Indice i avant les matchs automatiques */
    char op;             /**< Opération effectuée : 'i' (insertion), 'd' (suppression), 'm' (match) */
    int prev_k;          /**< Valeur de k à l'étape précédente */
    int prev_d;          /**< Valeur de d à l'étape précédente */
    int i_before_matches;/**< Valeur de i avant les matchs automatiques */
};

/**
 * @brief Calcule la distance de Myers entre deux séquences de hash.
 *
 * @param u          Séquence de hash de la première entrée
 * @param len_u      Longueur de u
 * @param v          Séquence de hash de la deuxième entrée
 * @param len_v      Longueur de v
 * @param steps      Buffer pour stocker les étapes (taille minimale : (len_u + len_v)²)
 * @param step_count Nombre d'étapes enregistrées dans steps (paramètre de sortie)
 * @param final_d    Valeur de d à la fin de l'algorithme (paramètre de sortie)
 * @return Nombre d'opérations minimum (insertions + suppressions) pour transformer u en v
 */
unsigned dist_myers(const unsigned long *u, unsigned len_u,
                    const unsigned long *v, unsigned len_v,
                    struct step *steps, int *step_count, int *final_d);

/**
 * @brief Reconstruit le script d'édition à partir des étapes de l'algo Myers.
 *
 * @param steps      Buffer des étapes enregistrées durant dist_myers
 * @param step_count Nombre d'étapes dans le buffer
 * @param final_k    Valeur de k à la fin de l'algorithme
 * @param final_d    Valeur de d à la fin de l'algorithme
 * @param len_u      Longueur de la séquence u
 * @param len_v      Longueur de la séquence v
 * @return Chaîne de caractères contenant le script d'édition
 *         ('m' = match, 'i' = insertion, 'd' = suppression).
 *         À libérer avec free().
 */
char *script_myers(const struct step *steps, int step_count, int final_k, int final_d, int len_u, int len_v);

#endif
