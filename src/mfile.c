#include "mfile.h"

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


struct MFile *loadFile(const char *path){
  int fd;
  int flen;
  void *data;
  struct MFile *mfile=NULL;
  fd=open(path,O_RDONLY);
  if (fd==-1){
    perror(path);
    return NULL;
  }
  flen=lseek(fd,0,SEEK_END);
  if (flen<=0){
    close(fd);
    return NULL;
  }
  lseek(fd,0,SEEK_SET);
  data=mmap(NULL,flen,PROT_READ,MAP_SHARED,fd,0);
  if (data==MAP_FAILED){
    perror("mmap");
    return NULL;
  }
  close(fd);
  mfile=(struct MFile *)malloc(sizeof(*mfile));
  mfile->data=data;
  mfile->flen=flen;
  return mfile;
}

void releaseFile(struct MFile *mfile){
  munmap((void *)mfile->data,mfile->flen);
  free(mfile);
}
