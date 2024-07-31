#ifndef TABLES_H
#define TABLES_H

#include "typedefs.h"

extern const unsigned char op_ticks[256];
extern const unsigned char op_extended_ticks[256];
extern const unsigned char ioReset[0x100];
extern const OPS ops[256];
extern const OPS_extended ops_extended[256];
extern Licensee new_licensees[];
extern Licensee old_licensees[];
extern const int new_licensee_table_size;
extern const int old_licensee_table_size;
extern const char *cartridgeTypeStrings[];

#endif
