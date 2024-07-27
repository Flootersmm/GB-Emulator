#ifndef NETWORK_H
#define NETWORK_H

#include "ijvm.h"

void init_network(ijvm *m);
int find_free_connection(ijvm *m);
void close_connection(ijvm *m, int index);

#endif
