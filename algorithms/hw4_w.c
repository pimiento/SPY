#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define SIZES 3
#define HASH(v,e) (((3 * v[0] + 5 * v[1] + 7 * v[2]) % 15000000047) % e)
#define SWAP(type, e1, e2) {type temp; temp = *(e1); *(e1) = *(e2); *(e2) = temp;}

uint32_t _gcd(uint32_t a, uint32_t b) {
  while (b) {
    a %= b;
    SWAP(uint32_t, &a, &b);
  }
  return a;
}

uint32_t gcd(uint32_t *triangle) {
  return _gcd(triangle[0],
              _gcd(triangle[1], triangle[2]));
}

size_t find_class(uint32_t *triangle);
void sort_triangle(uint32_t *triangle);
size_t lookup(uint64_t key, uint32_t *values);

static size_t elements;
typedef struct Element Element;
struct Element {
  uint64_t key;
  uint32_t values[SIZES];
  struct Element *next;
};
static Element **hashtable = NULL;

int main(void) {
  size_t result = 0;
  if (scanf("%zu", &elements) != 1) {
    exit(1);
  }

  uint32_t* triangle = NULL;
  if ((triangle = malloc(SIZES*sizeof(uint32_t))) == NULL)
    goto free_all;

  if ((hashtable = malloc(elements*sizeof(Element))) == NULL)
    goto free_all;

  for (size_t i = 0; i < elements; i++) {
    if (scanf("%"SCNu32" %"SCNu32" %"SCNu32,
              triangle, triangle+1, triangle+2) != 3) {
      goto free_all;
    } else {
      result += find_class(triangle);
    }
  }

  printf("%zu\n", result);
free_all:
  if (triangle != NULL)  free(triangle);
  if (hashtable != NULL) free(hashtable);
  return 0;
}

size_t find_class(uint32_t *triangle) {
  sort_triangle(triangle);
  uint32_t abc_gcd = gcd(triangle);
  uint32_t values[SIZES];

  for (size_t i = 0; i < SIZES; i++) {
    values[i] = triangle[i] / abc_gcd;
  }
  uint64_t key = HASH(values, elements);

  return lookup(key, values);
}

void sort_triangle(uint32_t *triangle) {
  if (triangle[0] > triangle[1]) { SWAP(uint32_t, triangle, triangle+1);  }
  if (triangle[1] > triangle[2]) { SWAP(uint32_t, triangle+1, triangle+2);}
  if (triangle[0] > triangle[1]) { SWAP(uint32_t, triangle, triangle+1);  }
}

size_t lookup(uint64_t key, uint32_t *values) {
  Element *e;
  /* that equality class is already in hashtable */
  for (e = hashtable[key]; e != NULL; e = e->next) {
    if (e->values[0] == values[0] &&
        e->values[1] == values[1] &&
        e->values[2] == values[2]) {
      return 0;
    }
  }
  /* add new equality class */
  if ((e = malloc(sizeof(Element))) == NULL) {
    exit(1);
  }
  e->key = key;
  for (size_t i = 0; i < SIZES; i++) {
    e->values[i] = values[i];
  }
  e->next = hashtable[key];
  hashtable[key] = e;
  return 1;
}
