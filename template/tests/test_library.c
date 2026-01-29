#include <library_impl.h>
#include <rtest.h>


void test_loading_library(){
  struct Library* lib = loadLibrary("lib.txt");
  CMP_NE(lib,NULL);
  CMP_EQ(lib->nbooks,9);
  freeLibrary(lib);
}

int main(){
  RUN(test_loading_library());
}
