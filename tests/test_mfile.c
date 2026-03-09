#include <rtest.h>

#include <mfile.h>

void test_loadfile_fail(){
  struct MFile *mfile=loadFile("");
  CMP_EQ(mfile,NULL);
} 

void test_loadfile_ok(){
  struct MFile *mfile=loadFile("data.txt");
  CMP_STR_EQ(mfile->data,"data\n");
  releaseFile(mfile);
} 


int main(){
  RUN(test_loadfile_fail());
  RUN(test_loadfile_ok());
}
