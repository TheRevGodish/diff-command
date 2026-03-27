#ifndef HASH_H
#define HASH_H

#include "file.h"

/**
 * @brief Calcule le hash djb2 d'une chaîne de caractères.
 *
 * @param line  Chaîne à hacher
 * @return Valeur de hash
 */
unsigned long hash(const char *line);

/**
 * @brief Résout les collisions de hash entre deux fichiers.
 *
 * Si deux lignes de contenu différent ont le même hash, le hash de la ligne
 * dans f2 est remplacé par une valeur unique.
 *
 * @param f1  Lignes du premier fichier
 * @param f2  Lignes du deuxième fichier (modifié en cas de collision)
 */
void fix_collisions(const struct dfile_lines *f1, const struct dfile_lines *f2);

#endif
