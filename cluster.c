#include "cluster.h"
#include "union.h"
#include <stdlib.h>

/*--------------------- distribute_by_cluster ---------------------
 * recebe o union-find, já com todas as uniões feitas, e o número de
 * cidades, e agrupa as cidades por cluster
 * devolve um ClusterSet com o array de Clusters (índices de 1 a
 * count, a posição 0 não é usada) e o cluster_of_root, que diz o
 * índice do cluster de cada raiz do union-find. cada cluster fica com
 * as cidades por ordem crescente, e os clusters ficam ordenados entre
 * si pela cidade mais pequena de cada um
 * o union-find não pode mudar depois disto, senão as raízes deixam de
 * bater certo com o cluster_of_root
 * quem chama liberta tudo com free_clusters
 * devolve NULL se alguma reserva de memória falhar
 *----------------------------------------------------------------- */

ClusterSet *distribute_by_cluster(quick_union *map, int ncity) {
  ClusterSet *set = calloc(1, sizeof(ClusterSet));

  if (set == NULL) {
    return NULL;
  }

  set->cluster_of_root = calloc(ncity + 1, sizeof(int));

  if (set->cluster_of_root == NULL) {
    free_clusters(set);
    return NULL;
  }

  set->clusters = calloc((get_cluster_count(map) + 1), sizeof(Cluster));

  if (set->clusters == NULL) {
    free_clusters(set);
    return NULL;
  }

  int cluster_count = 0;

  for (int i = 1; i <= ncity; i++) {

    int root = find(map, i);

    if (set->cluster_of_root[root] == 0) {

      cluster_count++;
      set->cluster_of_root[root] = cluster_count;
      set->clusters[cluster_count].members =
          malloc(get_cluster_size(map, root) * sizeof(int));
      if (set->clusters[cluster_count].members == NULL) {
        set->count = cluster_count;
        free_clusters(set);
        return NULL;
      }
      set->clusters[cluster_count].members[0] = i;
      set->clusters[cluster_count].size = 1;
    } else {

      int cluster_id = set->cluster_of_root[root];
      set->clusters[cluster_id].members[set->clusters[cluster_id].size] = i;
      set->clusters[cluster_id].size++;
    }
  }
  set->count = cluster_count;
  return set;
}

/*------------------------- free_clusters -------------------------
 * liberta o members de cada cluster (índices 1 a count), o array de
 * clusters, o cluster_of_root e a própria struct. aceita NULL e
 * structs preenchidas só em parte
 *----------------------------------------------------------------- */

void free_clusters(ClusterSet *set) {
  if (set == NULL)
    return;
  if (set->clusters != NULL) {
    for (int i = 1; i <= set->count; i++) {
      free(set->clusters[i].members);
    }
  }
  free(set->clusters);
  free(set->cluster_of_root);
  free(set);
}

/*----------------- closest_city_outside_cluster ------------------
 * Task3: devolve a cidade mais próxima (distância euclidiana) da
 * cidade city que não pertence ao mesmo cluster. em caso de empate
 * fica a de menor índice
 * devolve -2 se a cidade não existir ou se só houver um cluster
 *----------------------------------------------------------------- */

int closest_city_outside_cluster(quick_union *map, const ClusterSet *set,
                                 position *coordinates, int ncity, int city) {
  if (city < 1 || city > ncity || set->count == 1)
    return -2;

  int id = set->cluster_of_root[find(map, city)];
  int closest = -2;
  long best_distance = 0;

  for (int j = 1; j <= set->count; j++) {
    if (j == id) // se estiver no mesmo cluster, dar skip
      continue;
    for (int i = 0; i < set->clusters[j].size; i++) {
      long distance = squared_distance(coordinates[city],
                                       coordinates[set->clusters[j].members[i]]);
      if (closest == -2 || distance < best_distance ||
          (distance == best_distance && set->clusters[j].members[i] < closest)) {
        closest = set->clusters[j].members[i];
        best_distance = distance;
      }
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

int closest_city_to_cluster(quick_union *map, ClusterSet *set,
                            position *coordinates, int ncity, int city) {
  if (city < 1 || city > ncity || set->count == 1)
    return -2;

  int id = set->cluster_of_root[find(map, city)];

  if (set->clusters[id].closest_outside != 0)
    return set->clusters[id].closest_outside;

  int closest = -2;
  long best_distance = 0;

  for (int member = 0; member < set->clusters[id].size; member++) {
    for (int j = 1; j <= set->count; j++) {
      if (j == id)
        continue;
      for (int i = 0; i < set->clusters[j].size; i++) {
        long distance =
            squared_distance(coordinates[set->clusters[id].members[member]],
                             coordinates[set->clusters[j].members[i]]);
        if (closest == -2 || distance < best_distance ||
            (distance == best_distance &&
             set->clusters[j].members[i] < closest)) {
          closest = set->clusters[j].members[i];
          best_distance = distance;
        }
      }
    }
  }
  set->clusters[id].closest_outside = closest;
  return closest;
}
