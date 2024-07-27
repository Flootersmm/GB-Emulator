#include <stdio.h>  // for getc, printf
#include <stdlib.h> // malloc, free
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "ijvm.h"
#include "print.h" // actual nice debug prints, also for debugger bonus
#include "util.h" // read this file for debug prints, endianness helper functions
#include "stack.h"
#include "heap.h"
#include "ops.h"

// IJVM ////////////////////////////////////////////////////////////////////////
int read_section(FILE *fp, word_t *data_size, byte_t **data) {
  // We grab the origin, the data size, and the data itself. fread advances the
  // file position indicator for the stream, so we're automatically moving
  // through the file as we do this.
  word_t origin;
  // fread takes an address and puts the result into the thing at the address.
  fread(&origin, sizeof(word_t), 1, fp);
  fread(data_size, sizeof(word_t), 1, fp);
  // and we then swap the endianness of the thing.
  origin = swap_uint32(origin);
  *data_size = swap_uint32(*data_size);

  *data = malloc(*data_size);
  if (*data == NULL) {
    fprintf(stderr, "Failed to allocate memory\n");
    fclose(fp);
    return -1;
  }
  if (fread(*data, sizeof(byte_t), *data_size, fp) != *data_size) {
    fprintf(stderr, "Failed to read data\n");
    free(*data);
    fclose(fp);
    return -1;
  }
  return 0;
}

ijvm *init_ijvm(char *binary_path, FILE *input, FILE *output) {
  ijvm *m = (ijvm *)malloc(sizeof(ijvm));
  if (m == NULL) {
    fprintf(stderr, "Failed to allocate memory for IJVM\n");
    return NULL;
  }

  *m = (ijvm){.in = input,
              .out = output,
              .const_pool_data = NULL,
              .text_data = NULL,
              .r = {.pc = 0, .old_pc = 0, .lv = 0, .old_lv = 0},
              .MAX_FRAMES = 2048,
              .MAX_CONNECTIONS = 16,
              .MAIN_LOCAL_VAR_COUNT = 256,
              .INITIAL_STACK_SIZE = 1024,
              .stack.frame_locals = 256,
              .halting = 0};

  FILE *fp = fopen(binary_path, "rb");
  if (fp == NULL) {
    fprintf(stderr, "Error opening file\n");
    fclose(fp);
    free(m);
    return NULL;
  }

  // IJVM binaries are as such:
  // 4 bytes magic
  //
  // 4 bytes const origin
  // 4 bytes const size
  // n bytes const
  //
  // 4 bytes text origin
  // 4 bytes text size
  // n bytes text

  word_t magic;
  if (fread(&magic, sizeof(word_t), 1, fp) != 1) {
    fprintf(stderr, "Failed to read magic number\n");
    fclose(fp);
    free(m);
    return NULL;
  }
  magic = swap_uint32(magic);

  // Our magic number is 0x1DEADFAD.
  if (magic != MAGIC_NUMBER) {
    fprintf(stderr, "Invalid magic number: %x\n", magic);
    fclose(fp);
    free(m);
    return NULL;
  }

  if (read_section(fp, &m->const_pool_size, &m->const_pool_data) != 0 ||
      read_section(fp, &m->text_size, &m->text_data) != 0) {
    fclose(fp);
    free(m);
    return NULL;
  }
  fclose(fp);

  // Set in util.h
  if (DEBUG_LEVEL == 4) {
    print_file(m);
  }

  // Malloc stack, heap, and call_stack.
  // Call_stack is for debug printing and provides no intepreter functionality.
  if (init_stack(m) != 0 || init_heap(m) != 0 || init_call_stack(m) != 0) {
    destroy_ijvm(m);
    return NULL;
  }

  return m;
}

void destroy_ijvm(ijvm *m) {
  // Free the memory allocated for constant pool and text sections,
  // the stack, the heap, the callstack, then the ijvm itself.
  if (m == NULL) {
    return;
  }
  if (m->const_pool_data) {
    free(m->const_pool_data);
  }
  if (m->text_data) {
    free(m->text_data);
  }
  free_stack(m);
  free_heap(m);
  free_call_stack(m);
  free(m);
}

byte_t *get_text(ijvm *m) { return m->text_data; }

unsigned int get_text_size(ijvm *m) { return m->text_size; }

word_t get_constant(ijvm *m, int i) {
  if (i < 0 || i >= m->const_pool_size / sizeof(word_t)) {
    fprintf(stderr, "Constant pool index out of bounds\n");
    return 0;
  }

  // const_pool is obviously in bytes, but we cast to words so each index is a
  // whole constant and not half of one.
  word_t *constant = (word_t *)m->const_pool_data;
  return swap_uint32(constant[i]);
}

unsigned int get_program_counter(ijvm *m) { return m->r.pc; }

word_t get_local_variable(ijvm *m, int i) {
  if (m->r.lv + i >= m->stack.capacity || m->r.lv + i < 0) {
    fprintf(stderr, "Stack index out of bounds\n");
    destroy_ijvm(m);
    exit(EXIT_FAILURE);
  }
  return m->stack.items[m->r.lv + i];
}

void set_local_variable(ijvm *m, int i, word_t value) {
  if (m->r.lv + i >= m->stack.capacity || m->r.lv + i < 0) {
    fprintf(stderr, "Stack index out of bounds\n");
    destroy_ijvm(m);
    exit(EXIT_FAILURE);
  }
  m->stack.items[m->r.lv + i] = value;
}

bool finished(ijvm *m) {
  if (m->r.pc >= m->text_size) {
    return true;
  }
  if (m->halting == 1) {
    return true;
  }
  return false;
}

// INSTRUCTIONS ////////////////////////////////////////////////////////////////

void step(ijvm *m) {
  byte_t instruction = get_instruction(m);

  m->r.pc++;

  switch (DEBUG_LEVEL) {
  case 2:
    print_stack(m);
    break;
  case 3:
    print_stack(m);
    print_backtrace(m);
    break;
  case 4:
    print_stack(m);
    print_backtrace(m);
    // We also print_file(m) once, in init_ijvm().
    break;
  default:
    break;
  }

  if ((int)instruction < 256 && operations[instruction] != NULL) {
    operations[instruction](m);
  } else {
    fprintf(stderr, "Unknown instruction %02X at PC: %d\n", instruction,
            m->r.pc);
    m->halting = 1;
  }
}

byte_t get_instruction(ijvm *m) {
  if (m->r.pc >= m->text_size) {
    fprintf(stderr,
            "PC out of bounds of .text area: PC = %u, .text Size = %u\n",
            m->r.pc, m->text_size);
  }
  return get_text(m)[get_program_counter(m)];
}

ijvm *init_ijvm_std(char *binary_path) {
  return init_ijvm(binary_path, stdin, stdout);
}

void run(ijvm *m) {
  while (!finished(m)) {
    step(m);
  }
}

// Below: methods needed by bonus assignments, see ijvm.h
// You can leave these unimplemented if you are not doing these bonus
// assignments.

int get_call_stack_size(ijvm *m) { return (m->stack.sp - m->r.lv); }

// Checks if reference is a freed heap array. Note that this assumes that
//
bool is_heap_freed(ijvm *m, word_t reference) {
  // TODO: implement me if doing garbage collection bonus
  return 0;
}
