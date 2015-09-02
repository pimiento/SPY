#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define SUCCESSCODE 0
#define ERRORCODE -1

double length(int32_t* p1, int32_t* p2) {
  return hypot(p1[0] - p2[0], p1[1] - p2[1]);
}

size_t min_key(double* key, bool* mst_set, size_t n);
double mst_max(double* edges, size_t n);
double find_max(double* edges, int16_t* stored_mst, size_t n);

#define EDGES(i,j,n) edges[i+(j*n)]

int main(void) {
  int ret_value = ERRORCODE;
  size_t n;
  if (scanf("%zu", &n) != 1)
    exit(1);

  int32_t* agents = NULL;
  double* edges = NULL;
  if ((agents = malloc(2*n*sizeof(int32_t))) == NULL)
    goto free_all;
  if ((edges = malloc(n*n*sizeof(double))) == NULL)
    goto free_all;
  memset(edges, 0, n);

  for (size_t i = 0; i < 2*n; i++) {
    if (scanf("%"SCNd32, agents+i) != 1)
      goto free_all;
  }

  for (size_t i = 0; i < n; i++) {
    for (size_t j = i + 1; j < n; j++) {
      EDGES(i,j,n) = length(agents+(i*2), agents+(j*2));
      EDGES(j,i,n) = EDGES(i, j,n);
    }
  }

  double result;
  if ((result = mst_max(edges, n)) < 0)
    goto free_all;
  printf("%.10f\n", result);

  ret_value = SUCCESSCODE;
free_all:
  free(agents);
  free(edges);
  return ret_value;
}

double mst_max(double* edges, size_t n) {
  double ret_value = ERRORCODE;

  int16_t* stored_mst  = NULL;
  double*  key     = NULL;
  bool*    mst_avail = NULL;
  if ((stored_mst = malloc(n * sizeof(int16_t))) == NULL)
    goto free_all;

  if ((key = malloc(n * sizeof(double))) == NULL)
    goto free_all;

  if ((mst_avail = malloc(n * sizeof(bool))) == NULL)
    goto free_all;

  for (size_t i = 0; i < n; i++) {
    key[i] = INFINITY;
    mst_avail[i] = true;
  }
  key[0] = 0.0;
  stored_mst[0] = -1;

  size_t u;
  for (size_t count = 0; count < n-1; count++) {
    u = min_key(key, mst_avail, n);
    mst_avail[u] = false;
    for (size_t v = 0; v < n; v++) {
      if (mst_avail[v] && EDGES(u,v,n) < key[v]) {
        stored_mst[v] = u;
        key[v] = EDGES(u,v,n);
      }
    }
  }
  ret_value = find_max(edges, stored_mst, n);

free_all:
  free(stored_mst);
  free(key);
  free(mst_avail);
  return ret_value;
}

size_t min_key(double* key, bool* mst_avail, size_t n) {
  double min = INFINITY;
  size_t min_index = 0;
  for (size_t v = 0; v < n; v++) {
    if (mst_avail[v] && key[v] < min) {
      min = key[v];
      min_index = v;
    }
  }
  return min_index;
}

double find_max(double* edges, int16_t* stored_mst, size_t n) {
  double ret_value = ERRORCODE;
  for (size_t k = 1; k < n; k++)
    if (EDGES(k, stored_mst[k], n) > ret_value)
      ret_value = EDGES(k, stored_mst[k], n);
  return ret_value;
}
