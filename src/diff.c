#include "./readfile.h"

#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// représente l'action effectuée dans Myers à un instant donné
struct step {
    int k;
    int d;
    char op;   // 'i' ou 'd' ou 'm'
    int prev_k;
    int prev_d;
    int i_before_matches;
};

// lit un fichier situé à 'path' et le transforme en un 'struct dfile'
struct dfile *read_file(const char *path){
    int fd;
    int flen;
    void *data;
    struct dfile *dfile = NULL;

    fd = open(path, O_RDONLY);
    if (fd==-1){
        perror(path);
        return NULL;
    }

    flen = lseek(fd, 0, SEEK_END);
    if (flen<=0){
        close(fd);
        return NULL;
    }

    lseek(fd,0,SEEK_SET);
    data=mmap(NULL,flen,PROT_READ,MAP_SHARED,fd,0);
    if (data==MAP_FAILED){
        perror("mmap");
        return NULL;
    }

    close(fd);

    dfile=(struct dfile *)malloc(sizeof(*dfile));
    dfile->data=data;
    dfile->flen=flen;

    return dfile;
}

static unsigned min(unsigned a, unsigned b) {
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

// renvoie la distance entre les deux hash 'u' et 'v' grâce à leur matrice 'dist_mat' correspondante
unsigned naive_dist(const unsigned long* u, const unsigned long* v, unsigned **dist_mat) {
    unsigned len_u = 0;
    unsigned len_v = 0;
    while (u[len_u] != 0) len_u++;
    while (v[len_v] != 0) len_v++;

    // on initialise notre matrice
    if (dist_mat != NULL) {
        for (unsigned i = 0; i <= len_u; i++) {
            dist_mat[i][0] = i;
        }
        for (unsigned j = 0; j <= len_v; j++) {
            dist_mat[0][j] = j;
        }
    }

    // on remplit la matrice en comparant lettre à lettre
    for (unsigned i = 1; i <= len_u; i++) {
        for (unsigned j = 1; j <= len_v; j++) {
            if (u[i-1] == v[j-1]) {
                dist_mat[i][j] = dist_mat[i-1][j-1];
            } else {
                dist_mat[i][j] = 1 + min(dist_mat[i][j-1], dist_mat[i-1][j]);
            }
        }
    }
    return dist_mat[len_u][len_v];
}

// affiche notre t'ableau d'avancement pour Myers
void print_av(const int *av, const int k) {
    printf("k=%d : ", k);
    for (int d = -k; d <= k; d += 2) {
        printf("av[%2d]=%d  ", d, av[d]);
    }
    printf("\n");
}

// renvoie la distance entre les deux hash 'u' et 'v' suivant l'algo de Myers
unsigned dist_myers(const unsigned long * u, const unsigned long * v, struct step *steps, int *step_count, int *final_d) {
    // 'u' mot de gauche
    // TODO: problème longueur, il ne faut pas que le hash vale 0
    // TODO: avoir une fonction qui calcule la longueur et déporter ça ailleurs, puis passer les tailles en paramètre ?
    unsigned len_u = 0;
    unsigned len_v = 0;
    while (u[len_u] != 0) len_u++;
    while (v[len_v] != 0) len_v++;

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

// affiche la matrice LCS
void print_dist_mat_lines(const struct dfile_lines *f1, const struct dfile_lines *f2, unsigned **dist_mat) {
    const unsigned lu = f1->line_count;
    const unsigned lv = f2->line_count;

    printf("        \"\"");
    for (unsigned j = 0; j < lv; j++)
        printf("  %3d", f2->lines[j]->id_line);
    printf("\n");

    for (unsigned i = 0; i <= lu; i++) {
        if (i == 0) printf("   \"\"");
        else        printf("  %3d", f1->lines[i-1]->id_line);
        for (unsigned j = 0; j <= lv; j++)
            printf("  %3u", dist_mat[i][j]);
        printf("\n");
    }
    printf("\n");
}

// retourne le script de l'algo LCS en remontant 'dist_mat'
char* script_lcs(unsigned **dist_mat, const unsigned lu, const unsigned lv){
    char *script = malloc((lu + lv + 1) * sizeof(char));
    int i = lu;
    int j = lv;
    int pos = 0;

    while (i > 0 || j > 0) {
        // il ne reste que des insertions
        if (i == 0) {
            script[pos++] = 'i';
            j--;
        }
        // il ne reste que des suppressions
        else if (j == 0) {
            script[pos++] = 'd';
            i--;
        } else {
            // on regarde les valeurs en haut, à gauche, et en diagonale en haut à gauche
            const int top  = dist_mat[i-1][j];
            const int left = dist_mat[i][j-1];
            const int cur  = dist_mat[i][j];

            // on compare les valeurs pour connaître la bonne opération
            // suppression
            if (top < cur) {
                script[pos++] = 'd';
                i--;
            }
            // insertion
            else if (left < cur) {
                script[pos++] = 'i';
                j--;
            }
            // match
            else {
                script[pos++] = 'm';
                i--;
                j--;
            }
        }
    }
    script[pos] = '\0';
    return script;
}

// retourne le nombre de lignes du fichier passé en paramètre de la commande diff
int counting_lines(const struct dfile *dfile){
    int line_count = 0;
    for (int i = 0; i < dfile->flen; i++) {
        if ((dfile->data)[i] == '\n') {
            line_count++;
        }
    }
	if (dfile->flen > 0 && dfile->data[dfile->flen - 1] != '\n') {
        line_count++;
    }
    return line_count;
}

// fonction de hash
unsigned long hash (const char * line) {
	unsigned long hash = 5381;
	int c;
	while ((c = *line++)) {
		hash = ((hash << 5) + hash) + c;
	}
	return hash;
}

// affiche la sortie de la commande diff grâce au 'script'
void final_display_diff(const struct dfile_lines *dfile1, const struct dfile_lines *dfile2, const char *script) {
	int count_i = 0;
	int count_d = 0;
	int k = 0;

	while (script[k] != '\0') {
		if (script[k] == 'm') {
			printf("%s",dfile1->lines[count_d]->content);
			count_i++;
			count_d++;
		}
		else if (script[k] == 'i') {
			printf("+ %s",dfile2->lines[count_i]->content);
			count_i++;
		}
		else if(script[k] == 'd') {
			printf("- %s",dfile1->lines[count_d]->content);
			count_d++;
		}
		k++;
	}
	printf("\n");
}

// sépare un 'struct dfile' en un 'struct dfile_lines'
struct dfile_lines *separate_lines(struct dfile *dfile){
    struct dfile_lines *dfile_lines = malloc(sizeof(*dfile_lines));
    dfile_lines->line_count=counting_lines(dfile);
	char *temp;

    dfile_lines->lines = malloc(sizeof(struct line *) * dfile_lines->line_count);
    int start = 0;
    int lines_index = 0;

    for (int i = 0; i < dfile->flen; i++) {
        if (dfile->data[i] == '\n') {
            // garde le '\n'
            const int len = i - start + 1;

            dfile_lines->lines[lines_index] = malloc(sizeof(struct line));
			temp = malloc(len + 1);
            memcpy(temp, dfile->data + start, len);
            temp[len] = '\0';

			dfile_lines->lines[lines_index]->hash = hash(temp);
			dfile_lines->lines[lines_index]->id_line = lines_index + 1;
			dfile_lines->lines[lines_index]->content = temp;
			dfile_lines->lines[lines_index]->len = len;

            lines_index++;
            start = i + 1;
        }
    }

    if (start < dfile->flen) {
        int len = dfile->flen - start;
        dfile_lines->lines[lines_index] = malloc(sizeof(struct line));
        temp = malloc(len + 1);
        memcpy(temp, dfile->data + start, len);
        temp[len] = '\0';

		dfile_lines->lines[lines_index]->hash = hash(temp);
		dfile_lines->lines[lines_index]->id_line = lines_index + 1;
		dfile_lines->lines[lines_index]->content = temp;
		dfile_lines->lines[lines_index]->len = len;
    }
    return dfile_lines;
}

// vérifie et corrige les collisions de hash
void fix_collisions(const struct dfile_lines *f1, const struct dfile_lines *f2) {
    int counter = 1;
    if (f1 != NULL && f2 != NULL) {
        for (int i = 0; i < f1->line_count; i++) {
            for (int j = 0; j < f2->line_count; j++) {
                if (f1->lines[i]->hash == f2->lines[j]->hash &&
                    (f1->lines[i]->len != f2->lines[j]->len ||
                    strncmp(f1->lines[i]->content, f2->lines[j]->content, f1->lines[i]->len) != 0)) {
                    // collision : on cherche un counter dispo
                    while (counter == f1->lines[i]->hash) counter++;
                    f2->lines[j]->hash = counter++;
                    }
            }
        }
    }
}

void release_file(struct dfile *dfile){
    munmap((void *)dfile->data,dfile->flen);
    free(dfile);
}

int main(int argc, char **argv) {
    if (argc!=3){
        fprintf(stderr,"wrong number of arguments\n");
        return EXIT_FAILURE;
    }

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

    /* affichage hash */
    /*printf("hash data1 \n");
    for (int i = 0; i < dfile1_lines->line_count; i++) {
        printf("%d: %ld", dfile1_lines->lines[i]->id_line, dfile1_lines->lines[i]->hash);
        printf("\n");
    }
    printf("\n");

    printf("hash data2 \n");
    for (int j = 0; j < dfile2_lines->line_count; j++) {
        printf("%d: %ld", dfile2_lines->lines[j]->id_line, dfile2_lines->lines[j]->hash);
        printf("\n");
    }
    printf("\n");*/
}