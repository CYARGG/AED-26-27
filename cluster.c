#include "cluster.h"
#include "union.h"
#include <stdlib.h>

/*--------------------- distribute_by_cluster ---------------------
 * recebe o union-find, já com todas as uniões feitas, e o número de
 * cidades, e agrupa as cidades por cluster
 * devolve um array de Clusters com índices de 1 a get_cluster_count
 * a posição 0 não é usada. cada cluster fica com as cidades por
 * ordem crescente, e os clusters ficam ordenados entre si pela
 * cidade mais pequena de cada um
 * quem chama liberta o array e o members de cada cluster
 * devolve NULL se alguma reserva de memória falhar
 *----------------------------------------------------------------- */

Cluster *distribute_by_cluster(quick_union *map, int ncity) {
  int *cluster_of_root = calloc(ncity + 1, sizeof(int));

  if (cluster_of_root == NULL) {
    return NULL;
  }

  Cluster *clusters = calloc((get_cluster_count(map) + 1), sizeof(Cluster));

  if (clusters == NULL) {
    free(cluster_of_root);
    return NULL;
  }

  int cluster_count = 0;

  for (int i = 1; i <= ncity; i++) {

    int root = find(map, i);

    if (cluster_of_root[root] == 0) {

      cluster_count++;
      cluster_of_root[root] = cluster_count;
      clusters[cluster_count].members =
          malloc(get_cluster_size(map, root) * sizeof(int));
      if (clusters[cluster_count].members == NULL) {
        free(cluster_of_root);
        free_clusters(clusters, cluster_count);
        return NULL;
      }
      clusters[cluster_count].members[0] = i;
      clusters[cluster_count].size = 1;
    } else {

      int cluster_id = cluster_of_root[root];
      clusters[cluster_id].members[clusters[cluster_id].size] = i;
      clusters[cluster_id].size++;
    }
  }
  free(cluster_of_root);
  return clusters;
}

/*------------------------- free_clusters -------------------------
 * liberta o members de cada cluster (índices 1 a cluster_count) e
 * depois o próprio array. aceita NULL
 *----------------------------------------------------------------- */

void free_clusters(Cluster *clusters, int cluster_count) {
  if (clusters == NULL)
    return;
  for (int i = 1; i <= cluster_count; i++) {
    free(clusters[i].members);
  }
  free(clusters);
}

/*----------------- closest_city_outside_cluster ------------------
 * Task3: devolve a cidade mais próxima (distância euclidiana) da
 * cidade city que não pertence ao mesmo cluster. em caso de empate
 * fica a de menor índice
 * devolve -2 se a cidade não existir ou se só houver um cluster
 *----------------------------------------------------------------- */

int closest_city_outside_cluster(quick_union *map, position *coordinates,
                                 int ncity, int city) {
  if (city < 1 || city > ncity || get_cluster_count(map) == 1)
    return -2;

  int root = find(map, city);
  int closest = -2;
  long best_distance = 0;

  for (int j = 1; j <= ncity; j++) {
    if (find(map, j) == root) // se estiver no mesmo cluster, dar skip
      continue;
    long distance = squared_distance(coordinates[city], coordinates[j]);
    if (closest == -2 || distance < best_distance) {
      closest = j;
      best_distance = distance;
    }
  }
  return closest;
}

/*------------------- closest_city_to_cluster ---------------------
 * Task4: devolve a cidade de fora do cluster de city que está mais
 * perto de alguma cidade desse cluster (o cluster como um todo, não
 * só a city). em caso de empate fica a de menor índice
 * devolve -2 se a cidade não existir ou se só houver um cluster
 *----------------------------------------------------------------- */

int closest_city_to_cluster(quick_union *map, position *coordinates,
                            int ncity, int city) {
  if (city < 1 || city > ncity || get_cluster_count(map) == 1)
    return -2;

  int root = find(map, city);
  int closest = -2;
  long best_distance = 0;

  for (int member = 1; member <= ncity; member++) {
    if (find(map, member) != root)
      continue;
    for (int j = 1; j <= ncity; j++) {
      if (find(map, j) == root)
        continue;
      long distance = squared_distance(coordinates[member], coordinates[j]);
      if (closest == -2 || distance < best_distance ||
          (distance == best_distance && j < closest)) {
        closest = j;
        best_distance = distance;
      }
    }
  }
  return closest;
}
