
#ifndef IJVM_STRUCT_H
#define IJVM_STRUCT_H

#include "ijvm_types.h"
#include <stdio.h> /* contains type FILE * */
/**
 * All the state of your IJVM machine goes in this struct!
 **/

typedef struct Stack {
  word_t *items;
  int sp;
  int capacity;
  int frame_locals;
} Stack;

typedef struct Registers {
  uint32_t pc;
  uint32_t old_pc;
  uint32_t lv;
  uint32_t old_lv;
} Registers;

typedef struct Connection {
  int sock;
  int conn;
  bool used;
} Connection;

typedef struct {
  int **arrays;
  int *sizes;
  int count;
  int capacity;
} Heap;

typedef struct {
  word_t return_address;
  word_t *local_variables;
  int num_args;
  int num_locals;
} CallStackFrame;

typedef struct {
  CallStackFrame *frames;
  int top;
} CallStack;

typedef struct IJVM {
  // do not changes these two variables
  FILE *in;  // use fgetc(ijvm->in) to get a character from in.
             // This will return EOF if no char is available.
  FILE *out; // use for example fprintf(ijvm->out, "%c", value); to print value
             // to out

  word_t text_size;
  byte_t *text_data;
  word_t const_pool_size;
  byte_t *const_pool_data;

  int MAX_FRAMES; // for call_stack printing
  int MAX_CONNECTIONS;
  int MAIN_LOCAL_VAR_COUNT;
  int INITIAL_STACK_SIZE;

  int halting; // 0 or 1

  Stack stack;
  Registers r;
  Connection conn[16];
  Heap h;
  CallStack call_stack;

} ijvm;

#endif
