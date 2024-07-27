#ifndef STACK_H
#define STACK_H

#include "ijvm.h"

int init_stack(ijvm *m);
void push(ijvm *m, word_t item);
word_t pop(ijvm *m);
word_t tos(ijvm *m);
void free_stack(ijvm *m);
int init_call_stack(ijvm *m);
void extend_frames(ijvm *m);
void free_call_stack(ijvm *m);

#endif
