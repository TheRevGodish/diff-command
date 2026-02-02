#ifndef MFILE_H
#define MFILE_H

/*! \file readfile.h
 * \brief loading a file as a char *
 *
 * The module load efficiently a file in memory using mmap. If loadFile
 * success, user can access to the whole file content throught a char * (the
 * data field)

 * \include pfile.c
 */

/*! \brief Represent a file loaded, all field must only be accessed 
 * in read (not write).
 */
struct dfile{
  /*! the content of the file */
  const char *data;
  /*! the size of the file data */
  int flen;
};

struct dfile_lines{
    // TODO: à modifier en nouvelle 'struct line'
    char **lines;
    int line_count;
    // est-ce qu'on propage le nom du fichier avec : char *file;
};

// TODO: continuer en prenant en compte cette nouvelle struct
struct line{
    char *data;
    int hash;
    int id_line;
    // est-ce qu'on propage le nom du fichier avec : char *file;
};

/*! \brief open the file and if everything work, it returns the structure
 * allowing to read file content.
*/
struct dfile *read_File(const char *path);

/*! Release the ressources associate to an struct MFile, after resources
 * must not be accessed
 */
void releaseFile(struct dfile *dfile);


#endif
