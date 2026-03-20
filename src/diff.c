#include "readfile.h"
#include "hash.h"
#include "utils.h"
#include "myers.h"
#include "matrix.h"
#include "display.h"

#include <stdlib.h>     // malloc, free
#include <stdio.h>      // printf, fprintf
#include <string.h>     // memcpy

unsigned long *str_to_ascii(const char *s) {
    int len = strlen(s);
    unsigned long *res = malloc((len + 1) * sizeof(unsigned long));
    for (int i = 0; i < len; i++)
        res[i] = (unsigned long)(unsigned char)s[i];
    res[len] = 0;  // sentinel pour tes fonctions qui s'arrêtent à 0
    return res;
}

int main(int argc, char **argv) {
    if (argc!=3){
        fprintf(stderr,"wrong number of arguments\n");
        return EXIT_FAILURE;
    }

    /*unsigned long *u = str_to_ascii(argv[1]);
    unsigned long *v = str_to_ascii(argv[2]);

    unsigned lu = strlen(argv[1]);
    unsigned lv = strlen(argv[2]);

    unsigned **dist_mat = malloc((lu + 1) * sizeof(unsigned *));
    for (unsigned i = 0; i <= lu; i++)
        dist_mat[i] = malloc((lv + 1) * sizeof(unsigned));

    naive_dist(u, v, dist_mat);
    char *s = script_lcs(dist_mat, lu, lv);
    printf("LCS: %s\n\n", s);
    revert(s);
    printf("\n");
    display_diff_chars(argv[1], argv[2], s);*/

    struct dfile *dfile1 = read_file(argv[1]);
    struct dfile *dfile2 = read_file(argv[2]);

    const struct dfile_lines *dfile1_lines = separate_lines(dfile1);
    const struct dfile_lines *dfile2_lines = separate_lines(dfile2);
    fix_collisions(dfile1_lines, dfile2_lines);

    const unsigned lu = dfile1_lines->line_count;
    const unsigned lv = dfile2_lines->line_count;

    unsigned **dist_mat = malloc((lu + 1) * sizeof(unsigned *));
    for (unsigned i = 0; i <= lu; i++)
        dist_mat[i] = malloc((lv + 1) * sizeof(unsigned));

    unsigned long *u = malloc((lu + 1) * sizeof(unsigned long));
    for (unsigned i = 0; i < lu; i++)
        u[i] = dfile1_lines->lines[i]->hash;
    u[lu] = 0;

    unsigned long *v = malloc((lv + 1) * sizeof(unsigned long));
    for (unsigned i = 0; i < lv; i++)
        v[i] = dfile2_lines->lines[i]->hash;
    v[lv] = 0;

    naive_dist(u, v, dist_mat);
    char *s = script_lcs(dist_mat, lu, lv);
    printf("LCS: %s\n\n", s);
    revert(s);
    final_display_diff(dfile1_lines, dfile2_lines, s);
    printf("\n");

    struct step *steps = malloc(((lu + lv + 1) * (lu + lv + 1)) * sizeof(struct step));
    int step_count = 0;
    int final_d = 0;

    const unsigned dist = dist_myers(u, v, steps, &step_count, &final_d);
    char *s2 = script_myers(steps, step_count, dist, final_d, lu, lv);
    printf("Myers: %s\n\n", s2);

    final_display_diff(dfile1_lines, dfile2_lines, s2);

    free(steps);
    free(u);
    free(v);
    for (int i = 0; i <= lu; i++) free(dist_mat[i]);
    free(dist_mat);
    free(s);
    free(s2);
    free(dfile1_lines->lines);
    free(dfile2_lines->lines);
    release_file(dfile1);
    release_file(dfile2);
}