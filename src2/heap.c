#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

int init_heap(ijvm *m) {
  m->h.capacity = 10;
  m->h.count = 0;
  m->h.arrays = malloc(m->h.capacity * sizeof(int *));
  m->h.sizes = malloc(m->h.capacity * sizeof(int));
  if (m->h.arrays == NULL || m->h.sizes == NULL) {
    fprintf(stderr, "Failed to allocate memory for the heap\n");
    free_heap(m);
    return -1;
  }
  return 0;
}

void extend_heap(ijvm *m) {
  m->h.capacity *= 2;
  m->h.arrays = realloc(m->h.arrays, m->h.capacity * sizeof(int *));
  m->h.sizes = realloc(m->h.sizes, m->h.capacity * sizeof(int));
  if (m->h.arrays == NULL || m->h.sizes == NULL) {
    fprintf(stderr, "Failed to reallocate memory for the heap\n");
    exit(EXIT_FAILURE);
  }
}

int allocate_array(ijvm *m, int size) {
  if (m->h.count == m->h.capacity) {
    extend_heap(m);
  }
  int *array = malloc(size * sizeof(int));
  if (array == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }
  m->h.arrays[m->h.count] = array;
  m->h.sizes[m->h.count] = size;
  return m->h.count++;
}

void free_heap(ijvm *m) {
  for (int i = 0; i < m->h.count; i++) {
    free(m->h.arrays[i]);
  }
  free(m->h.arrays);
  free(m->h.sizes);
}
