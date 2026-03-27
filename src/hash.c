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

static int hash_taken(unsigned long val,
                      const struct dfile_lines *f1,
                      const struct dfile_lines *f2,
                      int f2_limit) {
    for (int i = 0; i < f1->line_count; i++)
        if (f1->lines[i]->hash == val) return 1;
    for (int j = 0; j < f2_limit; j++)
        if (f2->lines[j]->hash == val) return 1;
    return 0;
}

void fix_collisions(const struct dfile_lines *f1, const struct dfile_lines *f2) {
    if (f1 == NULL || f2 == NULL) return;

    unsigned long counter = 1;
    for (int i = 0; i < f1->line_count; i++) {
        for (int j = 0; j < f2->line_count; j++) {
            if (f1->lines[i]->hash == f2->lines[j]->hash &&
                (f1->lines[i]->len != f2->lines[j]->len ||
                 strncmp(f1->lines[i]->content, f2->lines[j]->content, f1->lines[i]->len) != 0)) {
                // collision : on cherche une valeur libre dans f1 et dans f2 (jusqu'à j exclus)
                while (hash_taken(counter, f1, f2, j)) counter++;
                f2->lines[j]->hash = counter++;
            }
        }
    }
}