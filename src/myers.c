#include "myers.h"
#include "utils.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// renvoie la distance entre les deux hash 'u' et 'v' suivant l'algo de Myers
unsigned dist_myers(const unsigned long *u, unsigned len_u,
                    const unsigned long *v, unsigned len_v,
                    struct step *steps, int *step_count, int *final_d) {

    const int len_sum = len_u + len_v;
    const int offset = len_sum;
    int *av_buf = malloc((2 * len_sum + 1) * sizeof(int));
    int *av = av_buf + offset;

    for (int idx = -offset; idx <= offset; idx++) av[idx] = 0;

    int j = 0;
    int i_before = 0;
    for (int k = 0; k < len_sum; k++) {
        for (int d = - k; d < k + 1; d+=2) {
            // insertion obligatoire
            if (d == -k) {
                av[d] = av[d + 1];
                steps[*step_count].k = k;
                steps[*step_count].d = d;
                steps[*step_count].op = 'i';
                steps[*step_count].prev_k = k - 1;
                steps[*step_count].prev_d = d + 1;
                (*step_count)++;
            // suppression obligatoire
            } else if (d == k) {
                av[d] = av[d - 1] + 1;
                steps[*step_count].k = k;
                steps[*step_count].d = d;
                steps[*step_count].op = 'd';
                steps[*step_count].prev_k = k - 1;
                steps[*step_count].prev_d = d - 1;
                (*step_count)++;
            // cas normal, pas sur les bordures
            } else {
                const int ins = av[d + 1];
                const int del = av[d - 1] + 1;
                // on compare la valeur de l'insertion et de la suppression pour savoir ce qu'on met dans 'av'
                if (del > ins){
                    av[d] = del;
                    steps[*step_count].k = k;
                    steps[*step_count].d = d;
                    steps[*step_count].op = 'd';
                    steps[*step_count].prev_k = k - 1;
                    steps[*step_count].prev_d = d - 1;
                    (*step_count)++;
                } else {
                    av[d] = ins;
                    steps[*step_count].k = k;
                    steps[*step_count].d = d;
                    steps[*step_count].op = 'i';
                    steps[*step_count].prev_k = k - 1;
                    steps[*step_count].prev_d = d + 1;
                    (*step_count)++;
                }
            }
            // le tableau d'avancement représente les indices i...
            int i = av[d];
            // ...duquel on peut déduire l'indice j
            j = i - d;
            i_before = i;

            // si ça match, on avance "gratuitement"
            while (j < len_v && i < len_u && u[i] == v[j]) {
                i++;
                j++;
            }
            av[d] = i;
            steps[*step_count - 1].i_before_matches = i_before;

            // dès que 'i' et 'j' atteignent les longueurs des tableaux de hash correspondants,
            // on retourne k, le nombre d'opérations nécessaires, donc la distance
            if (i >= len_u && j >= len_v) {
                *final_d = d;
                //free(av - offset);
                free(av_buf);
                return k;
            }
        }
    }

    // dans le pire des cas, la distance entre les deux tableaux de hash est la somme des longueurs de ces derniers
    //free(av - offset);
    free(av_buf);
    return len_sum;
}

// retourne le script de l'algo Myers en remontant le 'struct steps'
char* script_myers(const struct step *steps, int step_count,
                   const int final_k, const int final_d, const int len_u, const int len_v) {
    char *res = malloc((len_u + len_v + 1) * sizeof(char));
    int pos = 0;

    int cur_k = final_k;
    int cur_d = final_d;
    int ci = len_u;  // on part de la fin

    while (cur_k > 0) {
        // trouver le step correspondant à (cur_k, cur_d)
        int idx = -1;
        for (int i = step_count - 1; i >= 0; i--) {
            if (steps[i].k == cur_k && steps[i].d == cur_d) {
                idx = i;
                break;
            }
        }
        if (idx == -1) {
            fprintf(stderr, "step non trouvé pour k=%d d=%d\n", cur_k, cur_d);
            break;
        }

        // matchs : de i_before_matches jusqu'à ci
        while (ci > steps[idx].i_before_matches) {
            res[pos++] = 'm';
            ci--;
        }

        // l'opération
        res[pos++] = steps[idx].op;
        if (steps[idx].op == 'd') ci--;

        // remonter
        cur_k = steps[idx].prev_k;
        cur_d = steps[idx].prev_d;
    }

    // matchs restants au début
    while (ci > 0) { res[pos++] = 'm'; ci--; }

    res[pos] = '\0';
    revert(res);
    return res;
}