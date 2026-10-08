#ifndef HANDLER_H
#define HANDLER_H
#include <stdio.h>

enum { QUESTS, MAP, POSITION };

FILE *verify_file(char *file, int ext);

char *change_ext_to_results(char *file);
#endif
