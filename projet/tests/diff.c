#include "readfile.h"

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

void reverte(char *str) {
    int n = strlen(str);
    for (int i = 0; i < n / 2; i++) {
        char tmp = str[i];
        str[i] = str[n - 1 - i];
        str[n - 1 - i] = tmp;
    }
}

unsigned naive_dist(const int* u, const int* v, unsigned **dist_mat) {
    unsigned len_u = 0;
    unsigned len_v = 0;
    while (u[len_u] != 0) len_u++;
    while (v[len_v] != 0) len_v++;
    for (unsigned i = 0; i <= len_u; i++) {
        dist_mat[i][0] = i;
    }
    for (unsigned j = 0; j <= len_v; j++) {
        dist_mat[0][j] = j;
    }
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

char* script(unsigned **dist_mat, unsigned lu, unsigned lv){
	// distance entre les deux mots
	int dist = dist_mat[lu][lv];
	int current = dist;
	// on balaye la valeur au dessus, à gauche, et en diagonale gauche de current
	while (current != 0) {
		// i = lu     j = lv
		int top = dist_mat[i][j-1];
		int left = dist_mat[i-1][j];
		int diag = dist_mat[i-1][j-1];
		// on veut celui qui vaut current - 1 et se déplace dessus en modifiant i et j à la baisse
	}
}

int counting_lines(struct dfile *dfile){
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

unsigned long hash (char * line) {
	unsigned long hash = 5381;
	int c;
	while ((c = *line++)) {
		hash = ((hash << 5) + hash) + c;
	}
	return hash;
}

struct dfile_lines *separate_lines(struct dfile *dfile){
    struct dfile_lines *dfile_lines = malloc(sizeof(*dfile_lines));
    dfile_lines->line_count=counting_lines(dfile);
	char *temp;

    dfile_lines->lines = malloc(sizeof(struct line *) * dfile_lines->line_count);
    int start = 0;
    int lines_index = 0;

    for (int i = 0; i < dfile->flen; i++) {
        if (dfile->data[i] == '\n') {
            int len = i - start + 1; // garde le '\n'

            dfile_lines->lines[lines_index] = malloc(sizeof(struct line));
			temp = malloc(len + 1);
            memcpy(temp, dfile->data + start, len);
            temp[len] = '\0';

			dfile_lines->lines[lines_index]->hash = hash(temp);
			dfile_lines->lines[lines_index]->id_line = lines_index + 1;

            lines_index++;
            start = i + 1;
			free(temp);
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

		free(temp);
    }

    return dfile_lines;
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

    struct dfile_lines *dfile1_lines = separate_lines(dfile1);
    struct dfile_lines *dfile2_lines = separate_lines(dfile2);

    for (int i = 0; i < dfile1_lines->line_count; i++) {
        printf("%d: %ld", dfile1_lines->lines[i]->id_line, dfile1_lines->lines[i]->hash);
		printf("\n");
    }
    printf("\n");

    for (int j = 0; j < dfile2_lines->line_count; j++) {
        printf("%d: %ld", dfile2_lines->lines[j]->id_line, dfile2_lines->lines[j]->hash);
		printf("\n");
    }

    release_file(dfile1);
    release_file(dfile2);
}