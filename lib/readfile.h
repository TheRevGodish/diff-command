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
    struct line **lines;
    int line_count;
};

struct line{
    unsigned long hash;
    int id_line;
	// ajouté pour collision
	const char *content;
    int len;
};

/*! \brief open the file and if everything work, it returns the structure
 * allowing to read file content.
*/
struct dfile *read_file(const char *path);

/*! Release the ressources associate to an struct MFile, after resources
 * must not be accessed
 */
void release_file(struct dfile *dfile);

int counting_lines(const struct dfile *dfile);

struct dfile_lines *separate_lines(struct dfile *dfile);

#endif
