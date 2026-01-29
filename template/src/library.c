#include "library_impl.h"
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


struct Library *loadLibrary(const char *file){
  int i,j;
  int nbooks=0;
  struct Library *lib=NULL;
  struct MFile *raw=loadFile(file);
  if (raw==NULL) return NULL;
  for(i=1;i<raw->flen;++i){
    if (raw->data[i]=='\n')
      nbooks+=1;
  }
  if (nbooks==0){
    releaseFile(raw);
    return NULL;
  }
  lib=(struct Library *)malloc(sizeof(*lib));
  lib->raw=raw;
  lib->nbooks=nbooks;
  lib->books = (struct Book *)malloc(sizeof(struct Book)*nbooks);
  j=0;
  nbooks=0;
  for(i=1;i<raw->flen;++i){
    if (raw->data[i]=='\n'){
      lib->books[nbooks].id=nbooks;
      lib->books[nbooks].title=(char *)malloc((i-j+1)*sizeof(char));
      strncpy(lib->books[nbooks].title,raw->data+j,i-j);
      lib->books[nbooks].title[i-j]='\0';
      j=i+1;
      nbooks+=1;
    }
  }
  return lib;
}
void freeLibrary(struct Library *library){
  int i;
  for(i=0;i<library->nbooks;++i){
    free(library->books[i].title);
  }
  free(library->books);
  releaseFile(library->raw);
  free(library);
}
struct Book *peekABook(struct Library *library){
  if (library->nbooks==0) return NULL;
  return &(library->books[(rand()+getpid())%library->nbooks]);
}
