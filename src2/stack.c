#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

int init_stack(ijvm *m) {
  m->stack.items = malloc(m->INITIAL_STACK_SIZE * sizeof(word_t));
  if (m->stack.items == NULL) {
    fprintf(stderr, "Failed to allocate initial memory for the stack\n");
    return -1;
  }
  m->stack.capacity = m->INITIAL_STACK_SIZE;
  m->stack.sp = m->MAIN_LOCAL_VAR_COUNT; // 256 locals in main

  // Set stack items to 0 so we don't get garbage if we go out of bounds.
  memset(m->stack.items, 0, m->MAIN_LOCAL_VAR_COUNT * sizeof(word_t));
  return 0;
}

void push(ijvm *m, word_t item) {
  while (m->stack.sp >= m->stack.capacity) {
    m->stack.capacity *= 2;
    word_t *new_items =
        realloc(m->stack.items, m->stack.capacity * sizeof(word_t));
    if (new_items == NULL) {
      fprintf(stderr, "Failed to reallocate memory for the stack\n");
      free(m->stack.items);
      m->stack.items = NULL;
      m->stack.capacity = 0;
      exit(EXIT_FAILURE);
    }
    m->stack.items = new_items;
  }
  m->stack.items[++m->stack.sp] = item;
}

word_t pop(ijvm *m) {
  if (m->stack.sp == -1) {
    fprintf(stderr, "Stack underflow\n");
    exit(EXIT_FAILURE);
  }
  return m->stack.items[m->stack.sp--];
}

word_t tos(ijvm *m) {
  if (m->stack.sp == -1) {
    fprintf(stderr, "Stack underflow\n");
    exit(EXIT_FAILURE);
  }
  return m->stack.items[m->stack.sp];
}

void free_stack(ijvm *m) {
  if (m->stack.items != NULL) {
    free(m->stack.items);
    m->stack.items = NULL;
  }
}

// FOR DEBUGGER ////////////////////////////////////////////////////////////////
// FUNCTIONS BELOW HAVE NO INTEPRETER FUNCTIONALITY AND ARE JUST FOR PRINTING.

int init_call_stack(ijvm *m) {
  // Malloc frames and their space for locals.
  m->call_stack.frames = malloc(m->MAX_FRAMES * sizeof(CallStackFrame));
  if (m->call_stack.frames == NULL) {
    fprintf(stderr, "Failed to allocate memory for call stack frames\n");
    free_call_stack(m);
    return -1;
  }

  m->call_stack.top = -1;
  return 0;
}

void extend_frames(ijvm *m) {
  // If we hit max frames, double it.
  m->MAX_FRAMES *= 2;
  CallStackFrame *new_frames =
      realloc(m->call_stack.frames, m->MAX_FRAMES * sizeof(CallStackFrame));
  if (new_frames == NULL) {
    fprintf(stderr, "Failed to reallocate memory for the call stack\n");
    free_call_stack(m);
    exit(EXIT_FAILURE);
  }
  m->call_stack.frames = new_frames;
}

void free_call_stack(ijvm *m) {
  if (m->call_stack.frames != NULL) {
    for (int i = 0; i <= m->call_stack.top; i++) {
      free(m->call_stack.frames[i].local_variables);
    }
    free(m->call_stack.frames);
    m->call_stack.frames = NULL;
  }
}
