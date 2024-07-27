#ifndef PRINT_H
#define PRINT_H

#include "ijvm_struct.h"

// Term colours
#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"

void print_stack(ijvm *m);
void print_backtrace(ijvm *m);
void print_file(ijvm *m);

#endif
