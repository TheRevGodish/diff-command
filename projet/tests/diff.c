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

int counting_lines(struct dfile *dfile){
    int line_count = 1;
    for (int i = 0; i < dfile->flen; i++) {
        if ((dfile->data)[i] == '\n') {
            line_count++;
        }
    }
    return line_count;
}

struct dfile_lines *separate_lines(struct dfile *dfile){
    struct dfile_lines *dfile_lines = malloc(sizeof(*dfile_lines));
    dfile_lines->line_count=counting_lines(dfile);

    dfile_lines->lines = malloc(sizeof(char *) * dfile_lines->line_count);
    int start = 0;
    int line = 0;

    for (int i = 0; i < dfile->flen; i++) {
        if (dfile->data[i] == '\n') {
            int len = i - start + 1; // garde le '\n'

            dfile_lines->lines[line] = malloc(len + 1); // pour avoir la place de rajouter le '\0'
            memcpy(dfile_lines->lines[line], dfile->data + start, len);
            dfile_lines->lines[line][len] = '\0';

            line++;
            start = i + 1;
        }
    }

    if (start < dfile->flen) {
        int len = dfile->flen - start;
        dfile_lines->lines[line] = malloc(len + 1);
        memcpy(dfile_lines->lines[line], dfile->data + start, len);
        dfile_lines->lines[line][len] = '\0';
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

    printf("nb line of %s: %d\n", argv[1], counting_lines(dfile1));
    printf("nb line of %s: %d\n", argv[2], counting_lines(dfile2));

    struct dfile_lines *dfile1_lines = separate_lines(dfile1);
    struct dfile_lines *dfile2_lines = separate_lines(dfile2);

    for (int i = 0; i < dfile1_lines->line_count; i++) {
        printf("%d: %s", i, dfile1_lines->lines[i]);
    }
    printf("\n");

    for (int i = 0; i < dfile2_lines->line_count; i++) {
        printf("%d: %s", i, dfile2_lines->lines[i]);
    }

    release_file(dfile1);
    release_file(dfile2);
}