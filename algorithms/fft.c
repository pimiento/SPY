#include <math.h>
#include <stdio.h>
#include <complex.h>
#include <stdbool.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
typedef double complex cplx;

void _fft(cplx buf[], cplx out[], int n, int step, bool inv) {
  if (step < n) {
    _fft(out, buf, n, step * 2, inv);
    _fft(out + step, buf + step, n, step * 2, inv);

    for (int i = 0; i < n; i += 2 * step) {
      cplx t = cexp(-I * M_PI * i / n * (inv ? -1 : 1)) * out[i + step];
      buf[i / 2]     = out[i] + t;
      buf[(i + n)/2] = out[i] - t;
      if (inv) {
        buf[i / 2] /= 2;
        buf[(i + n)/2] /= 2;
      }
    }
  }
}

void fft(cplx buf[], int n, bool inv)
{
  cplx out[n];
  for (int i = 0; i < n; i++) out[i] = buf[i];

  _fft(buf, out, n, 1, inv);
}


void show(const char * s, cplx buf[]) {
  printf("%s", s);
  for (int i = 0; i < 8; i++)
    if (!cimag(buf[i]))
      printf("%g ", creal(buf[i]));
    else
      printf("(%g, %g) ", creal(buf[i]), cimag(buf[i]));
}

void mult(cplx* buf1, cplx* buf2, cplx* out, int n) {
  for (size_t i = 0; i < n; i++)
    out[i] = buf1[i] * buf2[i];
}

void sum(cplx* buf1, cplx* buf2, cplx* buf3, cplx* buf4, cplx* out, int n) {
  for (size_t i = 0; i < n; i++)
    out[i] = buf1[i] + buf2[i] + buf3[i] + buf4[i];
}

int main()
{
  cplx buf[] = { (4.0 + 0.0*I), (-2.414214 + -2.414214*I),
                 (0.0 + 2.0*I), (0.414214 + -0.414214*I),
                 (0.0 + 0.0*I), (0.414214 + 0.414214*I),
                 (0.0 + -2.0*I),  (-2.414214 + 2.414214 * I) };

  /* show("Data: ", buf); */
  fft(buf, 8, false);
  /* show("\nFFT : ", buf); */
  fft(buf, 8, true);
  show("\nIFFT : ", buf);

  return 0;
}
