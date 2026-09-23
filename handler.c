#include "handler.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

FILE *verify_file(char *file, int ext) {
  const char *extension;
  int size;

  switch (ext) {
  case 0:
    extension = ".quests";
    size = strlen(extension);
    break;
  case 1:
    extension = ".map";
    size = strlen(extension);
    break;
  case 2:
    extension = ".position";
    size = strlen(extension);
    break;
  }
  if (strlen(file) < strlen(extension))
    return NULL;

  char *file_extension = file + strlen(file) - size;

  if (strcmp(file_extension, extension) != 0) {
    return NULL;
  }

  FILE *open_file = fopen(file, "r");
  if (open_file == NULL)
    return NULL;

  return open_file;
}
