#include "macro_test.h"
#include "../include/file.h"

void test_read_file_fail(){
  const struct dfile *dfile=read_file("");
  CMP_EQ(dfile,NULL);
} 

void test_read_file_ok(){
  struct dfile *dfile=read_file("resources/test_read_file_data.txt");
  CMP_STR_EQ(dfile->data, "data");
  CMP_EQ(dfile->flen, 4);
  release_file(dfile);
} 

int main(){
  RUN(test_read_file_fail());
  RUN(test_read_file_ok());
}
