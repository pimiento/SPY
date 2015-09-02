#include <stdio.h>
#include <inttypes.h>

int main(void) {
  int64_t a, b;
  int32_t c;
  int64_t mod;

  if (fscanf(stdin, "%" SCNd64 " %" SCNd64 " %" SCNd32, &a, &b, &c) == EOF) {
    return 1;
  }
  mod = (a + b) % c;
  /* (a+b)x + mod = C */
  if (mod < 0) { mod += c; }
  printf("%" SCNd64 "\n", mod);
  return 0;
}
