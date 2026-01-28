#include <library.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, const char **argv){
  struct Library *library;
  struct Book *book;
  if (argc!=2){
    fprintf(stderr,"missing library file path\n");
    return EXIT_FAILURE;
  }
  library = loadLibrary(argv[1]);
  printf("library loaded\n");
  book=peekABook(library);
  printf("peek a book => %d - %s\n",book->id,book->title);
  freeLibrary(library);
  return EXIT_SUCCESS;
}
