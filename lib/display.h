#ifndef DISPLAY_H
#define DISPLAY_H

#include "readfile.h"

void final_display_diff(const struct dfile_lines *dfile1,
                        const struct dfile_lines *dfile2,
                        const char *script);

#endif