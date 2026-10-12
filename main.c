#include "cluster.h"
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

  /* tudo começa a NULL para o cleanup só libertar/fechar o que foi aberto */
  int exit_code = EXIT_FAILURE;
  FILE *files[number_files] = {NULL}; /* array de pointer para os ficheiros */
  FILE *results_file = NULL;
  quick_union *map = NULL;
  Cluster *list_of_clusters = NULL;
  position *coordinates = NULL;
  bool *seen = NULL;

  /*-----------verifica se os ficheiros estão corretos ----*/

  for (int i = 0; i < number_files; i++) {
    files[i] = verify_file(argv[i + 1], i);

    if (files[i] == NULL) {
      goto cleanup;
    }
  }

  /*------------------ ler .map---------------------------*/

  int ncity, nconnections;
  if (fscanf(files[MAP], "%d %d", &ncity, &nconnections) != 2 || ncity < 1 ||
      nconnections < 0) {
    goto cleanup;
  }

  map = initialize_quick_union(ncity);

  if (map == NULL)
    goto cleanup;

  int city, connection;
  for (int j = 0; j < nconnections; j++) {
    if (fscanf(files[MAP], "%d %d", &city, &connection) != 2 || city < 1 ||
        city > ncity || connection < 1 || connection > ncity) {
      goto cleanup;
    }
    unite(map, city, connection);
  }
  fclose(files[MAP]);
  files[MAP] = NULL;

  list_of_clusters = distribute_by_cluster(map, ncity);

  if (list_of_clusters == NULL)
    goto cleanup;
  /*--------- ler .position-------------------------------*/

  coordinates = malloc((ncity + 1) * sizeof(position));
  if (coordinates == NULL) {
    goto cleanup;
  }

  int limx, limy;
  if (fscanf(files[POSITION], "%d %d", &limx, &limy) != 2)
    goto cleanup;

  seen = calloc((ncity + 1), sizeof(bool));

  if (seen == NULL) {
    goto cleanup;
  }
  int id, x, y;
  for (int i = 1; i <= ncity; i++) {
    if (fscanf(files[POSITION], "%d %d %d", &id, &x, &y) != 3 ||
        out_of_bound(id, x, y, ncity, limx, limy) || seen[id]) {
      goto cleanup;
    }
    coordinates[id].x = x;
    coordinates[id].y = y;
    seen[id] = true;
  }

  free(seen);
  seen = NULL;
  fclose(files[POSITION]);
  files[POSITION] = NULL;

  /*--------------- ler .quests  e resolver------*/
  int task, arg;
  char *results = change_ext_to_results(argv[1]);
  if (results == NULL)
    goto cleanup;

  results_file = fopen(results, "w");
  free(results);

  if (results_file == NULL)
    goto cleanup;

  while (fscanf(files[QUESTS], " Task%d", &task) == 1) {
    if (task == 3 || task == 4) {
      if (fscanf(files[QUESTS], "%d", &arg) != 1)
        goto cleanup;
    }
    switch (task) {
    case 1:
      fprintf(results_file, "Task1 %d\n\n", get_cluster_count(map));
      break;

    case 2:
      fprintf(results_file, "Task2 %d\n", get_cluster_count(map));
      for (int i = 1; i <= get_cluster_count(map); i++) {
        fprintf(results_file, "Cluster:");
        for (int j = 0; j < list_of_clusters[i].size; j++) {
          fprintf(results_file, " %d", list_of_clusters[i].members[j]);
        }
        fprintf(results_file, "\n");
      }
      fprintf(results_file, "\n");
      break;

    case 3:
      fprintf(results_file, "Task3 %d %d\n\n", arg,
              closest_city_outside_cluster(map, coordinates, ncity, arg));
      break;

    case 4:
      fprintf(results_file, "Task4 %d %d\n\n", arg,
              closest_city_to_cluster(map, coordinates, ncity, arg));
      break;

    case 5:

      break;

    case 6:

      break;
    }
  }
  exit_code = EXIT_SUCCESS;

  /*--------- liberta a memória e fecha os ficheiros------*/
cleanup:
  if (results_file != NULL)
    fclose(results_file);
  for (int i = 0; i < number_files; i++) {
    if (files[i] != NULL)
      fclose(files[i]);
  }
  if (map != NULL)
    free_clusters(list_of_clusters, get_cluster_count(map));
  free_quick_union(map);
  free(coordinates);
  free(seen);
  return exit_code;
}
