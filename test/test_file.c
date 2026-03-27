#include <stdlib.h>
#include "macro_test.h"
#include "../include/file.h"

void test_read_file_fail(){
  const struct dfile *dfile=read_file("");
  CMP_EQ(dfile,NULL);
} 

void test_read_file_ok(){
  struct dfile *dfile=read_file("resources/test_read_file_data.txt");
  CMP_STR_EQ(dfile->data, "data"); // changer par un CMP_NOTNULL ou qqchose dans le style
  CMP_EQ(dfile->flen, 4); // pareil
  release_file(dfile);
}

void test_dfile_ok(){
  struct dfile *dfile=read_file("resources/test_dfile.txt");
  CMP_STR_EQ(dfile->data, "je suis un\ntest_file\n"); // rajouter un '\0'
  CMP_EQ(dfile->flen, 23);
  release_file(dfile);
}

int main(){
  RUN(test_read_file_fail());
  RUN(test_read_file_ok());
  RUN(test_dfile_ok());
}
