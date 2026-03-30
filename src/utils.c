#include "utils.h"

#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

unsigned min(unsigned a, unsigned b) {
    return (a < b) ? a : b;
}

// prend un chaîne de caractère en entrée et la retourne à l'envers
void revert(char *str) {
    if (str == NULL) return;
    const int n = strlen(str);
    {
        for (int i = 0; i < n / 2; i++) {
            const char tmp = str[i];
            str[i] = str[n - 1 - i];
            str[n - 1 - i] = tmp;
        }
    }
}

unsigned long *str_to_ascii(const char *s) {
    const int len = strlen(s);
    unsigned long *res = malloc((len + 1) * sizeof(unsigned long));
    for (int i = 0; i < len; i++) {
        res[i] = (unsigned long)(unsigned char)s[i];
    }
    res[len] = 0;
    return res;
}

int is_directory(const char *path) {
    struct stat st;
    if (stat(path, &st) != 0)
        return 0;
    return S_ISDIR(st.st_mode);
}