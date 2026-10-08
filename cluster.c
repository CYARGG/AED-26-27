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

/*TODO: se algum falhar, fica por libertar os malloc/callocs anteriores*/

Cluster *distribute_by_cluster(quick_union *map, int ncity) {
  int *cluster_of_root = calloc(ncity + 1, sizeof(int));

  if (cluster_of_root == NULL) {
    return NULL;
  }

  Cluster *clusters = calloc((get_cluster_count(map) + 1), sizeof(Cluster));

  if (clusters == NULL) {
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
