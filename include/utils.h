#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Inverse une chaîne de caractères en place.
 *
 * @param str  Chaîne à inverser (modifiée directement)
 */
void revert(char *str);

/**
 * @brief Renvoie le minimum entre a et b.
 *
 * @param a  Première valeur
 * @param b  Deuxième valeur
 * @return Minimum entre a et b
 */
unsigned min(unsigned a, unsigned b);

/**
 * @brief Convertit une chaîne en tableau de valeurs ASCII.
 *
 * Chaque caractère est converti en sa valeur ASCII (unsigned long).
 * Le tableau est terminé par 0.
 *
 * @param s  Chaîne à convertir
 * @return Tableau alloué dynamiquement. À libérer avec free().
 */
unsigned long *str_to_ascii(const char *s);

#endif
