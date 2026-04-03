#include "file.h"
#include "hash.h"
#include "utils.h"
#include "myers.h"
#include "matrix.h"
#include "display.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

int main(int argc, char **argv) {
    int opt;
    int flag_n = 0;  // -n : algo matrix
    int flag_s = 0;  // -s : strings directement
    int flag_r = 0;  // -r : récursif sur dossiers

    while ((opt = getopt(argc, argv, "nsr")) != -1) {
        switch (opt) {
            case 'n': flag_n = 1; break;
            case 's': flag_s = 1; break;
            case 'r': flag_r = 1; break;
            default:
                fprintf(stderr, "Usage: %s [-n|-s|-r] arg1 arg2\n", argv[0]);
                return EXIT_FAILURE;
        }
    }

    // après getopt, optind pointe sur le premier argument non-option
    if (argc - optind != 2) {
        fprintf(stderr, "Usage: %s [-n|-s|-r] arg1 arg2\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *arg1 = argv[optind];
    char *arg2 = argv[optind + 1];

    if (flag_s) {
        // --- Mode strings directement ---
        unsigned long *u = str_to_ascii(arg1);
        unsigned long *v = str_to_ascii(arg2);
        unsigned lu = strlen(arg1);
        unsigned lv = strlen(arg2);

        unsigned **dist_mat = malloc((lu + 1) * sizeof(unsigned *));
        for (unsigned i = 0; i <= lu; i++)
            dist_mat[i] = malloc((lv + 1) * sizeof(unsigned));

        struct step *steps = malloc(((lu + lv + 1) * (lu + lv + 1)) * sizeof(struct step));
        int step_count = 0;
        int final_d = 0;

        char *s = NULL;

        if (flag_n) {
            naive_dist(u, lu, v, lv, dist_mat);
            s = script_lcs(dist_mat, lu, lv);
            revert(s);
            printf("LCS: %s\n\n", s);
            display_diff_chars(arg1, arg2, s);
        } else {
            const unsigned dist = dist_myers(u, lu, v, lv, steps, &step_count, &final_d);
            s = script_myers(steps, step_count, dist, final_d, lu, lv);
            printf("Myers: %s\n\n", s);
            display_diff_chars(arg1, arg2, s);
        }

        free(s);
        free(steps);
        free(u);
        free(v);
        for (unsigned i = 0; i <= lu; i++) free(dist_mat[i]);
        free(dist_mat);

    } else if (flag_r) {
        // --- Mode récursif sur dossiers ---
        printf("------------------------------------\n");
        printf("arg1: %d, arg2: %d\n", is_directory(arg1), is_directory(arg2));
        printf("------------------------------------\n");
        if (is_directory(arg1) == 0 || is_directory(arg2) == 0) {
            fprintf(stderr, "Usage: %s -r: arg1 arg2 must be directory\n", argv[0]);
            return EXIT_FAILURE;
        }

        struct dirent *dir1;
        int l1 = 0;
		char **tableau1 = NULL;
        DIR *d1 = opendir(arg1);
        if (d1) {
            while ((dir1 = readdir(d1)) != NULL) {
                if (strcmp(dir1->d_name, ".") == 0 || strcmp(dir1->d_name, "..") == 0) continue;
                l1++;
            }
            closedir(d1);
            d1 = opendir(arg1);
            tableau1 = malloc(l1 * sizeof(char *));
            int idx = 0;
            while ((dir1 = readdir(d1)) != NULL) {
				if (strcmp(dir1->d_name, ".") == 0 || strcmp(dir1->d_name, "..") == 0) continue;
                tableau1[idx++] = strdup(dir1->d_name);
            }
            closedir(d1);
        }
        struct dirent *dir2;
        int l2 = 0;
		char **tableau2 = NULL;
        DIR *d2 = opendir(arg2);
        if (d2) {
			while ((dir2 = readdir(d2)) != NULL) {
				if (strcmp(dir2->d_name, ".") == 0 || strcmp(dir2->d_name, "..") == 0) continue;
        		l2++;
        	}
            closedir(d2);
            d2 = opendir(arg2);
            tableau2 = malloc(l2 * sizeof(char *));
            int idx = 0;
            while ((dir2 = readdir(d2)) != NULL) {
				if (strcmp(dir2->d_name, ".") == 0 || strcmp(dir2->d_name, "..") == 0) continue;
                tableau2[idx++] = strdup(dir2->d_name);
            }
            closedir(d2);
        }
        // dist_myers(tableau1, l1, tableau2, l2, steps, &step_count, &final d)
        // s = script_myers(steps, step_count, dist, final_d, l1, l2);
        // while (s != '\0') {
        //      if (s[idx] = 'd') {
        //
        //      }
        // }

        // pour chaque nom dans tableau1 :
        //      chercher ce nom dans tableau2 avec strcmp
        //      si trouvé : diff_fichiers(arg1/nom, arg2/nom)
        //      sinon : afficher "seulement dans arg1: nom"
        // pour chaque nom dans tableau2 :
        //      chercher ce nom dans tableau1 avec strcmp
        //      si non trouvé : afficher "seulement dans arg2: nom"

    } else {
        // --- Mode fichiers (défaut, avec ou sans -n) ---
        struct dfile *dfile1 = read_file(arg1);
        struct dfile *dfile2 = read_file(arg2);

        const struct dfile_lines *dfile1_lines = separate_lines(dfile1);
        const struct dfile_lines *dfile2_lines = separate_lines(dfile2);
        fix_collisions(dfile1_lines, dfile2_lines);

        const unsigned lu = dfile1_lines->line_count;
        const unsigned lv = dfile2_lines->line_count;

        unsigned **dist_mat = malloc((lu + 1) * sizeof(unsigned *));
        for (unsigned i = 0; i <= lu; i++)
            dist_mat[i] = malloc((lv + 1) * sizeof(unsigned));

        unsigned long *u = malloc(lu * sizeof(unsigned long));
        for (unsigned i = 0; i < lu; i++)
            u[i] = dfile1_lines->lines[i]->hash;

        unsigned long *v = malloc(lv * sizeof(unsigned long));
        for (unsigned i = 0; i < lv; i++)
            v[i] = dfile2_lines->lines[i]->hash;

        // moins gourmand avec (lu + 1) * (lu + 1)
        struct step *steps = malloc(((lu + lv + 1) * (lu + lv + 1)) * sizeof(struct step));
        int step_count = 0;
        int final_d = 0;

        char *s = NULL;

        if (flag_n) {
            naive_dist(u, lu, v, lv, dist_mat);
            s = script_lcs(dist_mat, lu, lv);
            revert(s);
            printf("LCS: %s\n\n", s);
            final_display_diff(dfile1_lines, dfile2_lines, s);
        } else {
            const unsigned dist = dist_myers(u, lu, v, lv, steps, &step_count, &final_d);
            s = script_myers(steps, step_count, dist, final_d, lu, lv);
            printf("Myers: %s\n\n", s);
            final_display_diff(dfile1_lines, dfile2_lines, s);
        }

        free(s);
        free(steps);
        free(u);
        free(v);
        for (unsigned i = 0; i <= lu; i++) free(dist_mat[i]);
        free(dist_mat);
        free(dfile1_lines->lines);
        free(dfile2_lines->lines);
        release_file(dfile1);
        release_file(dfile2);
    }

    return EXIT_SUCCESS;
}