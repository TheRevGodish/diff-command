#include "readfile.h"
#include "hash.h"

#include <sys/mman.h>
#include <sys/types.h>
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

void release_file(struct dfile *dfile){
    munmap((void *)dfile->data,dfile->flen);
    free(dfile);
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