#include "gb.h"

void nop(GB *vm) {
  // Do nothing
}

void jp_nn(GB *vm, u16 operand) {
  // TODO: grab operand with an external function actually
  operand = (vm->cart.data[vm->r.pc]) | (vm->cart.data[vm->r.pc + 0x01] << 8);
  vm->r.pc = operand;
}

void inc_a(GB *vm, u8 operand) {
  vm->r.a++;
  _reg_set_flag(vm, vm->r.a == 0, 0, (vm->r.a & 0x0F) == 0, 0);
}

void inc_b(GB *vm, u8 operand) {
  vm->r.b++;
  _reg_set_flag(vm, vm->r.b == 0, 0, (vm->r.b & 0x0F) == 0, 0);
}

void inc_c(GB *vm, u8 operand) {
  vm->r.c++;
  _reg_set_flag(vm, vm->r.c == 0, 0, (vm->r.c & 0x0F) == 0, 0);
}

void inc_d(GB *vm, u8 operand) {
  vm->r.d++;
  _reg_set_flag(vm, vm->r.d == 0, 0, (vm->r.d & 0x0F) == 0, 0);
}

void inc_e(GB *vm, u8 operand) {
  vm->r.e++;
  _reg_set_flag(vm, vm->r.e == 0, 0, (vm->r.e & 0x0F) == 0, 0);
}

void inc_h(GB *vm, u8 operand) {
  vm->r.h++;
  _reg_set_flag(vm, vm->r.h == 0, 0, (vm->r.h & 0x0F) == 0, 0);
}

void inc_l(GB *vm, u8 operand) {
  vm->r.l++;
  _reg_set_flag(vm, vm->r.l == 0, 0, (vm->r.l & 0x0F) == 0, 0);
}

void inc_hl(GB *vm, u8 operand) {
  u16 address = (vm->r.h << 8) | vm->r.l;
  vm->mem.data[address]++;
  _reg_set_flag(vm, vm->mem.data[address] == 0, 0,
                (vm->mem.data[address] & 0x0F) == 0, 0);
}
