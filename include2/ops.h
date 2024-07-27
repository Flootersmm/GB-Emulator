#ifndef OPS_H
#define OPS_H

#include "ijvm.h"

// We define an array of function pointers instead of a switch,
// and extern our array to use in ijvm.c
typedef void (*operation_func)(ijvm *);
extern operation_func operations[256];

void op_bipush(ijvm *m);
void op_dup(ijvm *m);
void op_iadd(ijvm *m);
void op_iand(ijvm *m);
void op_ior(ijvm *m);
void op_isub(ijvm *m);
void op_nop(ijvm *m);
void op_pop(ijvm *m);
void op_swap(ijvm *m);
void op_err(ijvm *m);
void op_halt(ijvm *m);
void op_in(ijvm *m);
void op_out(ijvm *m);
void op_goto(ijvm *m);
void op_ifeq(ijvm *m);
void op_iflt(ijvm *m);
void op_if_icmpeq(ijvm *m);
void op_ldc_w(ijvm *m);
void op_iload(ijvm *m);
void op_istore(ijvm *m);
void op_iinc(ijvm *m);
void op_wide(ijvm *m);
void op_invokevirtual(ijvm *m);
void op_ireturn(ijvm *m);
void op_netbind(ijvm *m);
void op_netconnect(ijvm *m);
void op_netin(ijvm *m);
void op_netout(ijvm *m);
void op_netclose(ijvm *m);
void op_tailcall(ijvm *m);
void op_newarray(ijvm *m);
void op_iaload(ijvm *m);
void op_iastore(ijvm *m);

#endif
