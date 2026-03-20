#include "utils.h"

#include <string.h>

unsigned min(unsigned a, unsigned b) {
    return (a < b) ? a : b;
}

// prend un chaîne de caractère en entrée et la retourne à l'envers
void revert(char *str) {
    const int n = strlen(str);
    if (str != NULL) {
        for (int i = 0; i < n / 2; i++) {
            const char tmp = str[i];
            str[i] = str[n - 1 - i];
            str[n - 1 - i] = tmp;
        }
    }
}