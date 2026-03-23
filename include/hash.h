#ifndef HASH_H
#define HASH_H

#include "file.h"

unsigned long hash(const char *line);
void fix_collisions(const struct dfile_lines *f1, const struct dfile_lines *f2);

#endif