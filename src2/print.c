#include <stdio.h>
#include <ctype.h>
#include "ijvm.h"
#include "print.h"

void print_stack(ijvm *m) {
  if (m->r.lv > m->stack.sp) {
    fprintf(stderr, "Local variable index is out of stack bounds.\n");
    return;
  }

  printf("=================================================================\n");
  printf(BLU "%10s     %-10s    %-10s\n", "Index", "Hex", "Dec" RESET);

  int consecutive_zeros = 0;
  int start_zero_index = 0;

  // Through LV to SP, we print the stack. However, we don't want to print
  // massive empty spaces (like how hexdump doesn't), so we count consecutive
  // zeros. If we hit a first zero, we set start_zero_index and
  // consecutive_zeros++. If we don't hit a zero, but we had more than five
  // beforehand, we print ... and then the non-zero data.

  for (int i = m->r.lv; i <= m->stack.sp; i++) {
    if (m->stack.items[i] == 0) {
      if (consecutive_zeros == 0) {
        start_zero_index = i;
        printf("%10d     0x%08X    %-10d\n", i, m->stack.items[i],
               m->stack.items[i]);
      }
      consecutive_zeros++;
    } else {
      if (consecutive_zeros >= 5) {
        printf("          ...\n");
        printf("%10d     0x%08X    %-10d\n", i - 1, m->stack.items[i - 1],
               m->stack.items[i - 1]);
      } else if (consecutive_zeros > 1) {
        for (int j = start_zero_index + 1; j < i; j++) {
          printf("%10d     0x%08X    %-10d\n", j, m->stack.items[j],
                 m->stack.items[j]);
        }
      }
      consecutive_zeros = 0;

      printf("%10d    ", i);
      if (m->stack.items[i] & 0x80000000) {
        printf("-0x%08X    %-10d", -m->stack.items[i], m->stack.items[i]);
      } else {
        printf(" 0x%08X    %-10d", m->stack.items[i], m->stack.items[i]);
      }

      if (i == m->r.lv) {
        printf(" <- LV (Base of Frame)");
      }
      if (i == m->r.lv + m->stack.frame_locals - 1) {
        printf(" <- End of Locals");
      }
      if (i == m->stack.sp) {
        printf(" <- SP (Top of Stack)");
      }
      printf("\n");
    }
  }

  if (consecutive_zeros >= 5) {
    printf("          ...\n");
    printf("%10d     0x%08X    %20d\n", m->stack.sp,
           m->stack.items[m->stack.sp], m->stack.items[m->stack.sp]);
  } else if (consecutive_zeros > 1) {
    for (int j = start_zero_index + 1; j <= m->stack.sp; j++) {
      printf("%10d     0x%08X    %20d\n", j, m->stack.items[j],
             m->stack.items[j]);
    }
  }
}

void print_call_stack(ijvm *m) {
  // Using our call_stack struct, we print each frame, its return addr and args.
  printf(BLU "Backtrace:\n" RESET);
  for (int i = m->call_stack.top; i >= 0; i--) {
    CallStackFrame *frame = &m->call_stack.frames[i];
    printf("Frame %d: Return Address = %08x, Args = [", i,
           frame->return_address);
    for (int j = 0; j < frame->num_args; j++) {
      printf("0x%08X", frame->local_variables[j]);
      if (j < frame->num_args - 1) {
        printf(", ");
      }
    }
    printf("]\n");
  }
}

void print_file(ijvm *m) {
  // Similar to hexdump -C, we print the file similar to:
  //
  // 00000000  10 00 10 09 59 10 01 9f 00 2e 5f 10 01 60 5f 59  ....Y....._..`_Y
  // 00000010  10 01 7e 99 00 0d 59 59 60 60 10 01 60 a7 ff e7  ..~...YY``..`...
  // 00000020  10 01 5f 10 02 64 59 99 00 0a 5f 10 01 60 a7 ff  .._..dY..._..`..
  // 00000030  f4 57 a7 ff d2 57 10 ba 5f b0 ff                 .W...W.._..
  // PC :   2, Instruction: 0x10

  if (m->const_pool_size > 0) {
    printf(BLU "Constant Pool:\n" RESET);
    for (word_t i = 0; i < m->const_pool_size; i += 16) {
      printf("%08x  ", i);
      for (int j = 0; j < 16; ++j) {
        if (i + j < m->const_pool_size) {
          printf("%02x ", m->const_pool_data[i + j]);
        } else {
          printf("   ");
        }
      }
      printf(" ");
      // Printing chars, probably pointless tbh but it looks fun.
      for (int j = 0; j < 16; j++) {
        if (i + j < m->const_pool_size) {
          char c = m->const_pool_data[i + j];
          printf("%c", isprint(c) ? c : '.');
        } else {
          printf(" ");
        }
      }
      printf("\n");
    }
  }

  printf(BLU "Text Section:\n" RESET);
  for (word_t i = 0; i < m->text_size; i += 16) {
    printf("%08x  ", i);
    for (int j = 0; j < 16; ++j) {
      if (i + j < m->text_size) {
        if (i + j == m->r.pc) {
          printf(RED "%02x " RESET, m->text_data[i + j]);
        } else {
          printf("%02x ", m->text_data[i + j]);
        }
      } else {
        printf("   ");
      }
    }
    printf(" ");
    for (int j = 0; j < 16; ++j) {
      if (i + j < m->text_size) {
        char c = m->text_data[i + j];
        if (i + j == m->r.pc) {
          printf(RED "%c" RESET, isprint(c) ? c : '.');
        } else {
          printf("%c", isprint(c) ? c : '.');
        }
      } else {
        printf(" ");
      }
    }
    printf("\n");
  }
  byte_t instruction = get_instruction(m);
  printf(RED "PC :%4d, Instruction: 0x%02X \n\n" RESET, m->r.pc, instruction);
}
