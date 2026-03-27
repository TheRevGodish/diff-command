#ifndef MFILE_H
#define MFILE_H

/**
   * @brief Fichier chargé en mémoire via mmap.
   */
  struct dfile {
      const char *data; /**< Contenu du fichier */
      int flen;         /**< Taille du fichier en octets */
  };
/**
   * @brief Représente un fichier découpé en lignes.
   */
struct dfile_lines{
    struct line **lines;   /**< Tableau de pointeur vers chaque ligne */
    int line_count;        /**< Nombre de ligne */
};
/**
   * @brief Représente une ligne d'un fichier.
   */
struct line{
    unsigned long hash;    /**< Hash djb2 du contenu de la ligne */
    int id_line;           /**< Numéro de la ligne (commence à 1) */
	const char *content;   /**< Le contenu de la ligne (incluant '\n' si présent)*/
    int len;               /**< Longueur du contenu en octets */
};
/**
 * @brief Charge un fichier en mémoire via mmap.
 *
 * @param path  Chemin vers le fichier à ouvrir
 * @return Pointeur vers le fichier chargé, ou NULL en cas d'erreur. À libérer avec release_file()
 */
struct dfile *read_file(const char *path);
/**
 * @brief Libère les ressources associées à un struct dfile.
 * Démmappe la mémoire (munmap) et libère la structure.
 * Après appel, le pointeur ne doit plus être utilisé.
 *
 * @param dfile  Le fichier à libérer
 */
void release_file(struct dfile *dfile);
/**
 * @brief Compte le nombre de lignes d'un struct dfile.
 *
 * @param dfile  Le fichier où l'on va compter les lignes
 * @return Le nombre de lignes.
 */
int counting_lines(const struct dfile *dfile);
/**
 * @brief Découpe un fichier en lignes.
 * Alloue et remplit un struct dfile_lines à partir d'un struct dfile.
 * Chaque ligne est allouée dynamiquement avec son contenu, sa longueur
 * et son hash calculé.
 *
 * @param dfile  Le fichier source
 * @return Struct dfile_lines alloué dynamiquement. Les lignes et la structure elle-même sont à libérer manuellement.
 */
struct dfile_lines *separate_lines(struct dfile *dfile);

#endif
