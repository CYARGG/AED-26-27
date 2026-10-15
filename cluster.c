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
  /* calloc para os ponteiros começarem a NULL e o count a 0: assim o
   * free_clusters aceita a struct preenchida só em parte */
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

  for (int i = 1; i <= ncity; i++) {

    int root = find(map, i);

    if (set->cluster_of_root[root] == 0) {

      /* o count sobe antes do malloc: se falhar, o members fica a NULL
       * (calloc) e o free_clusters pode libertá-lo na mesma */
      set->count++;
      set->cluster_of_root[root] = set->count;
      Cluster *new_cluster = &set->clusters[set->count];
      new_cluster->members = malloc(get_cluster_size(map, root) * sizeof(int));
      if (new_cluster->members == NULL) {
        free_clusters(set);
        return NULL;
      }
      new_cluster->members[0] = i;
      new_cluster->size = 1;
    } else {

      Cluster *cluster = &set->clusters[set->cluster_of_root[root]];
      cluster->members[cluster->size] = i;
      cluster->size++;
    }
  }
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
 * fica a de menor índice (esclarecimento do enunciado: com várias
 * cidades à mesma distância, a resposta certa é a de menor índice)
 * percorre os membros dos outros clusters, por isso as cidades não
 * são visitadas por ordem 1..ncity e o desempate tem de ser explícito
 * devolve -2 se a cidade não existir ou se só houver um cluster
 *----------------------------------------------------------------- */

int closest_city_outside_cluster(quick_union *map, const ClusterSet *set,
                                 position *coordinates, int ncity, int city) {
  if (city < 1 || city > ncity || set->count == 1)
    return -2;

  int idx = set->cluster_of_root[find(map, city)];
  int closest = -2;
  long best_distance = 0;

  for (int k = 1; k <= set->count; k++) {
    if (k == idx) // se for o próprio cluster, dar skip
      continue;
    const Cluster *other = &set->clusters[k];
    for (int j = 0; j < other->size; j++) {
      int candidate = other->members[j];
      long distance =
          squared_distance(coordinates[city], coordinates[candidate]);
      if (closest == -2 || distance < best_distance ||
          (distance == best_distance && candidate < closest)) {
        closest = candidate;
        best_distance = distance;
      }
    }
  }
  return closest;
}

/*------------------- closest_city_to_cluster ---------------------
 * Task4: devolve a cidade de fora do cluster de city que está mais
 * perto de alguma cidade desse cluster (o cluster como um todo, não
 * só a city). em caso de empate fica a de menor índice de todas as
 * que são solução (esclarecimento do enunciado)
 * a resposta só depende do cluster, por isso é calculada uma vez e
 * guardada no closest_outside do cluster. são K*(C-K) comparações,
 * com K o tamanho do cluster de city
 * devolve -2 se a cidade não existir ou se só houver um cluster
 *----------------------------------------------------------------- */

int closest_city_to_cluster(quick_union *map, ClusterSet *set,
                            position *coordinates, int ncity, int city) {
  /* validado antes da cache, por isso o -2 nunca fica guardado */
  if (city < 1 || city > ncity || set->count == 1)
    return -2;

  int idx = set->cluster_of_root[find(map, city)];
  Cluster *own = &set->clusters[idx];

  if (own->closest_outside != 0)
    return own->closest_outside;

  int closest = -2;
  long best_distance = 0;

  for (int m = 0; m < own->size; m++) {
    position member = coordinates[own->members[m]];
    for (int k = 1; k <= set->count; k++) {
      if (k == idx)
        continue;
      const Cluster *other = &set->clusters[k];
      for (int j = 0; j < other->size; j++) {
        int candidate = other->members[j];
        long distance = squared_distance(member, coordinates[candidate]);
        if (closest == -2 || distance < best_distance ||
            (distance == best_distance && candidate < closest)) {
          closest = candidate;
          best_distance = distance;
        }
      }
    }
  }
  own->closest_outside = closest;
  return closest;
}
