#include "handler.h"
#include "position.h"
#include "union.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define number_files 3

int main(int argc, char *argv[]) {
  if (argc != 4)
    return EXIT_FAILURE;

  FILE *files[number_files]; /* array de pointer para os ficheiros */

  /*-----------verifica se os ficheiros estão corretos ----*/

  for (int i = 0; i < number_files; i++) {
    files[i] = verify_file(argv[i + 1], i);

    if (files[i] == NULL) {
      return EXIT_FAILURE;
    }
  }

  /*------------------ ler .map---------------------------*/

  int ncity, nconnections;
  if (fscanf(files[MAP], "%d %d", &ncity, &nconnections) != 2) {
    return EXIT_FAILURE;
  }

  int *parent = malloc((ncity + 1) * sizeof(int));
  int *size = malloc((ncity + 1) * sizeof(int));

  if (parent == NULL || size == NULL) {
    return EXIT_FAILURE;
  }

  for (int i = 1; i <= ncity; i++) {
    parent[i] = i;
    size[i] = 1;
  }

  int city, connection;
  for (int j = 0; j < nconnections; j++) {
    if (fscanf(files[MAP], "%d %d", &city, &connection) != 2) {
      return EXIT_FAILURE;
    }
    unite(parent, size, city, connection);
  }
  fclose(files[MAP]);

  /*--------- ler .position-------------------------------*/

  position *coordinates = malloc((ncity + 1) * sizeof(position));
  if (coordinates == NULL) {
    return EXIT_FAILURE;
  }

  int limx, limy;
  if (fscanf(files[POSITION], "%d %d", &limx, &limy) != 2)
    return EXIT_FAILURE;

  bool *seen = calloc((ncity + 1), sizeof(bool));

  if (seen == NULL) {
    return EXIT_FAILURE;
  }
  int id, x, y;
  for (int i = 1; i <= ncity; i++) {
    if (fscanf(files[POSITION], "%d %d %d", &id, &x, &y) != 3 ||
        out_of_bound(id, x, y, ncity, limx, limy) || seen[id]) {
      return EXIT_FAILURE;
    }
    coordinates[id].x = x;
    coordinates[id].y = y;
    seen[id] = true;
  }

  free(seen);
  fclose(files[POSITION]);

  /*--------------- ler .quests  e resolver------*/

  return EXIT_SUCCESS;
}
