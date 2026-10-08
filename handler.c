#include "handler.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *verify_file(char *file, int ext) {
  const char *extension;
  int size;

  switch (ext) {
  case QUESTS:
    extension = ".quests";
    size = strlen(extension);
    break;
  case MAP:
    extension = ".map";
    size = strlen(extension);
    break;
  case POSITION:
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

char *change_ext_to_results(char *file) {

  size_t base_file = strlen(file) - strlen(".quests");
  char *new_ext = ".results";

  char *name = malloc(base_file + strlen(new_ext) + 1);
  if (name == NULL)
    return NULL;

  memcpy(name, file, base_file);
  strcpy(name + base_file, new_ext);
  return name;
}
