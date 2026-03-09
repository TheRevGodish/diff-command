#ifndef LIBRARY_H
#define LIBRARY_H


struct Library;

struct Book {
  char *title;
  int id;
};

struct Library *loadLibrary(const char *file);
void freeLibrary(struct Library *);
struct Book *peekABook(struct Library *);

#endif
