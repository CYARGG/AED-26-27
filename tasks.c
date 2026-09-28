/*chama as funções que resolvem as tasks
 * for bla bla uasodp() */
#include "tasks.h"
#include "union.h"
#include <stdlib.h>

/*-------------------------- task 2 --------------------------
 * recebe o parent do union-find, o número de clusters (task 1)
 * e o número de cidades, e agrupa as cidades por cluster; usa
 * um array em que cada posição guarda o ponteiro para o array
 * de um cluster (uma struct com size e members). cada cluster
 * fica com as cidades por ordem crescente, e os clusters saem
 * ordenados entre si pela cidade mais pequena de cada um
 *-------------------------------------------------------------- */

Cluster *task_2(int parent[], int cluster_number, int ncity) {
  int *cluster_of_root = calloc(ncity + 1, sizeof(int));

  if (cluster_of_root == NULL) {
    return NULL;
  }

  Cluster *clusters = calloc(cluster_number + 1, sizeof(Cluster));

  if (clusters == NULL) {
    return NULL;
  }

  int cluster_count = 0;

  for (int i = 1; i <= ncity; i++) {

    int root = find(parent, i);

    if (cluster_of_root[root] == 0) {

      cluster_count++;
      cluster_of_root[root] = cluster_count;
      clusters[cluster_count].members = malloc(sizeof(int));
      if (clusters[cluster_count].members == NULL) {
        return NULL;
      }
      clusters[cluster_count].members[0] = i;
      clusters[cluster_count].size = 1;
    } else {

      int cluster_id = cluster_of_root[root];
      clusters[cluster_id].size++;
      clusters[cluster_id].members =
          realloc(clusters[cluster_id].members,
                  clusters[cluster_id].size * sizeof(int));
      if (clusters[cluster_id].members == NULL) {
        return NULL;
      }
      clusters[cluster_id].members[clusters[cluster_id].size - 1] = i;
    }
  }
  free(cluster_of_root);
  return clusters;
}
