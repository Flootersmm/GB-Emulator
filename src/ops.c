#include "gb.h"

void nop() {
  // Do nothing
}

/// Opcode function pointer array population with debug messages
OPS opcodes = {
    {"NOP", 0, nop},
    {"LD BC, 0x%04X", 2, NULL},
    {"LD (BC), A", 0, NULL},

};
