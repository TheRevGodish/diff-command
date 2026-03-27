#ifndef DISPLAY_H
#define DISPLAY_H

#include "file.h"
/**
 * @brief Affiche le diff entre deux fichiers ligne par ligne.
 * Les lignes supprimées sont préfixées par "- ", les lignes ajoutées
 * par "+ ", et les lignes communes sont affichées telles quelles.
 *
 * @param dfile1  Lignes du premier fichier
 * @param dfile2  Lignes du deuxième fichier
 * @param script  Script d'édition ('m', 'i', 'd')
 */
void final_display_diff(const struct dfile_lines *dfile1,
                        const struct dfile_lines *dfile2,
                        const char *script);
/**
 * @brief Affiche le diff entre deux chaînes caractère par caractère.
 * Les caractères supprimés sont préfixées par "- ", les caractères ajoutés
 * par "+ ", et les caractères communs sont affichés tels quels.
 *
 * @param u  Première chaîne de caractères
 * @param v  Deuxième chaîne de caractères
 * @param script  Script d'édition ('m', 'i', 'd')
 */
void display_diff_chars(const char *u, const char *v, const char *script);

#endif