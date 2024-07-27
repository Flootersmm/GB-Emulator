#ifndef HEAP_H
#define HEAP_H

#include "ijvm.h"

int init_heap(ijvm *m);
void extend_heap(ijvm *m);
int allocate_array(ijvm *m, int size);
void free_heap(ijvm *m);

#endif
