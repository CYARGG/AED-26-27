#include "cluster.h"
#include "handler.h"
#include "position.h"
#include "union.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define number_files 3
/*TODO: verificar a memória, se algum malloc falhar os outros ficam por
 * libertar*/
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

  quick_union *map = initialize_quick_union(ncity);

  if (map == NULL)
    return EXIT_FAILURE;

  int city, connection;
  for (int j = 0; j < nconnections; j++) {
    if (fscanf(files[MAP], "%d %d", &city, &connection) != 2) {
      return EXIT_FAILURE;
    }
    unite(map, city, connection);
  }
  fclose(files[MAP]);

  Cluster *list_of_clusters = distribute_by_cluster(map, ncity);

  if (list_of_clusters == NULL)
    return EXIT_FAILURE;
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
  int task, arg;
  char *results = change_ext_to_results(argv[1]);
  if (results == NULL)
    return EXIT_FAILURE;

  FILE *results_file = fopen(results, "w");
  free(results);

  if (results_file == NULL)
    return EXIT_FAILURE;

  while (fscanf(files[QUESTS], " Task%d", &task) == 1) {
    if (task == 3 || task == 4) {
      if (fscanf(files[QUESTS], "%d", &arg) != 1)
        return EXIT_FAILURE;
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

      break;

    case 4:

      break;

    case 5:

      break;

    case 6:

      break;
    }
  }
  fclose(results_file);
  fclose(files[QUESTS]);
  return EXIT_SUCCESS;
}
