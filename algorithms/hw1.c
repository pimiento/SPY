
#include <stdio.h>
#include <stdlib.h>

#define MAX 1000
#define DIM 3

int main(void) {
  size_t n;
  size_t result;
  size_t max = 1;
  size_t cubes[MAX][DIM];
  size_t* cur;

  scanf("%zu", &n);

  for (size_t i = 0; i < n; i++) {
    if (scanf("%zu %zu %zu", cubes[i], cubes[i]+1, cubes[i]+2) == EOF) {
      exit(1);
    }
  }
  for (size_t i = 0; i < n-1; i++) {
    result = 1;
    cur = cubes[i];
    for (size_t j = i+1; j < n; j++) {
      if (cur[0] < cubes[j][0] &&
          cur[1] < cubes[j][1] &&
          cur[2] < cubes[j][2]) {
        cur = cubes[j];
        result++;
      }
    }
    if (result > max) {
      max = result;
    }
  }
  printf("%zu\n", max);

  return 0;
}
