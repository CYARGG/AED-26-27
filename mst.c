#include "mst.h"
#include "cluster.h"
#include <limits.h>
#include <math.h>
#include <stdlib.h>

struct connection {

  int lower_city;
  int higher_city;
  long squared_distance;
};

struct mst {

  connection *connections;
  int number_connections;
  double total_cost;
};

static void matrix_free(connection **matrix, int rows) {
  if (matrix == NULL)
    return;
  for (int i = 0; i < rows; i++) {
    free(matrix[i]);
  }
  free(matrix);
}

static connection **matrix_create(int k) {
  connection **matrix = malloc(k * sizeof(connection *));
  if (matrix == NULL) {
    return NULL;
  }
  for (int i = 0; i < k; i++) {
    matrix[i] = malloc(k * sizeof(connection));
    if (matrix[i] == NULL) {
      matrix_free(matrix, i);
      return NULL;
    }
    for (int j = 0; j < k; j++) {
      matrix[i][j].squared_distance = LONG_MAX;
      matrix[i][j].higher_city = 0;
      matrix[i][j].lower_city = 0;
    }
  }
  return matrix;
}

static void prim(connection **matrix, int k, int *st, int *fr, long *val) {
  for (int i = 0; i < k; i++) {
    st[i] = -1;
    fr[i] = i;
    val[i] = LONG_MAX;
  }
  st[0] = 0;
  val[k] = LONG_MAX;

  int min = 0;

  while (min != k) {

    int i = min;
    st[i] = fr[i];
    min = k;

    for (int j = 0; j < k; j++) {

      if (st[j] == -1) {

        if (matrix[i][j].squared_distance < val[j]) {

          val[j] = matrix[i][j].squared_distance;
          fr[j] = i;
        }

        if (val[j] < val[min]) {
          min = j;
        }
      }
    }
  }
}

static int compare_connections(const void *a, const void *b) {
  const connection *x = a;
  const connection *y = b;

  if (x->lower_city < y->lower_city)
    return -1;

  if (x->lower_city > y->lower_city)
    return 1;

  if (x->higher_city < y->higher_city)
    return -1;

  if (x->higher_city > y->higher_city)
    return 1;

  return 0;
}

mst *mst_initialize(const ClusterSet *set, const position *coordinates,
                    int ncity) {

  int number_clusters = get_number_of_clusters(set);

  mst *tree = NULL;
  connection **matrix = NULL;
  int *st = NULL;
  int *fr = NULL;
  long *val = NULL;

  tree = malloc(sizeof(mst));
  if (tree == NULL) {
    goto error;
  }
  tree->connections = NULL;
  tree->number_connections = 0;
  tree->total_cost = 0;

  if (number_clusters <= 1) {
    return tree;
  }

  tree->connections = malloc((number_clusters - 1) * sizeof(connection));

  if (tree->connections == NULL) {
    goto error;
  }

  matrix = matrix_create(number_clusters);

  if (matrix == NULL) {
    goto error;
  }

  st = malloc((number_clusters + 1) * sizeof(int));

  fr = malloc((number_clusters + 1) * sizeof(int));

  val = malloc((number_clusters + 1) * sizeof(long));

  if (st == NULL || fr == NULL || val == NULL) {
    goto error;
  }

  for (int i = 0; i < number_clusters; i++) {
    for (int j = i + 1; j < number_clusters; j++) {
      int city_a, city_b;
      long distance = shortest_edge_between_clusters(set, coordinates, i + 1,
                                                     j + 1, &city_a, &city_b);

      matrix[i][j].squared_distance = distance;

      if (city_a < city_b) {
        matrix[i][j].lower_city = city_a;
        matrix[i][j].higher_city = city_b;
      } else {

        matrix[i][j].lower_city = city_b;
        matrix[i][j].higher_city = city_a;
      }

      matrix[j][i] = matrix[i][j];
    }
  }

  prim(matrix, number_clusters, st, fr, val);
  for (int i = 1; i < number_clusters; i++) {
    tree->connections[i - 1] = matrix[st[i]][i];
    tree->total_cost += sqrt(tree->connections[i - 1].squared_distance);
  }
  tree->number_connections = number_clusters - 1;

  qsort(tree->connections, tree->number_connections, sizeof(connection),
        compare_connections);

  matrix_free(matrix, number_clusters);
  free(st);
  free(fr);
  free(val);
  return tree;

error:
  matrix_free(matrix, number_clusters);
  free(st);
  free(fr);
  free(val);
  mst_cleanup(tree);
  return NULL;
}

int mst_get_count(const mst *tree) { return tree->number_connections; }

double mst_get_total(const mst *tree) { return tree->total_cost; }

int mst_get_start(const mst *tree, int i) {
  return tree->connections[i].lower_city;
}

int mst_get_ending(const mst *tree, int i) {
  return tree->connections[i].higher_city;
}

double mst_get_distance(const mst *tree, int i) {
  return sqrt(tree->connections[i].squared_distance);
}

void mst_cleanup(mst *tree) {
  if (tree == NULL) {
    return;
  }
  free(tree->connections);
  free(tree);
}
