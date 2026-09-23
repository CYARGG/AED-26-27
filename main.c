#include "handler.h"
#include "position.h"
#include "union.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc != 4)
    return EXIT_FAILURE;

  FILE **files = (FILE **)(malloc(
      3 * sizeof(FILE *))); /* faz um array de pointers para fd's */

  for (int i = 0; i < 3; i++) {
    files[i] =
        verify_file(argv[i + 1], i); /*0 = .quests , 1 = .map , 2 = .position */

    if (files[i] == NULL) {
      return EXIT_FAILURE;
    }
  }

  int ncity, nconnections;
  if (fscanf(files[1], "%d %d", &ncity, &nconnections) != 2) {
    return EXIT_FAILURE;
  }

  int *parent = (int *)(malloc((ncity + 1) * sizeof(int)));
  int *size = (int *)(malloc((ncity + 1) * sizeof(int)));

  if (parent == NULL || size == NULL) {
    perror("Erro a alocar memória");
    return EXIT_FAILURE;
  }

  for (int i = 1; i <= ncity; i++) {
    parent[i] = i;
    size[i] = 1;
  }

  int city, connection;
  for (int j = 0; j < nconnections; j++) {
    if (fscanf(files[1], "%d %d", &city, &connection) == 2) {
      unite(parent, size, city, connection);
    } else {
      perror("Erro a ler ficheiro");
      return EXIT_FAILURE;
    }
  }

  position *coordinates = (position *)(malloc((ncity + 1) * sizeof(position)));
  if (coordinates == NULL) {
    perror("erro a alocar memória");
    return EXIT_FAILURE;
  }

  int limx, limy;
  if (fscanf(files[2], "%d %d", &limx, &limy) != 2)
    return EXIT_FAILURE;

  int x, y, id; /*TODO: falta a verificação obriagatória de não haver cidades
                   repetidas e todas terem coordenadas*/
  for (int i = 1; i <= ncity; i++) {
    if (fscanf(files[2], "%d %d %d", &id, &x, &y) != 3 || x > limx || x < 1 ||
        y > limy || y < 1) {
      return EXIT_FAILURE;
    } else {
      coordinates[i].x = x;
      coordinates[i].y = y;
    }
  }

  return EXIT_SUCCESS;
}
