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
struct MFile{
  /*! the content of the file */
  const char *data;
  /*! the size of the file data */
  int flen; 
};

/*! \brief open the file and if everything work, it returns the structure
 * allowing to read file content.
*/
struct MFile *loadFile(const char *path);

/*! Release the ressources associate to an struct MFile, after resources
 * must not be accessed
 */
void releaseFile(struct MFile *mfile);


#endif
