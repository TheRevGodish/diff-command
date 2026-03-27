#ifndef MYERS_H
#define MYERS_H

// représente l'action effectuée dans Myers à un instant donné
struct step {
    int k;
    int d;
    char op;   // 'i' ou 'd' ou 'm'
    int prev_k;
    int prev_d;
    int i_before_matches;
};

unsigned dist_myers(const unsigned long *u, unsigned len_u,
                    const unsigned long *v, unsigned len_v,
                    struct step *steps, int *step_count, int *final_d);

char *script_myers(const struct step *steps, int step_count, int final_k, int final_d, int len_u, int len_v);

#endif