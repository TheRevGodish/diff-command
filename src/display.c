#include "display.h"

#include <stdio.h>

#define ANSI_RED_BG   "\033[48;2;80;0;0m"    // fond rouge foncé
#define ANSI_GREEN_BG "\033[48;2;0;50;0m"    // fond vert foncé
#define ANSI_RESET    "\033[0m"

void final_display_diff(const struct dfile_lines *dfile1,
                        const struct dfile_lines *dfile2,
                        const char *script) {
    int count_i = 0;
    int count_d = 0;
    int k = 0;

    while (script[k] != '\0') {
        if (script[k] == 'm') {
            const char *line = dfile1->lines[count_d]->content;
            const int   len  = dfile1->lines[count_d]->len;
            printf("%s", line);
            if (len > 0 && line[len - 1] != '\n') printf("\n");
            count_i++;
            count_d++;
        } else if (script[k] == 'i') {
            const char *line = dfile2->lines[count_i]->content;
            const int   len  = dfile2->lines[count_i]->len;
            int print_len;
            if (len > 0 && line[len - 1] == '\n') {
                print_len = len - 1;
            } else {
                print_len = len;
            }
            printf(ANSI_GREEN_BG "+ %.*s" ANSI_RESET "\n", print_len, line);
            count_i++;
        } else if (script[k] == 'd') {
            const char *line = dfile1->lines[count_d]->content;
            const int   len  = dfile1->lines[count_d]->len;
            int print_len;
            if (len > 0 && line[len - 1] == '\n') {
                print_len = len - 1;
            } else {
                print_len = len;
            }
            printf(ANSI_RED_BG "- %.*s" ANSI_RESET "\n", print_len, line);
            count_d++;
        }
        k++;
    }
    printf("\n");
}

void display_diff_chars(const char *u, const char *v, const char *script) {
    int i = 0, j = 0, k = 0;
    while (script[k] != '\0') {
        if (script[k] == 'm') {
            printf("  %c\n", u[i++]);
            j++;
        } else if (script[k] == 'i') {
            printf("+ %c\n", v[j++]);
        } else if (script[k] == 'd') {
            printf("- %c\n", u[i++]);
        }
        k++;
    }
}