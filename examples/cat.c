#include <mfile.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, const char **argv){
  struct MFile *f=NULL;
  if (argc!=2){
    fprintf(stderr,"expecting one argument\n");
    return EXIT_FAILURE;
  }
  f=loadFile(argv[1]);
  if (f!=NULL){
    fwrite(f->data,sizeof(char),f->flen,stdout);
    releaseFile(f);
  }
  return EXIT_SUCCESS;
}
