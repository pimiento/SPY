#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#define SIZES 3
#define HASH(v, e) (((3*v[0] + 5*v[1] + 7*v[2]) % 15000000047) % e)

struct Element {
  uint64_t key;
  uint32_t values[SIZES];
  struct Element* next;
};

void swap(uint32_t* e1, uint32_t* e2);
void free_mem(uint64_t* keys, size_t n, struct Element** hashtable);
uint32_t gcd_three(uint32_t* triangle);
uint64_t find_class(uint32_t* triangle, size_t n);
void sort_triangle(uint32_t* triangle);
bool keycmp(uint32_t* triangle1, uint32_t* triangle2);
size_t lookup(uint64_t key, uint32_t* values, struct Element** hashtable);

int main(void) {
  int ret_value = 1;
  size_t result = 0;
  uint32_t triangle[SIZES];
  uint64_t* keys = NULL;

  size_t n;
  struct Element** hashtable = NULL;
  if (scanf("%zu", &n) != 1)
    goto free_all;
  if ((hashtable = malloc(n*sizeof(struct Element))) == NULL)
    goto free_all;
  if ((keys = malloc(n*sizeof(uint64_t))) == NULL)
    goto free_all;

  size_t lookup_res;
  for (size_t i = 0; i < n; i++) {
    if (scanf("%"SCNu32" %"SCNu32" %"SCNu32,
              triangle, triangle+1, triangle+2) != 3)
      goto free_all;

    sort_triangle(triangle);
    keys[i] = find_class(triangle, n);
    if ((lookup_res = lookup(keys[i], triangle, hashtable)) == (size_t)(-1))
      goto free_all;

    result += lookup_res;
  }

  printf("%zu\n", result);
  ret_value = 0;

free_all:
  free_mem(keys, n, hashtable);
  return ret_value;
}

uint64_t find_class(uint32_t* triangle, size_t n) {
  uint32_t abc_gcd = gcd_three(triangle);
  for (size_t i = 0; i < SIZES; i++) {
    triangle[i] /= abc_gcd;
  }
  return HASH(triangle, n);
}

void sort_triangle(uint32_t* triangle) {
  if (triangle[0] > triangle[1]) { swap(triangle, triangle+1);  }
  if (triangle[1] > triangle[2]) { swap(triangle+1, triangle+2);}
  if (triangle[0] > triangle[1]) { swap(triangle, triangle+1);  }
}

size_t lookup(uint64_t key, uint32_t* values, struct Element** hashtable) {
  struct Element* e;
  for (e = hashtable[key]; e != NULL; e = e->next) {
    if (keycmp(e->values, values)) {
      return 0;
    }
  }

  if ((e = malloc(sizeof(struct Element))) == NULL)
    return -1;

  e->key = key;
  for (size_t i = 0; i < SIZES; i++) {
    e->values[i] = values[i];
  }
  e->next = hashtable[key];
  hashtable[key] = e;
  return 1;
}

void swap(uint32_t* e1, uint32_t* e2) {
  uint32_t temp = *(e1);
  *(e1) = *(e2);
  *(e2) = temp;
}

bool keycmp(uint32_t* triangle1, uint32_t* triangle2) {
  return (triangle1[0] == triangle2[0] &&
          triangle1[1] == triangle2[1] &&
          triangle1[2] == triangle2[2]);
}

uint32_t _gcd(uint32_t a, uint32_t b) {
  while (b) {
    a %= b;
    swap(&a, &b);
  }
  return a;
}
uint32_t gcd_three(uint32_t* triangle) {
  return _gcd(triangle[0],
              _gcd(triangle[1], triangle[2]));
}

void free_mem(uint64_t* keys, size_t n, struct Element** hashtable) {
  for (size_t i = 0; i < n; i++) {
    if (hashtable[keys[i]] != NULL) {
      struct Element *e = hashtable[keys[i]];
      while (e != NULL) {
        struct Element *t = e->next;
        free(e);
        e = t;
      }
      hashtable[keys[i]] = NULL;
    }
  }
  free(hashtable);
  free(keys);
}
