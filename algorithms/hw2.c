
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define MAX (1000*1000)
int32_t elements[MAX];
int32_t extmemory[MAX];

void merge(size_t i_a, size_t i_b, size_t i_end) {
  size_t i0 = i_a;
  size_t i1 = i_b;

  for (size_t idx = i_a; idx < i_end; idx++) {
    if (i0 < i_b && (i1 >= i_end || elements[i0] <= elements[i1])) {
      extmemory[idx] = elements[i0];
      i0++;
    } else {
      extmemory[idx] = elements[i1];
      i1++;
    }
  }

  for (size_t idx = i_a; idx < i_end; idx++) {
    elements[idx] = extmemory[I];
  }
}

size_t min(size_t a, size_t b) {
  if (a < b) return a;
  else       return b;
}

void elements_print(size_t size) {
  for (size_t i = 0; i < size; i++)
    fprintf(stdout, "%"PRId32" ", elements[i]);
}

int main(void) {
  int32_t m, n;
  if (scanf("%"SCNd32" %"SCNd32, &n, &m) != 2) {
    exit(1);
  }
  size_t size = m*n;

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < m; j++) {
      if (scanf("%"SCNd32, (elements+(i*m)+j)) != 1) {
        exit(1);
      }
    }
  }

  for (size_t width = m; width < size; width *= 2) {
    for (size_t sub = 0; sub < size; sub += (2*width)) {
      merge(sub, min(sub+width, size), min(sub+2*width, size));
    }
  }

  elements_print(size);
  return 0;
}
