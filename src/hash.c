#include "hash.h"

#include <string.h>

unsigned long hash (const char * line) {
    unsigned long hash = 5381;
    int c;
    while ((c = *line++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

void fix_collisions(const struct dfile_lines *f1, const struct dfile_lines *f2) {
    int counter = 1;
    if (f1 != NULL && f2 != NULL) {
        for (int i = 0; i < f1->line_count; i++) {
            for (int j = 0; j < f2->line_count; j++) {
                if (f1->lines[i]->hash == f2->lines[j]->hash &&
                    (f1->lines[i]->len != f2->lines[j]->len ||
                    strncmp(f1->lines[i]->content, f2->lines[j]->content, f1->lines[i]->len) != 0)) {
                    // collision : on cherche un counter dispo
                    while (counter == f1->lines[i]->hash) counter++;
                    f2->lines[j]->hash = counter++;
                    }
            }
        }
    }
}