#ifndef LIBRARY_IMPL_H
#define LIBRARY_IMPL_H
#include "library.h"
#include <mfile.h>

struct Library {
  struct MFile *raw;
  struct Book *books;
  int nbooks;
};

#endif
