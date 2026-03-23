#ifndef LCS_H
#define LCS_H

unsigned naive_dist(const unsigned long *u, const unsigned long *v,
                    unsigned **dist_mat);
char *script_lcs(unsigned **dist_mat, unsigned lu, unsigned lv);

#endif