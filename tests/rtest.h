#ifndef RTEST_H
#define RTEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RUN(t) fputs("\033[32mrunning\033[0m : "#t"\n",stderr); t

#define CMP_EQ(a,b) if (a!=b){  \
    fputs("\033[31merror\033[0m : "#a" != "#b" in ",stderr);	\
    fputs(__func__,stderr);			\
    fputs("\n",stderr);				\
    exit(EXIT_FAILURE); }			\
  else fputs("\033[32mOK\033[0m : "#a" != "#b"\n",stderr);

#define CMP_STR_EQ(a,b) if (strcmp(a,b)!=0){  \
    fputs("\033[31merror : "#a" == "#b" in ",stderr); \
    fputs(__func__,stderr);		      \
    fputs("\n",stderr);			      \
    exit(EXIT_FAILURE); }		      \
  else fputs("\033[32mOK\033[0m : "#a" == "#b"\n",stderr);

#define CMP_NE(a,b) if (a==b){  \
    fputs("\033[31merror\033[0m : "#a" == "#b" in ",stderr); \
    fputs(__func__,stderr);		      \
    fputs("\n",stderr);			      \
    exit(EXIT_FAILURE); }		      \
  else fputs("\033[32mOK\033[0m : "#a" == "#b"\n",stderr);

#endif
