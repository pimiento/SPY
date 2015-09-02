#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void next_2n(int v) {
  int r;
  if (v > 1) {
    float f = (float)v;
    unsigned int const t = 1U << ((*(unsigned int *)&f >> 23) - 0x7f);
    r = t << (t < v);
  } else {
    r = 1;
  }
  printf("%d -> %d\n", v, r);
}

int main(void) {
  next_2n(17);
  next_2n(200000);
  printf("%zu\n", lround(4.44089e-16));

  return 0;
}
