#include "display.h"

#include <stdio.h>

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
            printf("+ %s", line);
            if (len > 0 && line[len - 1] != '\n') printf("\n");
            count_i++;
        } else if (script[k] == 'd') {
            const char *line = dfile1->lines[count_d]->content;
            const int   len  = dfile1->lines[count_d]->len;
            printf("- %s", line);
            if (len > 0 && line[len - 1] != '\n') printf("\n");
            count_d++;
        }
        k++;
    }
    printf("\n");
}