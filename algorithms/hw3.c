#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <complex.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define MAXLEN 200000
#define SWAP(x,y) (size_t )
typedef double complex cplx;
char first_str[MAXLEN];
char second_str[MAXLEN];

void fft(cplx* vec, size_t size, bool invert);
size_t* conv(cplx* first, cplx* second, size_t len);
cplx* encoder(char* str, cplx* encoded_mem, const char* c, size_t len);
size_t find_first(size_t* by_a, size_t* by_g, size_t* by_t, size_t* by_c, size_t count);
size_t find_2n(size_t len);

int main(void) {
  if (fgets(first_str, MAXLEN, stdin) == NULL)  exit(1);
  if (fgets(second_str, MAXLEN, stdin) == NULL) exit(1);

  size_t flen = strlen(first_str) - 1;
  size_t slen = strlen(second_str) - 1;
  size_t len_2n = find_2n(flen);
  cplx* encoded_first  = malloc(len_2n*sizeof(cplx));
  cplx* encoded_second = malloc(len_2n*sizeof(cplx));

  size_t* a_conv = conv(encoder(first_str, encoded_first,  "A", len_2n),
                        encoder(second_str, encoded_second,"A", len_2n), len_2n);
  size_t* c_conv = conv(encoder(first_str, encoded_first,  "C", len_2n),
                        encoder(second_str, encoded_second,"C", len_2n), len_2n);
  size_t* g_conv = conv(encoder(first_str, encoded_first,  "G", len_2n),
                        encoder(second_str, encoded_second,"G", len_2n), len_2n);
  size_t* t_conv = conv(encoder(first_str, encoded_first,  "T", len_2n),
                        encoder(second_str, encoded_second,"T", len_2n), len_2n);

  size_t i = find_first(a_conv, g_conv, t_conv, c_conv, flen - slen);
  printf("%zu\n", i);

  free(encoded_first);
  free(encoded_second);
  free(a_conv);
  free(g_conv);
  free(t_conv);
  free(c_conv);
  exit(0);
}

void mul(cplx* a, cplx* b, cplx* out, size_t len) {
  for (size_t i = 0; i < len; i++)
    out[i] = a[i] * b[i];
}

size_t find_2n(size_t l) {
  if (l > 1) {
    float f = (float) l;
    unsigned int const t = 1U << ((*(unsigned int*)&f >> 23) - 0x7f);
    return 2 * (t << (t < l));
  }
  return 2;
}

cplx* encoder(char* str, cplx* encoded_mem, const char* c, size_t len) {
  memset(encoded_mem, 0.0, len*sizeof(cplx));
  for (size_t i = 0; i < strlen(str) - 1; i++)
      encoded_mem[i] = str[i] == *c ? 1.0 + 0*I : 0.0 + 0*I;
  return encoded_mem;
}

size_t find_first(size_t* by_a, size_t* by_g, size_t* by_t, size_t* by_c, size_t count) {
  size_t temp;
  size_t maximum = 0;
  size_t maximum_idx = 0;
  for (size_t i = 0; i < count; i++) {
    if ((temp = by_a[i] + by_g[i] + by_t[i] + by_c[i]) > maximum) {
      maximum = temp;
      maximum_idx = i;
    }
  }
  return maximum_idx + 1;
}

size_t* conv(cplx* first, cplx* second, size_t len) {
  /* A \conv B = IFFT(FFT(A) mul FFT(B)) */
  cplx* res = malloc(len*sizeof(cplx));
  size_t* res_return = malloc(len*sizeof(size_t));
  fft(first, len, false);
  fft(second, len, false);
  mul(first, second, res, len);
  fft(res, len, true);
  for (size_t i = 0; i < len; i++) {
    printf("%f ", creal(res[i]));
    res_return[i] = floor(creal(res[i]));
  }
  puts("");
  free(res);
  return res_return;
}

void _fft(cplx* vec, cplx* out, size_t len, size_t step, bool invert) {
    if (step < len) {
        _fft(out, vec, len, step * 2, invert);
        _fft(out + step, vec + step, len, step * 2, invert);

        cplx t;
        for (size_t i = 0; i < len; i += 2 * step) {
            t = cexp(-I * M_PI * i/len * (invert ? -1:1)) * out[i + step];
            vec[i / 2] = out[i] + t;
            vec[(i + len)/2] = out[i] - t;
            if (invert) {
                vec[i] /= 2;
                vec[(i + len)/2] /= 2;
            }
        }
    }
}
void fft(cplx* vec, size_t len, bool invert) {
  cplx* out = malloc(len*sizeof(cplx));
  for (size_t i = 0; i < len; i++)
    out[i] = vec[i];
  _fft(vec, out, len, 1, invert);
  free(out);
}
