#include "gb.h"

// http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf

void cb_n(GB *vm, uint8_t op) {
  ops_extended[op].func.func_no_op(vm);
  vm->cycles += op_extended_ticks[op];
}

// 3.3.1 8-Bit Loads
// 1. LD nn,n
void ld_b_n(GB *vm, u8 operand) { vm->r.b = operand; }
void ld_c_n(GB *vm, u8 operand) { vm->r.c = operand; }
void ld_d_n(GB *vm, u8 operand) { vm->r.d = operand; }
void ld_e_n(GB *vm, u8 operand) { vm->r.e = operand; }
void ld_h_n(GB *vm, u8 operand) { vm->r.h = operand; }
void ld_l_n(GB *vm, u8 operand) { vm->r.l = operand; }
void ld_a_a(GB *vm) { vm->r.a = vm->r.a; }

// 2. LD r1,r2
void ld_a_b(GB *vm) { vm->r.a = vm->r.b; }
void ld_a_c(GB *vm) { vm->r.a = vm->r.c; }
void ld_a_d(GB *vm) { vm->r.a = vm->r.d; }
void ld_a_e(GB *vm) { vm->r.a = vm->r.e; }
void ld_a_h(GB *vm) { vm->r.a = vm->r.h; }
void ld_a_l(GB *vm) { vm->r.a = vm->r.l; }
void ld_a_bc(GB *vm, u8 operand) { vm->r.a = read_u8(vm, vm->r.bc); }
void ld_a_de(GB *vm, u8 operand) { vm->r.a = read_u8(vm, vm->r.de); }
void ld_a_hl(GB *vm, u8 operand) { vm->r.a = read_u8(vm, vm->r.hl); }
void ld_a_nn(GB *vm, u16 operand) { vm->r.a = read_u8(vm, operand); }
void ld_a_n(GB *vm, u8 operand) { vm->r.a = operand; }
void ld_b_b(GB *vm) { vm->r.b = vm->r.b; }
void ld_b_c(GB *vm) { vm->r.b = vm->r.c; }
void ld_b_d(GB *vm) { vm->r.b = vm->r.d; }
void ld_b_e(GB *vm) { vm->r.b = vm->r.e; }
void ld_b_h(GB *vm) { vm->r.b = vm->r.h; }
void ld_b_l(GB *vm) { vm->r.b = vm->r.l; }
void ld_b_hl(GB *vm, u8 operand) { vm->r.b = read_u8(vm, vm->r.hl); }
void ld_b_a(GB *vm) { vm->r.b = vm->r.a; }
void ld_c_b(GB *vm) { vm->r.c = vm->r.b; }
void ld_c_c(GB *vm) { vm->r.c = vm->r.c; }
void ld_c_d(GB *vm) { vm->r.c = vm->r.d; }
void ld_c_e(GB *vm) { vm->r.c = vm->r.e; }
void ld_c_h(GB *vm) { vm->r.c = vm->r.h; }
void ld_c_l(GB *vm) { vm->r.c = vm->r.l; }
void ld_c_hl(GB *vm, u8 operand) { vm->r.c = read_u8(vm, vm->r.hl); }
void ld_c_a(GB *vm) { vm->r.c = vm->r.a; }
void ld_d_b(GB *vm) { vm->r.d = vm->r.b; }
void ld_d_c(GB *vm) { vm->r.d = vm->r.c; }
void ld_d_d(GB *vm) { vm->r.d = vm->r.d; }
void ld_d_e(GB *vm) { vm->r.d = vm->r.e; }
void ld_d_h(GB *vm) { vm->r.d = vm->r.h; }
void ld_d_l(GB *vm) { vm->r.d = vm->r.l; }
void ld_d_hl(GB *vm, u8 operand) { vm->r.d = read_u8(vm, vm->r.hl); }
void ld_d_a(GB *vm) { vm->r.d = vm->r.a; }
void ld_e_b(GB *vm) { vm->r.e = vm->r.b; }
void ld_e_c(GB *vm) { vm->r.e = vm->r.c; }
void ld_e_d(GB *vm) { vm->r.e = vm->r.d; }
void ld_e_e(GB *vm) { vm->r.e = vm->r.e; }
void ld_e_h(GB *vm) { vm->r.e = vm->r.h; }
void ld_e_l(GB *vm) { vm->r.e = vm->r.l; }
void ld_e_hl(GB *vm, u8 operand) { vm->r.e = read_u8(vm, vm->r.hl); }
void ld_e_a(GB *vm) { vm->r.e = vm->r.a; }
void ld_h_b(GB *vm) { vm->r.h = vm->r.b; }
void ld_h_c(GB *vm) { vm->r.h = vm->r.c; }
void ld_h_d(GB *vm) { vm->r.h = vm->r.d; }
void ld_h_e(GB *vm) { vm->r.h = vm->r.e; }
void ld_h_h(GB *vm) { vm->r.h = vm->r.h; }
void ld_h_l(GB *vm) { vm->r.h = vm->r.l; }
void ld_h_hl(GB *vm, u8 operand) { vm->r.h = read_u8(vm, vm->r.hl); }
void ld_h_a(GB *vm) { vm->r.h = vm->r.a; }
void ld_l_b(GB *vm) { vm->r.l = vm->r.b; }
void ld_l_c(GB *vm) { vm->r.l = vm->r.c; }
void ld_l_d(GB *vm) { vm->r.l = vm->r.d; }
void ld_l_e(GB *vm) { vm->r.l = vm->r.e; }
void ld_l_h(GB *vm) { vm->r.l = vm->r.h; }
void ld_l_l(GB *vm) { vm->r.l = vm->r.l; }
void ld_l_hl(GB *vm) { vm->r.l = read_u8(vm, vm->r.hl); }
void ld_l_a(GB *vm) { vm->r.l = vm->r.a; }
void ld_hl_b(GB *vm) { write_u8(vm, vm->r.hl, vm->r.b); }
void ld_hl_c(GB *vm) { write_u8(vm, vm->r.hl, vm->r.c); }
void ld_hl_d(GB *vm) { write_u8(vm, vm->r.hl, vm->r.d); }
void ld_hl_e(GB *vm) { write_u8(vm, vm->r.hl, vm->r.e); }
void ld_hl_h(GB *vm) { write_u8(vm, vm->r.hl, vm->r.h); }
void ld_hl_l(GB *vm) { write_u8(vm, vm->r.hl, vm->r.l); }
void ld_hl_a(GB *vm) { write_u8(vm, vm->r.hl, vm->r.a); }
void ld_hl_n(GB *vm, u8 operand) { write_u8(vm, vm->r.hl, operand); }

// 4. LD n, A
void ld_bc_a(GB *vm) { write_u8(vm, vm->r.bc, vm->r.a); }
void ld_de_a(GB *vm) { write_u8(vm, vm->r.de, vm->r.a); }
void ld_nn_a(GB *vm, u16 operand) { write_u8(vm, operand, vm->r.a); }

// 5. LD A,(C)
void ld_a_0xffc(GB *vm, u8 low_addr) {
  vm->r.a = read_u8(vm, 0xFF00 + vm->r.c);
}

// 6. LD (C),A
void ld_0xffc_a(GB *vm, u8 low_addr) {
  write_u8(vm, 0xFF00 + vm->r.c, vm->r.a);
}

// 7. LD A,(HLD)
void ld_a_hld(GB *vm) {
  vm->r.a = read_u8(vm, vm->r.hl);
  vm->r.hl--;
}

// 8. LD A,(HL-)
void ld_a_hlmin(GB *vm) {
  vm->r.a = read_u8(vm, vm->r.hl);
  vm->r.hl--;
}

// 9. LDD A,(HL)
void ldd_a_hl(GB *vm) {
  vm->r.a = read_u8(vm, vm->r.hl);
  vm->r.hl--;
}

// 10. LD (HLD),A
void ld_hld_a(GB *vm) {
  write_u8(vm, vm->r.hl, vm->r.a);
  vm->r.hl--;
}

// 11. LD (HL-),A
void ld_hlmin_a(GB *vm) {
  write_u8(vm, vm->r.hl, vm->r.a);
  vm->r.hl--;
}

// 12. LDD (HL),A
void ldd_hl_a(GB *vm) {
  write_u8(vm, vm->r.hl, vm->r.a);
  vm->r.hl--;
}

// 13. LD A,(HLI)
void ld_a_hli(GB *vm) {
  vm->r.a = read_u8(vm, vm->r.hl);
  vm->r.hl++;
}

// 14. LD A,(HL+)
void ld_a_hlplus(GB *vm) {
  vm->r.a = read_u8(vm, vm->r.hl);
  vm->r.hl++;
}

// 15. LDI A,(HL)
void ldi_a_hl(GB *vm) {
  vm->r.a = read_u8(vm, vm->r.hl);
  vm->r.hl++;
}

// 16. LD (HLI),A
void ld_hli_a(GB *vm) {
  write_u8(vm, vm->r.hl, vm->r.a);
  vm->r.hl++;
}

// 17. LD (HL+),A
void ld_hlplus_a(GB *vm) {
  write_u8(vm, vm->r.hl, vm->r.a);
  vm->r.hl++;
}

// 18. LDI (HL),A
void ldi_hl_a(GB *vm) {
  write_u8(vm, vm->r.hl, vm->r.a);
  vm->r.hl++;
}

// 19. LDH (n),A
void ldh_n_a(GB *vm, u8 lowaddr) { write_u8(vm, 0xFF00 + lowaddr, vm->r.a); }

// 20. LDH A,(n)
void ldh_a_n(GB *vm, u8 lowaddr) { vm->r.a = read_u8(vm, 0xFF00 + lowaddr); }

// 3.3.2 16-Bit Loads
// 1. LD n,nn
void ld_bc_nn(GB *vm, u16 operand) { vm->r.bc = operand; }
void ld_de_nn(GB *vm, u16 operand) { vm->r.de = operand; }
void ld_hl_nn(GB *vm, u16 operand) { vm->r.hl = operand; }
void ld_sp_nn(GB *vm, u16 operand) { vm->r.sp = operand; }

// 2. LD SP,HL
void ld_sp_hl(GB *vm) { vm->r.sp = vm->r.hl; }

// 3. LD HL,SP+n
void ld_hl_sp_n(GB *vm, u8 offset) {
  i16 result = vm->r.sp + (i8)offset;
  _reg_set_flag(vm, 0, 0, ((vm->r.sp & 0x0F) + (offset & 0x0F)) > 0x0F,
                ((vm->r.sp & 0xFF) + (offset & 0xFF)) > 0xFF);
  vm->r.hl = result;
}

// 4. LDHL SP,n
void ldhl_sp_n(GB *vm, u8 offset) {
  i16 result = vm->r.sp + (i8)offset;
  _reg_set_flag(vm, 0, 0, ((vm->r.sp & 0x0F) + (offset & 0x0F)) > 0x0F,
                ((vm->r.sp & 0xFF) + (offset & 0xFF)) > 0xFF);
  vm->r.hl = result;
}

// 5. LD (nn),SP
void ld_nn_sp(GB *vm, u16 address) { write_u16(vm, address, vm->r.sp); }

// 6. PUSH nn
void push_af(GB *vm) {
  vm->r.sp -= 2;
  write_u16(vm, vm->r.sp, vm->r.af);
}

void push_bc(GB *vm) {
  vm->r.sp -= 2;
  write_u16(vm, vm->r.sp, vm->r.bc);
}

void push_de(GB *vm) {
  vm->r.sp -= 2;
  write_u16(vm, vm->r.sp, vm->r.de);
}

void push_hl(GB *vm) {
  vm->r.sp -= 2;
  write_u16(vm, vm->r.sp, vm->r.hl);
}

// 7. POP nn
void pop_af(GB *vm) {
  vm->r.af = read_u16(vm, vm->r.sp);
  vm->r.sp += 2;
}

void pop_bc(GB *vm) {
  vm->r.bc = read_u16(vm, vm->r.sp);
  vm->r.sp += 2;
}

void pop_de(GB *vm) {
  vm->r.de = read_u16(vm, vm->r.sp);
  vm->r.sp += 2;
}

void pop_hl(GB *vm) {
  vm->r.hl = read_u16(vm, vm->r.sp);
  vm->r.sp += 2;
}

// 3.3.3 8-Bit ALU
// 1. ADD A,n
void add_a_a(GB *vm) {
  u16 result = vm->r.a + vm->r.a;
  _reg_set_flag(vm, (result & 0xFF) == 0, 0,
                ((vm->r.a & 0x0F) + (vm->r.a & 0x0F)) > 0x0F, result > 0xFF);
  vm->r.a = result & 0xFF;
}

void add_a_b(GB *vm) {
  u16 result = vm->r.a + vm->r.b;
  _reg_set_flag(vm, (result & 0xFF) == 0, 0,
                ((vm->r.a & 0x0F) + (vm->r.b & 0x0F)) > 0x0F, result > 0xFF);
  vm->r.a = result & 0xFF;
}

void add_a_c(GB *vm) {
  u16 result = vm->r.a + vm->r.c;
  _reg_set_flag(vm, (result & 0xFF) == 0, 0,
                ((vm->r.a & 0x0F) + (vm->r.c & 0x0F)) > 0x0F, result > 0xFF);
  vm->r.a = result & 0xFF;
}

void add_a_d(GB *vm) {
  u16 result = vm->r.a + vm->r.d;
  _reg_set_flag(vm, (result & 0xFF) == 0, 0,
                ((vm->r.a & 0x0F) + (vm->r.d & 0x0F)) > 0x0F, result > 0xFF);
  vm->r.a = result & 0xFF;
}

void add_a_e(GB *vm) {
  u16 result = vm->r.a + vm->r.e;
  _reg_set_flag(vm, (result & 0xFF) == 0, 0,
                ((vm->r.a & 0x0F) + (vm->r.e & 0x0F)) > 0x0F, result > 0xFF);
  vm->r.a = result & 0xFF;
}

void add_a_h(GB *vm) {
  u16 result = vm->r.a + vm->r.h;
  _reg_set_flag(vm, (result & 0xFF) == 0, 0,
                ((vm->r.a & 0x0F) + (vm->r.h & 0x0F)) > 0x0F, result > 0xFF);
  vm->r.a = result & 0xFF;
}

void add_a_l(GB *vm) {
  u16 result = vm->r.a + vm->r.l;
  _reg_set_flag(vm, (result & 0xFF) == 0, 0,
                ((vm->r.a & 0x0F) + (vm->r.l & 0x0F)) > 0x0F, result > 0xFF);
  vm->r.a = result & 0xFF;
}

void add_a_hl(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  u16 result = vm->r.a + value;
  _reg_set_flag(vm, (result & 0xFF) == 0, 0,
                ((vm->r.a & 0x0F) + (value & 0x0F)) > 0x0F, result > 0xFF);
  vm->r.a = result & 0xFF;
}

void add_a_n(GB *vm, u8 operand) {
  u16 result = vm->r.a + operand;
  _reg_set_flag(vm, (result & 0xFF) == 0, 0,
                ((vm->r.a & 0x0F) + (operand & 0x0F)) > 0x0F, result > 0xFF);
  vm->r.a = result & 0xFF;
}

// 2. ADC A,n
void adc_a_a(GB *vm) {
  u8 carry = (vm->r.f & 0x10) >> 4;
  u16 result = vm->r.a + vm->r.a + carry;
  _reg_set_flag(vm, (result & 0xFF) == 0, 0,
                ((vm->r.a & 0x0F) + (vm->r.a & 0x0F) + carry) > 0x0F,
                result > 0xFF);
  vm->r.a = result & 0xFF;
}

void adc_a_b(GB *vm) {
  u8 carry = (vm->r.f & 0x10) >> 4;
  u16 result = vm->r.a + vm->r.b + carry;
  _reg_set_flag(vm, (result & 0xFF) == 0, 0,
                ((vm->r.a & 0x0F) + (vm->r.b & 0x0F) + carry) > 0x0F,
                result > 0xFF);
  vm->r.a = result & 0xFF;
}

void adc_a_c(GB *vm) {
  u8 carry = (vm->r.f & 0x10) >> 4;
  u16 result = vm->r.a + vm->r.c + carry;
  _reg_set_flag(vm, (result & 0xFF) == 0, 0,
                ((vm->r.a & 0x0F) + (vm->r.c & 0x0F) + carry) > 0x0F,
                result > 0xFF);
  vm->r.a = result & 0xFF;
}

void adc_a_d(GB *vm) {
  u8 carry = (vm->r.f & 0x10) >> 4;
  u16 result = vm->r.a + vm->r.d + carry;
  _reg_set_flag(vm, (result & 0xFF) == 0, 0,
                ((vm->r.a & 0x0F) + (vm->r.d & 0x0F) + carry) > 0x0F,
                result > 0xFF);
  vm->r.a = result & 0xFF;
}

void adc_a_e(GB *vm) {
  u8 carry = (vm->r.f & 0x10) >> 4;
  u16 result = vm->r.a + vm->r.e + carry;
  _reg_set_flag(vm, (result & 0xFF) == 0, 0,
                ((vm->r.a & 0x0F) + (vm->r.e & 0x0F) + carry) > 0x0F,
                result > 0xFF);
  vm->r.a = result & 0xFF;
}

void adc_a_h(GB *vm) {
  u8 carry = (vm->r.f & 0x10) >> 4;
  u16 result = vm->r.a + vm->r.h + carry;
  _reg_set_flag(vm, (result & 0xFF) == 0, 0,
                ((vm->r.a & 0x0F) + (vm->r.h & 0x0F) + carry) > 0x0F,
                result > 0xFF);
  vm->r.a = result & 0xFF;
}

void adc_a_l(GB *vm) {
  u8 carry = (vm->r.f & 0x10) >> 4;
  u16 result = vm->r.a + vm->r.l + carry;
  _reg_set_flag(vm, (result & 0xFF) == 0, 0,
                ((vm->r.a & 0x0F) + (vm->r.l & 0x0F) + carry) > 0x0F,
                result > 0xFF);
  vm->r.a = result & 0xFF;
}

void adc_a_hl(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  u8 carry = (vm->r.f & 0x10) >> 4;
  u16 result = vm->r.a + value + carry;
  _reg_set_flag(vm, (result & 0xFF) == 0, 0,
                ((vm->r.a & 0x0F) + (value & 0x0F) + carry) > 0x0F,
                result > 0xFF);
  vm->r.a = result & 0xFF;
}

void adc_a_n(GB *vm, u8 operand) {
  u8 carry = (vm->r.f & 0x10) >> 4;
  u16 result = vm->r.a + operand + carry;
  _reg_set_flag(vm, (result & 0xFF) == 0, 0,
                ((vm->r.a & 0x0F) + (operand & 0x0F) + carry) > 0x0F,
                result > 0xFF);
  vm->r.a = result & 0xFF;
}

// 3. SUB A,n
void sub_a(GB *vm) {
  u16 result = vm->r.a - vm->r.a;
  _reg_set_flag(vm, 1, 1, 0, 0);
  vm->r.a = result & 0xFF;
}

void sub_b(GB *vm) {
  u16 result = vm->r.a - vm->r.b;
  _reg_set_flag(vm, (result & 0xFF) == 0, 1,
                (vm->r.a & 0x0F) < (vm->r.b & 0x0F), vm->r.a < vm->r.b);
  vm->r.a = result & 0xFF;
}

void sub_c(GB *vm) {
  u16 result = vm->r.a - vm->r.c;
  _reg_set_flag(vm, (result & 0xFF) == 0, 1,
                (vm->r.a & 0x0F) < (vm->r.c & 0x0F), vm->r.a < vm->r.c);
  vm->r.a = result & 0xFF;
}

void sub_d(GB *vm) {
  u16 result = vm->r.a - vm->r.d;
  _reg_set_flag(vm, (result & 0xFF) == 0, 1,
                (vm->r.a & 0x0F) < (vm->r.d & 0x0F), vm->r.a < vm->r.d);
  vm->r.a = result & 0xFF;
}

void sub_e(GB *vm) {
  u16 result = vm->r.a - vm->r.e;
  _reg_set_flag(vm, (result & 0xFF) == 0, 1,
                (vm->r.a & 0x0F) < (vm->r.e & 0x0F), vm->r.a < vm->r.e);
  vm->r.a = result & 0xFF;
}

void sub_h(GB *vm) {
  u16 result = vm->r.a - vm->r.h;
  _reg_set_flag(vm, (result & 0xFF) == 0, 1,
                (vm->r.a & 0x0F) < (vm->r.h & 0x0F), vm->r.a < vm->r.h);
  vm->r.a = result & 0xFF;
}

void sub_l(GB *vm) {
  u16 result = vm->r.a - vm->r.l;
  _reg_set_flag(vm, (result & 0xFF) == 0, 1,
                (vm->r.a & 0x0F) < (vm->r.l & 0x0F), vm->r.a < vm->r.l);
  vm->r.a = result & 0xFF;
}

void sub_hl(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  u16 result = vm->r.a - value;
  _reg_set_flag(vm, (result & 0xFF) == 0, 1, (vm->r.a & 0x0F) < (value & 0x0F),
                vm->r.a < value);
  vm->r.a = result & 0xFF;
}

void sub_n(GB *vm, u8 operand) {
  u16 result = vm->r.a - operand;
  _reg_set_flag(vm, (result & 0xFF) == 0, 1,
                (vm->r.a & 0x0F) < (operand & 0x0F), vm->r.a < operand);
  vm->r.a = result & 0xFF;
}

// 4. SBC A,n
void sbc_a_a(GB *vm, u8 operand) {
  u8 carry = (vm->r.f & 0x10) >> 4;
  u8 result = vm->r.a - vm->r.a - carry;
  _reg_set_flag(vm, result == 0, 1,
                ((vm->r.a & 0xF) - (vm->r.a & 0xF) - carry) < 0,
                (vm->r.a < vm->r.a + carry));
  vm->r.a = result;
}

void sbc_a_b(GB *vm, u8 operand) {
  u8 carry = (vm->r.f & 0x10) >> 4;
  u8 result = vm->r.a - vm->r.b - carry;
  _reg_set_flag(vm, result == 0, 1,
                ((vm->r.a & 0xF) - (vm->r.b & 0xF) - carry) < 0,
                (vm->r.a < vm->r.b + carry));
  vm->r.a = result;
}

void sbc_a_c(GB *vm, u8 operand) {
  u8 carry = (vm->r.f & 0x10) >> 4;
  u8 result = vm->r.a - vm->r.c - carry;
  _reg_set_flag(vm, result == 0, 1,
                ((vm->r.a & 0xF) - (vm->r.c & 0xF) - carry) < 0,
                (vm->r.a < vm->r.c + carry));
  vm->r.a = result;
}

void sbc_a_d(GB *vm, u8 operand) {
  u8 carry = (vm->r.f & 0x10) >> 4;
  u8 result = vm->r.a - vm->r.d - carry;
  _reg_set_flag(vm, result == 0, 1,
                ((vm->r.a & 0xF) - (vm->r.d & 0xF) - carry) < 0,
                (vm->r.a < vm->r.d + carry));
  vm->r.a = result;
}

void sbc_a_e(GB *vm, u8 operand) {
  u8 carry = (vm->r.f & 0x10) >> 4;
  u8 result = vm->r.a - vm->r.e - carry;
  _reg_set_flag(vm, result == 0, 1,
                ((vm->r.a & 0xF) - (vm->r.e & 0xF) - carry) < 0,
                (vm->r.a < vm->r.e + carry));
  vm->r.a = result;
}

void sbc_a_h(GB *vm, u8 operand) {
  u8 carry = (vm->r.f & 0x10) >> 4;
  u8 result = vm->r.a - vm->r.h - carry;
  _reg_set_flag(vm, result == 0, 1,
                ((vm->r.a & 0xF) - (vm->r.h & 0xF) - carry) < 0,
                (vm->r.a < vm->r.h + carry));
  vm->r.a = result;
}

void sbc_a_l(GB *vm, u8 operand) {
  u8 carry = (vm->r.f & 0x10) >> 4;
  u8 result = vm->r.a - vm->r.l - carry;
  _reg_set_flag(vm, result == 0, 1,
                ((vm->r.a & 0xF) - (vm->r.l & 0xF) - carry) < 0,
                (vm->r.a < vm->r.l + carry));
  vm->r.a = result;
}

void sbc_a_hl(GB *vm, u8 operand) {
  u8 carry = (vm->r.f & 0x10) >> 4;
  u8 value = read_u8(vm, vm->r.hl);
  u8 result = vm->r.a - value - carry;
  _reg_set_flag(vm, result == 0, 1,
                ((vm->r.a & 0xF) - (value & 0xF) - carry) < 0,
                (vm->r.a < value + carry));
  vm->r.a = result;
}

void sbc_a_n(GB *vm, u8 operand) {
  u8 carry = (vm->r.f & 0x10) >> 4;
  u8 result = vm->r.a - operand - carry;
  _reg_set_flag(vm, result == 0, 1,
                ((vm->r.a & 0xF) - (operand & 0xF) - carry) < 0,
                (vm->r.a < operand + carry));
  vm->r.a = result;
}

// 5. AND n
void and_a(GB *vm) {
  vm->r.a &= vm->r.a;
  _reg_set_flag(vm, vm->r.a == 0, 0, 1, 0);
}

void and_b(GB *vm) {
  vm->r.a &= vm->r.b;
  _reg_set_flag(vm, vm->r.a == 0, 0, 1, 0);
}

void and_c(GB *vm) {
  vm->r.a &= vm->r.c;
  _reg_set_flag(vm, vm->r.a == 0, 0, 1, 0);
}

void and_d(GB *vm) {
  vm->r.a &= vm->r.d;
  _reg_set_flag(vm, vm->r.a == 0, 0, 1, 0);
}

void and_e(GB *vm) {
  vm->r.a &= vm->r.e;
  _reg_set_flag(vm, vm->r.a == 0, 0, 1, 0);
}

void and_h(GB *vm) {
  vm->r.a &= vm->r.h;
  _reg_set_flag(vm, vm->r.a == 0, 0, 1, 0);
}

void and_l(GB *vm) {
  vm->r.a &= vm->r.l;
  _reg_set_flag(vm, vm->r.a == 0, 0, 1, 0);
}

void and_hl(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  vm->r.a &= value;
  _reg_set_flag(vm, vm->r.a == 0, 0, 1, 0);
}

void and_n(GB *vm, u8 operand) {
  vm->r.a &= operand;
  _reg_set_flag(vm, vm->r.a == 0, 0, 1, 0);
}
// 6. OR n
void or_a(GB *vm) {
  vm->r.a |= vm->r.a;
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, 0);
}

void or_b(GB *vm) {
  vm->r.a |= vm->r.b;
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, 0);
}

void or_c(GB *vm) {
  vm->r.a |= vm->r.c;
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, 0);
}

void or_d(GB *vm) {
  vm->r.a |= vm->r.d;
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, 0);
}

void or_e(GB *vm) {
  vm->r.a |= vm->r.e;
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, 0);
}

void or_h(GB *vm) {
  vm->r.a |= vm->r.h;
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, 0);
}

void or_l(GB *vm) {
  vm->r.a |= vm->r.l;
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, 0);
}

void or_hl(GB *vm) {
  vm->r.a |= read_u8(vm, vm->r.hl);
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, 0);
}

void or_n(GB *vm, u8 operand) {
  vm->r.a |= operand;
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, 0);
}

// 7. XOR n
void xor_a(GB *vm) {
  vm->r.a ^= vm->r.a;
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, 0);
}

void xor_b(GB *vm) {
  vm->r.a ^= vm->r.b;
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, 0);
}

void xor_c(GB *vm) {
  vm->r.a ^= vm->r.c;
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, 0);
}

void xor_d(GB *vm) {
  vm->r.a ^= vm->r.d;
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, 0);
}

void xor_e(GB *vm) {
  vm->r.a ^= vm->r.e;
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, 0);
}

void xor_h(GB *vm) {
  vm->r.a ^= vm->r.h;
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, 0);
}

void xor_l(GB *vm) {
  vm->r.a ^= vm->r.l;
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, 0);
}

void xor_hl(GB *vm) {
  vm->r.a ^= read_u8(vm, vm->r.hl);
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, 0);
}

void xor_n(GB *vm, u8 operand) {
  vm->r.a ^= operand;
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, 0);
}

// 8. CP n
void cp_a(GB *vm) {
  u8 result = vm->r.a - vm->r.a;
  _reg_set_flag(vm, 1, 1, 0, 0);
}

void cp_b(GB *vm) {
  u8 result = vm->r.a - vm->r.b;
  _reg_set_flag(vm, result == 0, 1, (vm->r.a & 0xF) < (vm->r.b & 0xF),
                vm->r.a < vm->r.b);
}

void cp_c(GB *vm) {
  u8 result = vm->r.a - vm->r.c;
  _reg_set_flag(vm, result == 0, 1, (vm->r.a & 0xF) < (vm->r.c & 0xF),
                vm->r.a < vm->r.c);
}

void cp_d(GB *vm) {
  u8 result = vm->r.a - vm->r.d;
  _reg_set_flag(vm, result == 0, 1, (vm->r.a & 0xF) < (vm->r.d & 0xF),
                vm->r.a < vm->r.d);
}

void cp_e(GB *vm) {
  u8 result = vm->r.a - vm->r.e;
  _reg_set_flag(vm, result == 0, 1, (vm->r.a & 0xF) < (vm->r.e & 0xF),
                vm->r.a < vm->r.e);
}

void cp_h(GB *vm) {
  u8 result = vm->r.a - vm->r.h;
  _reg_set_flag(vm, result == 0, 1, (vm->r.a & 0xF) < (vm->r.h & 0xF),
                vm->r.a < vm->r.h);
}

void cp_l(GB *vm) {
  u8 result = vm->r.a - vm->r.l;
  _reg_set_flag(vm, result == 0, 1, (vm->r.a & 0xF) < (vm->r.l & 0xF),
                vm->r.a < vm->r.l);
}

void cp_hl(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  u8 result = vm->r.a - value;
  _reg_set_flag(vm, result == 0, 1, (vm->r.a & 0xF) < (value & 0xF),
                vm->r.a < value);
}

void cp_n(GB *vm, u8 operand) {
  u8 result = vm->r.a - operand;
  _reg_set_flag(vm, result == 0, 1, (vm->r.a & 0xF) < (operand & 0xF),
                vm->r.a < operand);
}

// 9. INC n
void inc_a(GB *vm) {
  vm->r.a++;
  _reg_set_flag(vm, vm->r.a == 0, 0, (vm->r.a & 0x0F) == 0, vm->r.f & 0x10);
}

void inc_b(GB *vm) {
  vm->r.b++;
  _reg_set_flag(vm, vm->r.b == 0, 0, (vm->r.b & 0x0F) == 0, vm->r.f & 0x10);
}

void inc_c(GB *vm) {
  vm->r.c++;
  _reg_set_flag(vm, vm->r.c == 0, 0, (vm->r.c & 0x0F) == 0, vm->r.f & 0x10);
}

void inc_d(GB *vm) {
  vm->r.d++;
  _reg_set_flag(vm, vm->r.d == 0, 0, (vm->r.d & 0x0F) == 0, vm->r.f & 0x10);
}

void inc_e(GB *vm) {
  vm->r.e++;
  _reg_set_flag(vm, vm->r.e == 0, 0, (vm->r.e & 0x0F) == 0, vm->r.f & 0x10);
}

void inc_h(GB *vm) {
  vm->r.h++;
  _reg_set_flag(vm, vm->r.h == 0, 0, (vm->r.h & 0x0F) == 0, vm->r.f & 0x10);
}

void inc_l(GB *vm) {
  vm->r.l++;
  _reg_set_flag(vm, vm->r.l == 0, 0, (vm->r.l & 0x0F) == 0, vm->r.f & 0x10);
}

void inc_hl(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  value++;
  write_u8(vm, vm->r.hl, value);
  _reg_set_flag(vm, value == 0, 0, (value & 0x0F) == 0, vm->r.f & 0x10);
}

// 10. DEC n
void dec_a(GB *vm) {
  vm->r.a--;
  _reg_set_flag(vm, vm->r.a == 0, 1, (vm->r.a & 0x0F) == 0x0F, vm->r.f & 0x10);
}

void dec_b(GB *vm) {
  vm->r.b--;
  _reg_set_flag(vm, vm->r.b == 0, 1, (vm->r.b & 0x0F) == 0x0F, vm->r.f & 0x10);
}

void dec_c(GB *vm) {
  vm->r.c--;
  _reg_set_flag(vm, vm->r.c == 0, 1, (vm->r.c & 0x0F) == 0x0F, vm->r.f & 0x10);
}

void dec_d(GB *vm) {
  vm->r.d--;
  _reg_set_flag(vm, vm->r.d == 0, 1, (vm->r.d & 0x0F) == 0x0F, vm->r.f & 0x10);
}

void dec_e(GB *vm) {
  vm->r.e--;
  _reg_set_flag(vm, vm->r.e == 0, 1, (vm->r.e & 0x0F) == 0x0F, vm->r.f & 0x10);
}

void dec_h(GB *vm) {
  vm->r.h--;
  _reg_set_flag(vm, vm->r.h == 0, 1, (vm->r.h & 0x0F) == 0x0F, vm->r.f & 0x10);
}

void dec_l(GB *vm) {
  vm->r.l--;
  _reg_set_flag(vm, vm->r.l == 0, 1, (vm->r.l & 0x0F) == 0x0F, vm->r.f & 0x10);
}

void dec_hl(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  value--;
  write_u8(vm, vm->r.hl, value);
  _reg_set_flag(vm, value == 0, 1, (value & 0x0F) == 0x0F, vm->r.f & 0x10);
}

// 3.3.4 16-Bit Arithmetic
// 1. ADD HL,n
void add_hl_bc(GB *vm) {
  u32 result = vm->r.hl + vm->r.bc;
  _reg_set_flag(vm, vm->r.f & 0x80, 0,
                ((vm->r.hl & 0xFFF) + (vm->r.bc & 0xFFF)) > 0xFFF,
                result > 0xFFFF);
  vm->r.hl = result;
}

void add_hl_de(GB *vm) {
  u32 result = vm->r.hl + vm->r.de;
  _reg_set_flag(vm, vm->r.f & 0x80, 0,
                ((vm->r.hl & 0xFFF) + (vm->r.de & 0xFFF)) > 0xFFF,
                result > 0xFFFF);
  vm->r.hl = result;
}

void add_hl_hl(GB *vm) {
  u32 result = vm->r.hl + vm->r.hl;
  _reg_set_flag(vm, vm->r.f & 0x80, 0,
                ((vm->r.hl & 0xFFF) + (vm->r.hl & 0xFFF)) > 0xFFF,
                result > 0xFFFF);
  vm->r.hl = result;
}

void add_hl_sp(GB *vm) {
  u32 result = vm->r.hl + vm->r.sp;
  _reg_set_flag(vm, vm->r.f & 0x80, 0,
                ((vm->r.hl & 0xFFF) + (vm->r.sp & 0xFFF)) > 0xFFF,
                result > 0xFFFF);
  vm->r.hl = result;
}

// 2. ADD SP,n
void add_sp_n(GB *vm, u8 operand) {
  u32 result = vm->r.sp + (i8)operand;
  _reg_set_flag(vm, 0, 0, ((vm->r.sp & 0x0F) + (operand & 0x0F)) > 0x0F,
                ((vm->r.sp & 0xFF) + (operand & 0xFF)) > 0xFF);
  vm->r.sp = result;
}

// 3. INC nn
void inc_bc(GB *vm) { vm->r.bc++; }

void inc_de(GB *vm) { vm->r.de++; }

void inc_hl_16(GB *vm) { vm->r.hl++; }

void inc_sp(GB *vm) { vm->r.sp++; }

// 4. DEC nn
void dec_bc(GB *vm) { vm->r.bc--; }

void dec_de(GB *vm) { vm->r.de--; }

void dec_hl_16(GB *vm) { vm->r.hl--; }

void dec_sp(GB *vm) { vm->r.sp--; }

// 3.3.5 Miscellaneous
// 1. SWAP n
void swap_a(GB *vm) {
  vm->r.a = (vm->r.a << 4) | (vm->r.a >> 4);
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, 0);
}

void swap_b(GB *vm) {
  vm->r.b = (vm->r.b << 4) | (vm->r.b >> 4);
  _reg_set_flag(vm, vm->r.b == 0, 0, 0, 0);
}

void swap_c(GB *vm) {
  vm->r.c = (vm->r.c << 4) | (vm->r.c >> 4);
  _reg_set_flag(vm, vm->r.c == 0, 0, 0, 0);
}

void swap_d(GB *vm) {
  vm->r.d = (vm->r.d << 4) | (vm->r.d >> 4);
  _reg_set_flag(vm, vm->r.d == 0, 0, 0, 0);
}

void swap_e(GB *vm) {
  vm->r.e = (vm->r.e << 4) | (vm->r.e >> 4);
  _reg_set_flag(vm, vm->r.e == 0, 0, 0, 0);
}

void swap_h(GB *vm) {
  vm->r.h = (vm->r.h << 4) | (vm->r.h >> 4);
  _reg_set_flag(vm, vm->r.h == 0, 0, 0, 0);
}

void swap_l(GB *vm) {
  vm->r.l = (vm->r.l << 4) | (vm->r.l >> 4);
  _reg_set_flag(vm, vm->r.l == 0, 0, 0, 0);
}

void swap_hl(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  value = (value << 4) | (value >> 4);
  write_u8(vm, vm->r.hl, value);
  _reg_set_flag(vm, value == 0, 0, 0, 0);
}

// 2. DAA
void daa(GB *vm) {
  unsigned short s = vm->r.a;

  if (vm->r.f & 0x40) {
    if (vm->r.f & 0x20) {
      s = (s - 0x06) & 0xFF;
    }
    if (vm->r.f & 0x10) {
      s -= 0x60;
    }
  } else {
    if ((vm->r.f & 0x20) || (s & 0xF) > 9) {
      s += 0x06;
    }
    if ((vm->r.f & 0x10) || s > 0x9F) {
      s += 0x60;
    }
  }

  vm->r.a = s;
  vm->r.f &= ~0x20;

  if (vm->r.a) {
    vm->r.f &= ~0x80;
  } else {
    vm->r.f |= 0x80;
  }

  if (s >= 0x100) {
    vm->r.f |= 0x10;
  } else {
    vm->r.f &= ~0x10;
  }
}

// 3. CPL
void cpl(GB *vm) {
  vm->r.a = ~vm->r.a;
  _reg_set_flag(vm, vm->r.f & 0x80, 1, 1, vm->r.f & 0x10);
}

// 4. CCF
void ccf(GB *vm) {
  u8 carry = vm->r.f & 0x10;
  _reg_set_flag(vm, vm->r.f & 0x80, 0, 0, !carry);
}

// 5. SCF
void scf(GB *vm) { _reg_set_flag(vm, vm->r.f & 0x80, 0, 0, 1); }

// 6. NOP
void nop(GB *vm) {}

// 7. HALT
void halt(GB *vm) { vm->flag.halt = true; }

// 8. STOP
void stop(GB *vm) { vm->flag.stop = true; }

// 9. DI
void di(GB *vm) { vm->flag.interrupt_disable_pending = true; }

// 10. EI
void ei(GB *vm) { vm->flag.interrupt_enable_pending = true; }

// 3.3.6 Rotates & Shifts
// 1. RLCA
void rlca(GB *vm) {
  u8 carry = (vm->r.a & 0x80) >> 7;
  vm->r.a = (vm->r.a << 1) | carry;
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, carry);
}

// 2. RLA
void rla(GB *vm) {
  u8 carry = (vm->r.f & 0x10) >> 4;
  u8 new_carry = (vm->r.a & 0x80) >> 7;
  vm->r.a = (vm->r.a << 1) | carry;
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, new_carry);
}

// 3. RRCA
void rrca(GB *vm) {
  u8 carry = vm->r.a & 0x01;
  vm->r.a = (vm->r.a >> 1) | (carry << 7);
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, carry);
}

// 4. RRA
void rra(GB *vm) {
  u8 carry = (vm->r.f & 0x10) >> 4;
  u8 new_carry = vm->r.a & 0x01;
  vm->r.a = (vm->r.a >> 1) | (carry << 7);
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, new_carry);
}

// 5. RLC n
void rlc_a(GB *vm) {
  u8 carry = (vm->r.a & 0x80) >> 7;
  vm->r.a = (vm->r.a << 1) | carry;
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, carry);
}

void rlc_b(GB *vm) {
  u8 carry = (vm->r.b & 0x80) >> 7;
  vm->r.b = (vm->r.b << 1) | carry;
  _reg_set_flag(vm, vm->r.b == 0, 0, 0, carry);
}

void rlc_c(GB *vm) {
  u8 carry = (vm->r.c & 0x80) >> 7;
  vm->r.c = (vm->r.c << 1) | carry;
  _reg_set_flag(vm, vm->r.c == 0, 0, 0, carry);
}

void rlc_d(GB *vm) {
  u8 carry = (vm->r.d & 0x80) >> 7;
  vm->r.d = (vm->r.d << 1) | carry;
  _reg_set_flag(vm, vm->r.d == 0, 0, 0, carry);
}

void rlc_e(GB *vm) {
  u8 carry = (vm->r.e & 0x80) >> 7;
  vm->r.e = (vm->r.e << 1) | carry;
  _reg_set_flag(vm, vm->r.e == 0, 0, 0, carry);
}

void rlc_h(GB *vm) {
  u8 carry = (vm->r.h & 0x80) >> 7;
  vm->r.h = (vm->r.h << 1) | carry;
  _reg_set_flag(vm, vm->r.h == 0, 0, 0, carry);
}

void rlc_l(GB *vm) {
  u8 carry = (vm->r.l & 0x80) >> 7;
  vm->r.l = (vm->r.l << 1) | carry;
  _reg_set_flag(vm, vm->r.l == 0, 0, 0, carry);
}

void rlc_hl(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  u8 carry = (value & 0x80) >> 7;
  value = (value << 1) | carry;
  write_u8(vm, vm->r.hl, value);
  _reg_set_flag(vm, value == 0, 0, 0, carry);
}

// 6. RL n
void rl_a(GB *vm) {
  u8 carry = (vm->r.a & 0x80) >> 7;
  vm->r.a = (vm->r.a << 1) | ((vm->r.f & 0x10) >> 4);
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, carry);
}

void rl_b(GB *vm) {
  u8 carry = (vm->r.b & 0x80) >> 7;
  vm->r.b = (vm->r.b << 1) | ((vm->r.f & 0x10) >> 4);
  _reg_set_flag(vm, vm->r.b == 0, 0, 0, carry);
}

void rl_c(GB *vm) {
  u8 carry = (vm->r.c & 0x80) >> 7;
  vm->r.c = (vm->r.c << 1) | ((vm->r.f & 0x10) >> 4);
  _reg_set_flag(vm, vm->r.c == 0, 0, 0, carry);
}

void rl_d(GB *vm) {
  u8 carry = (vm->r.d & 0x80) >> 7;
  vm->r.d = (vm->r.d << 1) | ((vm->r.f & 0x10) >> 4);
  _reg_set_flag(vm, vm->r.d == 0, 0, 0, carry);
}

void rl_e(GB *vm) {
  u8 carry = (vm->r.e & 0x80) >> 7;
  vm->r.e = (vm->r.e << 1) | ((vm->r.f & 0x10) >> 4);
  _reg_set_flag(vm, vm->r.e == 0, 0, 0, carry);
}

void rl_h(GB *vm) {
  u8 carry = (vm->r.h & 0x80) >> 7;
  vm->r.h = (vm->r.h << 1) | ((vm->r.f & 0x10) >> 4);
  _reg_set_flag(vm, vm->r.h == 0, 0, 0, carry);
}

void rl_l(GB *vm) {
  u8 carry = (vm->r.l & 0x80) >> 7;
  vm->r.l = (vm->r.l << 1) | ((vm->r.f & 0x10) >> 4);
  _reg_set_flag(vm, vm->r.l == 0, 0, 0, carry);
}

void rl_hl(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  u8 carry = (value & 0x80) >> 7;
  value = (value << 1) | ((vm->r.f & 0x10) >> 4);
  write_u8(vm, vm->r.hl, value);
  _reg_set_flag(vm, value == 0, 0, 0, carry);
}

// 7. RRC n
void rrc_a(GB *vm) {
  u8 carry = vm->r.a & 0x01;
  vm->r.a = (vm->r.a >> 1) | (carry << 7);
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, carry);
}

void rrc_b(GB *vm) {
  u8 carry = vm->r.b & 0x01;
  vm->r.b = (vm->r.b >> 1) | (carry << 7);
  _reg_set_flag(vm, vm->r.b == 0, 0, 0, carry);
}

void rrc_c(GB *vm) {
  u8 carry = vm->r.c & 0x01;
  vm->r.c = (vm->r.c >> 1) | (carry << 7);
  _reg_set_flag(vm, vm->r.c == 0, 0, 0, carry);
}

void rrc_d(GB *vm) {
  u8 carry = vm->r.d & 0x01;
  vm->r.d = (vm->r.d >> 1) | (carry << 7);
  _reg_set_flag(vm, vm->r.d == 0, 0, 0, carry);
}

void rrc_e(GB *vm) {
  u8 carry = vm->r.e & 0x01;
  vm->r.e = (vm->r.e >> 1) | (carry << 7);
  _reg_set_flag(vm, vm->r.e == 0, 0, 0, carry);
}

void rrc_h(GB *vm) {
  u8 carry = vm->r.h & 0x01;
  vm->r.h = (vm->r.h >> 1) | (carry << 7);
  _reg_set_flag(vm, vm->r.h == 0, 0, 0, carry);
}

void rrc_l(GB *vm) {
  u8 carry = vm->r.l & 0x01;
  vm->r.l = (vm->r.l >> 1) | (carry << 7);
  _reg_set_flag(vm, vm->r.l == 0, 0, 0, carry);
}

void rrc_hl(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  u8 carry = value & 0x01;
  value = (value >> 1) | (carry << 7);
  write_u8(vm, vm->r.hl, value);
  _reg_set_flag(vm, value == 0, 0, 0, carry);
}

// 8. RR n
void rr_a(GB *vm) {
  u8 carry = vm->r.f & 0x10 ? 1 : 0;
  u8 old_carry = vm->r.a & 0x01;
  vm->r.a = (vm->r.a >> 1) | (carry << 7);
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, old_carry);
}

void rr_b(GB *vm) {
  u8 carry = vm->r.f & 0x10 ? 1 : 0;
  u8 old_carry = vm->r.b & 0x01;
  vm->r.b = (vm->r.b >> 1) | (carry << 7);
  _reg_set_flag(vm, vm->r.b == 0, 0, 0, old_carry);
}

void rr_c(GB *vm) {
  u8 carry = vm->r.f & 0x10 ? 1 : 0;
  u8 old_carry = vm->r.c & 0x01;
  vm->r.c = (vm->r.c >> 1) | (carry << 7);
  _reg_set_flag(vm, vm->r.c == 0, 0, 0, old_carry);
}

void rr_d(GB *vm) {
  u8 carry = vm->r.f & 0x10 ? 1 : 0;
  u8 old_carry = vm->r.d & 0x01;
  vm->r.d = (vm->r.d >> 1) | (carry << 7);
  _reg_set_flag(vm, vm->r.d == 0, 0, 0, old_carry);
}

void rr_e(GB *vm) {
  u8 carry = vm->r.f & 0x10 ? 1 : 0;
  u8 old_carry = vm->r.e & 0x01;
  vm->r.e = (vm->r.e >> 1) | (carry << 7);
  _reg_set_flag(vm, vm->r.e == 0, 0, 0, old_carry);
}

void rr_h(GB *vm) {
  u8 carry = vm->r.f & 0x10 ? 1 : 0;
  u8 old_carry = vm->r.h & 0x01;
  vm->r.h = (vm->r.h >> 1) | (carry << 7);
  _reg_set_flag(vm, vm->r.h == 0, 0, 0, old_carry);
}

void rr_l(GB *vm) {
  u8 carry = vm->r.f & 0x10 ? 1 : 0;
  u8 old_carry = vm->r.l & 0x01;
  vm->r.l = (vm->r.l >> 1) | (carry << 7);
  _reg_set_flag(vm, vm->r.l == 0, 0, 0, old_carry);
}

void rr_hl(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  u8 carry = vm->r.f & 0x10 ? 1 : 0;
  u8 old_carry = value & 0x01;
  value = (value >> 1) | (carry << 7);
  write_u8(vm, vm->r.hl, value);
  _reg_set_flag(vm, value == 0, 0, 0, old_carry);
}

// 9. SLA n
void sla_a(GB *vm) {
  u8 old_carry = vm->r.a & 0x80;
  vm->r.a <<= 1;
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, old_carry >> 7);
}

void sla_b(GB *vm) {
  u8 old_carry = vm->r.b & 0x80;
  vm->r.b <<= 1;
  _reg_set_flag(vm, vm->r.b == 0, 0, 0, old_carry >> 7);
}

void sla_c(GB *vm) {
  u8 old_carry = vm->r.c & 0x80;
  vm->r.c <<= 1;
  _reg_set_flag(vm, vm->r.c == 0, 0, 0, old_carry >> 7);
}

void sla_d(GB *vm) {
  u8 old_carry = vm->r.d & 0x80;
  vm->r.d <<= 1;
  _reg_set_flag(vm, vm->r.d == 0, 0, 0, old_carry >> 7);
}

void sla_e(GB *vm) {
  u8 old_carry = vm->r.e & 0x80;
  vm->r.e <<= 1;
  _reg_set_flag(vm, vm->r.e == 0, 0, 0, old_carry >> 7);
}

void sla_h(GB *vm) {
  u8 old_carry = vm->r.h & 0x80;
  vm->r.h <<= 1;
  _reg_set_flag(vm, vm->r.h == 0, 0, 0, old_carry >> 7);
}

void sla_l(GB *vm) {
  u8 old_carry = vm->r.l & 0x80;
  vm->r.l <<= 1;
  _reg_set_flag(vm, vm->r.l == 0, 0, 0, old_carry >> 7);
}

void sla_hl(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  u8 old_carry = value & 0x80;
  value <<= 1;
  write_u8(vm, vm->r.hl, value);
  _reg_set_flag(vm, value == 0, 0, 0, old_carry >> 7);
}

// 10. SRA n
void sra_a(GB *vm) {
  u8 old_carry = vm->r.a & 0x01;
  vm->r.a = (vm->r.a >> 1) | (vm->r.a & 0x80);
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, old_carry);
}

void sra_b(GB *vm) {
  u8 old_carry = vm->r.b & 0x01;
  vm->r.b = (vm->r.b >> 1) | (vm->r.b & 0x80);
  _reg_set_flag(vm, vm->r.b == 0, 0, 0, old_carry);
}

void sra_c(GB *vm) {
  u8 old_carry = vm->r.c & 0x01;
  vm->r.c = (vm->r.c >> 1) | (vm->r.c & 0x80);
  _reg_set_flag(vm, vm->r.c == 0, 0, 0, old_carry);
}

void sra_d(GB *vm) {
  u8 old_carry = vm->r.d & 0x01;
  vm->r.d = (vm->r.d >> 1) | (vm->r.d & 0x80);
  _reg_set_flag(vm, vm->r.d == 0, 0, 0, old_carry);
}

void sra_e(GB *vm) {
  u8 old_carry = vm->r.e & 0x01;
  vm->r.e = (vm->r.e >> 1) | (vm->r.e & 0x80);
  _reg_set_flag(vm, vm->r.e == 0, 0, 0, old_carry);
}

void sra_h(GB *vm) {
  u8 old_carry = vm->r.h & 0x01;
  vm->r.h = (vm->r.h >> 1) | (vm->r.h & 0x80);
  _reg_set_flag(vm, vm->r.h == 0, 0, 0, old_carry);
}

void sra_l(GB *vm) {
  u8 old_carry = vm->r.l & 0x01;
  vm->r.l = (vm->r.l >> 1) | (vm->r.l & 0x80);
  _reg_set_flag(vm, vm->r.l == 0, 0, 0, old_carry);
}

void sra_hl(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  u8 old_carry = value & 0x01;
  value = (value >> 1) | (value & 0x80);
  write_u8(vm, vm->r.hl, value);
  _reg_set_flag(vm, value == 0, 0, 0, old_carry);
}

// 11. SRL n
void srl_a(GB *vm) {
  u8 old_carry = vm->r.a & 0x01;
  vm->r.a >>= 1;
  _reg_set_flag(vm, vm->r.a == 0, 0, 0, old_carry);
}

void srl_b(GB *vm) {
  u8 old_carry = vm->r.b & 0x01;
  vm->r.b >>= 1;
  _reg_set_flag(vm, vm->r.b == 0, 0, 0, old_carry);
}

void srl_c(GB *vm) {
  u8 old_carry = vm->r.c & 0x01;
  vm->r.c >>= 1;
  _reg_set_flag(vm, vm->r.c == 0, 0, 0, old_carry);
}

void srl_d(GB *vm) {
  u8 old_carry = vm->r.d & 0x01;
  vm->r.d >>= 1;
  _reg_set_flag(vm, vm->r.d == 0, 0, 0, old_carry);
}

void srl_e(GB *vm) {
  u8 old_carry = vm->r.e & 0x01;
  vm->r.e >>= 1;
  _reg_set_flag(vm, vm->r.e == 0, 0, 0, old_carry);
}

void srl_h(GB *vm) {
  u8 old_carry = vm->r.h & 0x01;
  vm->r.h >>= 1;
  _reg_set_flag(vm, vm->r.h == 0, 0, 0, old_carry);
}

void srl_l(GB *vm) {
  u8 old_carry = vm->r.l & 0x01;
  vm->r.l >>= 1;
  _reg_set_flag(vm, vm->r.l == 0, 0, 0, old_carry);
}

void srl_hl(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  u8 old_carry = value & 0x01;
  value >>= 1;
  write_u8(vm, vm->r.hl, value);
  _reg_set_flag(vm, value == 0, 0, 0, old_carry);
}

// 3.3.7 Bit Opcodes
// 1. BIT b,r
void bit_0_a(GB *vm) {
  _reg_set_flag(vm, (vm->r.a & 0x01) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_1_a(GB *vm) {
  _reg_set_flag(vm, (vm->r.a & 0x02) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_2_a(GB *vm) {
  _reg_set_flag(vm, (vm->r.a & 0x04) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_3_a(GB *vm) {
  _reg_set_flag(vm, (vm->r.a & 0x08) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_4_a(GB *vm) {
  _reg_set_flag(vm, (vm->r.a & 0x10) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_5_a(GB *vm) {
  _reg_set_flag(vm, (vm->r.a & 0x20) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_6_a(GB *vm) {
  _reg_set_flag(vm, (vm->r.a & 0x40) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_7_a(GB *vm) {
  _reg_set_flag(vm, (vm->r.a & 0x80) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_0_b(GB *vm) {
  _reg_set_flag(vm, (vm->r.b & 0x01) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_1_b(GB *vm) {
  _reg_set_flag(vm, (vm->r.b & 0x02) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_2_b(GB *vm) {
  _reg_set_flag(vm, (vm->r.b & 0x04) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_3_b(GB *vm) {
  _reg_set_flag(vm, (vm->r.b & 0x08) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_4_b(GB *vm) {
  _reg_set_flag(vm, (vm->r.b & 0x10) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_5_b(GB *vm) {
  _reg_set_flag(vm, (vm->r.b & 0x20) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_6_b(GB *vm) {
  _reg_set_flag(vm, (vm->r.b & 0x40) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_7_b(GB *vm) {
  _reg_set_flag(vm, (vm->r.b & 0x80) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_0_c(GB *vm) {
  _reg_set_flag(vm, (vm->r.c & 0x01) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_1_c(GB *vm) {
  _reg_set_flag(vm, (vm->r.c & 0x02) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_2_c(GB *vm) {
  _reg_set_flag(vm, (vm->r.c & 0x04) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_3_c(GB *vm) {
  _reg_set_flag(vm, (vm->r.c & 0x08) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_4_c(GB *vm) {
  _reg_set_flag(vm, (vm->r.c & 0x10) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_5_c(GB *vm) {
  _reg_set_flag(vm, (vm->r.c & 0x20) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_6_c(GB *vm) {
  _reg_set_flag(vm, (vm->r.c & 0x40) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_7_c(GB *vm) {
  _reg_set_flag(vm, (vm->r.c & 0x80) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_0_d(GB *vm) {
  _reg_set_flag(vm, (vm->r.d & 0x01) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_1_d(GB *vm) {
  _reg_set_flag(vm, (vm->r.d & 0x02) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_2_d(GB *vm) {
  _reg_set_flag(vm, (vm->r.d & 0x04) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_3_d(GB *vm) {
  _reg_set_flag(vm, (vm->r.d & 0x08) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_4_d(GB *vm) {
  _reg_set_flag(vm, (vm->r.d & 0x10) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_5_d(GB *vm) {
  _reg_set_flag(vm, (vm->r.d & 0x20) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_6_d(GB *vm) {
  _reg_set_flag(vm, (vm->r.d & 0x40) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_7_d(GB *vm) {
  _reg_set_flag(vm, (vm->r.d & 0x80) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_0_e(GB *vm) {
  _reg_set_flag(vm, (vm->r.e & 0x01) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_1_e(GB *vm) {
  _reg_set_flag(vm, (vm->r.e & 0x02) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_2_e(GB *vm) {
  _reg_set_flag(vm, (vm->r.e & 0x04) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_3_e(GB *vm) {
  _reg_set_flag(vm, (vm->r.e & 0x08) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_4_e(GB *vm) {
  _reg_set_flag(vm, (vm->r.e & 0x10) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_5_e(GB *vm) {
  _reg_set_flag(vm, (vm->r.e & 0x20) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_6_e(GB *vm) {
  _reg_set_flag(vm, (vm->r.e & 0x40) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_7_e(GB *vm) {
  _reg_set_flag(vm, (vm->r.e & 0x80) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_0_h(GB *vm) {
  _reg_set_flag(vm, (vm->r.h & 0x01) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_1_h(GB *vm) {
  _reg_set_flag(vm, (vm->r.h & 0x02) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_2_h(GB *vm) {
  _reg_set_flag(vm, (vm->r.h & 0x04) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_3_h(GB *vm) {
  _reg_set_flag(vm, (vm->r.h & 0x08) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_4_h(GB *vm) {
  _reg_set_flag(vm, (vm->r.h & 0x10) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_5_h(GB *vm) {
  _reg_set_flag(vm, (vm->r.h & 0x20) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_6_h(GB *vm) {
  _reg_set_flag(vm, (vm->r.h & 0x40) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_7_h(GB *vm) {
  _reg_set_flag(vm, (vm->r.h & 0x80) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_0_l(GB *vm) {
  _reg_set_flag(vm, (vm->r.l & 0x01) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_1_l(GB *vm) {
  _reg_set_flag(vm, (vm->r.l & 0x02) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_2_l(GB *vm) {
  _reg_set_flag(vm, (vm->r.l & 0x04) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_3_l(GB *vm) {
  _reg_set_flag(vm, (vm->r.l & 0x08) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_4_l(GB *vm) {
  _reg_set_flag(vm, (vm->r.l & 0x10) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_5_l(GB *vm) {
  _reg_set_flag(vm, (vm->r.l & 0x20) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_6_l(GB *vm) {
  _reg_set_flag(vm, (vm->r.l & 0x40) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_7_l(GB *vm) {
  _reg_set_flag(vm, (vm->r.l & 0x80) == 0, 0, 1, (vm->r.f & 0x10) >> 4);
}

void bit_0_hlp(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  _reg_set_flag(vm, !(value & (1 << 0)), 0, 1, vm->r.f & 0x10);
}

void bit_1_hlp(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  _reg_set_flag(vm, !(value & (1 << 1)), 0, 1, vm->r.f & 0x10);
}

void bit_2_hlp(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  _reg_set_flag(vm, !(value & (1 << 2)), 0, 1, vm->r.f & 0x10);
}

void bit_3_hlp(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  _reg_set_flag(vm, !(value & (1 << 3)), 0, 1, vm->r.f & 0x10);
}

void bit_4_hlp(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  _reg_set_flag(vm, !(value & (1 << 4)), 0, 1, vm->r.f & 0x10);
}

void bit_5_hlp(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  _reg_set_flag(vm, !(value & (1 << 5)), 0, 1, vm->r.f & 0x10);
}

void bit_6_hlp(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  _reg_set_flag(vm, !(value & (1 << 6)), 0, 1, vm->r.f & 0x10);
}

void bit_7_hlp(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  _reg_set_flag(vm, !(value & (1 << 7)), 0, 1, vm->r.f & 0x10);
}

// 2. SET b,r
void set_0_a(GB *vm) { vm->r.a |= (1 << 0); }
void set_1_a(GB *vm) { vm->r.a |= (1 << 1); }
void set_2_a(GB *vm) { vm->r.a |= (1 << 2); }
void set_3_a(GB *vm) { vm->r.a |= (1 << 3); }
void set_4_a(GB *vm) { vm->r.a |= (1 << 4); }
void set_5_a(GB *vm) { vm->r.a |= (1 << 5); }
void set_6_a(GB *vm) { vm->r.a |= (1 << 6); }
void set_7_a(GB *vm) { vm->r.a |= (1 << 7); }
void set_0_b(GB *vm) { vm->r.b |= (1 << 0); }
void set_1_b(GB *vm) { vm->r.b |= (1 << 1); }
void set_2_b(GB *vm) { vm->r.b |= (1 << 2); }
void set_3_b(GB *vm) { vm->r.b |= (1 << 3); }
void set_4_b(GB *vm) { vm->r.b |= (1 << 4); }
void set_5_b(GB *vm) { vm->r.b |= (1 << 5); }
void set_6_b(GB *vm) { vm->r.b |= (1 << 6); }
void set_7_b(GB *vm) { vm->r.b |= (1 << 7); }
void set_0_c(GB *vm) { vm->r.c |= (1 << 0); }
void set_1_c(GB *vm) { vm->r.c |= (1 << 1); }
void set_2_c(GB *vm) { vm->r.c |= (1 << 2); }
void set_3_c(GB *vm) { vm->r.c |= (1 << 3); }
void set_4_c(GB *vm) { vm->r.c |= (1 << 4); }
void set_5_c(GB *vm) { vm->r.c |= (1 << 5); }
void set_6_c(GB *vm) { vm->r.c |= (1 << 6); }
void set_7_c(GB *vm) { vm->r.c |= (1 << 7); }
void set_0_d(GB *vm) { vm->r.d |= (1 << 0); }
void set_1_d(GB *vm) { vm->r.d |= (1 << 1); }
void set_2_d(GB *vm) { vm->r.d |= (1 << 2); }
void set_3_d(GB *vm) { vm->r.d |= (1 << 3); }
void set_4_d(GB *vm) { vm->r.d |= (1 << 4); }
void set_5_d(GB *vm) { vm->r.d |= (1 << 5); }
void set_6_d(GB *vm) { vm->r.d |= (1 << 6); }
void set_7_d(GB *vm) { vm->r.d |= (1 << 7); }
void set_0_e(GB *vm) { vm->r.e |= (1 << 0); }
void set_1_e(GB *vm) { vm->r.e |= (1 << 1); }
void set_2_e(GB *vm) { vm->r.e |= (1 << 2); }
void set_3_e(GB *vm) { vm->r.e |= (1 << 3); }
void set_4_e(GB *vm) { vm->r.e |= (1 << 4); }
void set_5_e(GB *vm) { vm->r.e |= (1 << 5); }
void set_6_e(GB *vm) { vm->r.e |= (1 << 6); }
void set_7_e(GB *vm) { vm->r.e |= (1 << 7); }
void set_0_h(GB *vm) { vm->r.h |= (1 << 0); }
void set_1_h(GB *vm) { vm->r.h |= (1 << 1); }
void set_2_h(GB *vm) { vm->r.h |= (1 << 2); }
void set_3_h(GB *vm) { vm->r.h |= (1 << 3); }
void set_4_h(GB *vm) { vm->r.h |= (1 << 4); }
void set_5_h(GB *vm) { vm->r.h |= (1 << 5); }
void set_6_h(GB *vm) { vm->r.h |= (1 << 6); }
void set_7_h(GB *vm) { vm->r.h |= (1 << 7); }
void set_0_l(GB *vm) { vm->r.l |= (1 << 0); }
void set_1_l(GB *vm) { vm->r.l |= (1 << 1); }
void set_2_l(GB *vm) { vm->r.l |= (1 << 2); }
void set_3_l(GB *vm) { vm->r.l |= (1 << 3); }
void set_4_l(GB *vm) { vm->r.l |= (1 << 4); }
void set_5_l(GB *vm) { vm->r.l |= (1 << 5); }
void set_6_l(GB *vm) { vm->r.l |= (1 << 6); }
void set_7_l(GB *vm) { vm->r.l |= (1 << 7); }

void set_0_hlp(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  value |= (1 << 0);
  write_u8(vm, vm->r.hl, value);
}

void set_1_hlp(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  value |= (1 << 1);
  write_u8(vm, vm->r.hl, value);
}

void set_2_hlp(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  value |= (1 << 2);
  write_u8(vm, vm->r.hl, value);
}

void set_3_hlp(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  value |= (1 << 3);
  write_u8(vm, vm->r.hl, value);
}

void set_4_hlp(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  value |= (1 << 4);
  write_u8(vm, vm->r.hl, value);
}

void set_5_hlp(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  value |= (1 << 5);
  write_u8(vm, vm->r.hl, value);
}

void set_6_hlp(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  value |= (1 << 6);
  write_u8(vm, vm->r.hl, value);
}

void set_7_hlp(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  value |= (1 << 7);
  write_u8(vm, vm->r.hl, value);
}

// 3. RES b,r
void res_0_a(GB *vm) { vm->r.a &= ~(1 << 0); }

void res_1_a(GB *vm) { vm->r.a &= ~(1 << 1); }

void res_2_a(GB *vm) { vm->r.a &= ~(1 << 2); }

void res_3_a(GB *vm) { vm->r.a &= ~(1 << 3); }

void res_4_a(GB *vm) { vm->r.a &= ~(1 << 4); }

void res_5_a(GB *vm) { vm->r.a &= ~(1 << 5); }

void res_6_a(GB *vm) { vm->r.a &= ~(1 << 6); }

void res_7_a(GB *vm) { vm->r.a &= ~(1 << 7); }

void res_0_b(GB *vm) { vm->r.b &= ~(1 << 0); }

void res_1_b(GB *vm) { vm->r.b &= ~(1 << 1); }

void res_2_b(GB *vm) { vm->r.b &= ~(1 << 2); }

void res_3_b(GB *vm) { vm->r.b &= ~(1 << 3); }

void res_4_b(GB *vm) { vm->r.b &= ~(1 << 4); }

void res_5_b(GB *vm) { vm->r.b &= ~(1 << 5); }

void res_6_b(GB *vm) { vm->r.b &= ~(1 << 6); }

void res_7_b(GB *vm) { vm->r.b &= ~(1 << 7); }

void res_0_c(GB *vm) { vm->r.c &= ~(1 << 0); }

void res_1_c(GB *vm) { vm->r.c &= ~(1 << 1); }

void res_2_c(GB *vm) { vm->r.c &= ~(1 << 2); }

void res_3_c(GB *vm) { vm->r.c &= ~(1 << 3); }

void res_4_c(GB *vm) { vm->r.c &= ~(1 << 4); }

void res_5_c(GB *vm) { vm->r.c &= ~(1 << 5); }

void res_6_c(GB *vm) { vm->r.c &= ~(1 << 6); }

void res_7_c(GB *vm) { vm->r.c &= ~(1 << 7); }

void res_0_d(GB *vm) { vm->r.d &= ~(1 << 0); }

void res_1_d(GB *vm) { vm->r.d &= ~(1 << 1); }

void res_2_d(GB *vm) { vm->r.d &= ~(1 << 2); }

void res_3_d(GB *vm) { vm->r.d &= ~(1 << 3); }

void res_4_d(GB *vm) { vm->r.d &= ~(1 << 4); }

void res_5_d(GB *vm) { vm->r.d &= ~(1 << 5); }

void res_6_d(GB *vm) { vm->r.d &= ~(1 << 6); }

void res_7_d(GB *vm) { vm->r.d &= ~(1 << 7); }

void res_0_e(GB *vm) { vm->r.e &= ~(1 << 0); }

void res_1_e(GB *vm) { vm->r.e &= ~(1 << 1); }

void res_2_e(GB *vm) { vm->r.e &= ~(1 << 2); }

void res_3_e(GB *vm) { vm->r.e &= ~(1 << 3); }

void res_4_e(GB *vm) { vm->r.e &= ~(1 << 4); }

void res_5_e(GB *vm) { vm->r.e &= ~(1 << 5); }

void res_6_e(GB *vm) { vm->r.e &= ~(1 << 6); }

void res_7_e(GB *vm) { vm->r.e &= ~(1 << 7); }

void res_0_h(GB *vm) { vm->r.h &= ~(1 << 0); }

void res_1_h(GB *vm) { vm->r.h &= ~(1 << 1); }

void res_2_h(GB *vm) { vm->r.h &= ~(1 << 2); }

void res_3_h(GB *vm) { vm->r.h &= ~(1 << 3); }

void res_4_h(GB *vm) { vm->r.h &= ~(1 << 4); }

void res_5_h(GB *vm) { vm->r.h &= ~(1 << 5); }

void res_6_h(GB *vm) { vm->r.h &= ~(1 << 6); }

void res_7_h(GB *vm) { vm->r.h &= ~(1 << 7); }

void res_0_l(GB *vm) { vm->r.l &= ~(1 << 0); }

void res_1_l(GB *vm) { vm->r.l &= ~(1 << 1); }

void res_2_l(GB *vm) { vm->r.l &= ~(1 << 2); }

void res_3_l(GB *vm) { vm->r.l &= ~(1 << 3); }

void res_4_l(GB *vm) { vm->r.l &= ~(1 << 4); }

void res_5_l(GB *vm) { vm->r.l &= ~(1 << 5); }

void res_6_l(GB *vm) { vm->r.l &= ~(1 << 6); }

void res_7_l(GB *vm) { vm->r.l &= ~(1 << 7); }

void res_0_hlp(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  value &= ~(1 << 0);
  write_u8(vm, vm->r.hl, value);
}

void res_1_hlp(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  value &= ~(1 << 1);
  write_u8(vm, vm->r.hl, value);
}

void res_2_hlp(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  value &= ~(1 << 2);
  write_u8(vm, vm->r.hl, value);
}

void res_3_hlp(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  value &= ~(1 << 3);
  write_u8(vm, vm->r.hl, value);
}

void res_4_hlp(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  value &= ~(1 << 4);
  write_u8(vm, vm->r.hl, value);
}

void res_5_hlp(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  value &= ~(1 << 5);
  write_u8(vm, vm->r.hl, value);
}

void res_6_hlp(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  value &= ~(1 << 6);
  write_u8(vm, vm->r.hl, value);
}

void res_7_hlp(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  value &= ~(1 << 7);
  write_u8(vm, vm->r.hl, value);
}

// 3.3.8 Jumps
// 1. JP nn
void jp_nn(GB *vm, u16 address) { vm->r.pc = address - 2; }

// 2. JP cc,nn
void jp_nz_nn(GB *vm, u16 address) {
  if (!(vm->r.f & 0x80)) {
    vm->r.pc = address - 2;
  }
}

void jp_z_nn(GB *vm, u16 address) {
  if (vm->r.f & 0x80) {
    vm->r.pc = address - 2;
  }
}

void jp_nc_nn(GB *vm, u16 address) {
  if (!(vm->r.f & 0x10)) {
    vm->r.pc = address - 2;
  }
}

void jp_c_nn(GB *vm, u16 address) {
  if (vm->r.f & 0x10) {
    vm->r.pc = address - 2;
  }
}

// 3. JP (HL)
void jp_hl(GB *vm) { vm->r.pc = vm->r.hl; }

// 4. JR n
void jr_n(GB *vm, u8 operand) { vm->r.pc += (i8)operand; }

// 5. JR cc,n
void jr_nz_n(GB *vm, u8 offset) {
  if (!(vm->r.f & 0x80)) {
    vm->r.pc += (i8)offset;
  }
}

void jr_z_n(GB *vm, u8 offset) {
  if (vm->r.f & 0x80) {
    vm->r.pc += (i8)offset;
  }
}

void jr_nc_n(GB *vm, u8 offset) {
  if (!(vm->r.f & 0x10)) {
    vm->r.pc += offset;
  }
}

void jr_c_n(GB *vm, u8 offset) {
  if (vm->r.f & 0x10) {
    vm->r.pc += offset;
  }
}

// 3.3.9 Calls
// 1. CALL nn
void call_nn(GB *vm, u16 address) {
  vm->r.sp -= 2;
  write_u16(vm, vm->r.sp, vm->r.pc + 3);
  vm->r.pc = address;
}

// 2. CALL cc,nn
void call_nz_nn(GB *vm, u16 address) {
  if (!(vm->r.f & 0x80)) {
    call_nn(vm, address);
  } else {
    vm->r.pc += 3;
  }
}

void call_z_nn(GB *vm, u16 address) {
  if (vm->r.f & 0x80) {
    call_nn(vm, address);
  } else {
    vm->r.pc += 3;
  }
}

void call_nc_nn(GB *vm, u16 address) {
  if (!(vm->r.f & 0x10)) {
    call_nn(vm, address);
  } else {
    vm->r.pc += 3;
  }
}

void call_c_nn(GB *vm, u16 address) {
  if (vm->r.f & 0x10) {
    call_nn(vm, address);
  } else {
    vm->r.pc += 3;
  }
}

// 3.3.10 Restarts
// 1. RST n
void rst_00(GB *vm) {
  vm->r.sp -= 2;
  write_u16(vm, vm->r.sp, vm->r.pc);
  vm->r.pc = 0x0000;
}

void rst_08(GB *vm) {
  vm->r.sp -= 2;
  write_u16(vm, vm->r.sp, vm->r.pc);
  vm->r.pc = 0x0008;
}

void rst_10(GB *vm) {
  vm->r.sp -= 2;
  write_u16(vm, vm->r.sp, vm->r.pc);
  vm->r.pc = 0x0010;
}

void rst_18(GB *vm) {
  vm->r.sp -= 2;
  write_u16(vm, vm->r.sp, vm->r.pc);
  vm->r.pc = 0x0018;
}

void rst_20(GB *vm) {
  vm->r.sp -= 2;
  write_u16(vm, vm->r.sp, vm->r.pc);
  vm->r.pc = 0x0020;
}

void rst_28(GB *vm) {
  vm->r.sp -= 2;
  write_u16(vm, vm->r.sp, vm->r.pc);
  vm->r.pc = 0x0028;
}

void rst_30(GB *vm) {
  vm->r.sp -= 2;
  write_u16(vm, vm->r.sp, vm->r.pc);
  vm->r.pc = 0x0030;
}

void rst_38(GB *vm) {
  vm->r.sp -= 2;
  write_u16(vm, vm->r.sp, vm->r.pc);
  vm->r.pc = 0x0038;
}

// 3.3.11 Returns
// 1. RET
void ret(GB *vm) {
  vm->r.pc = read_u16(vm, vm->r.sp);
  vm->r.sp += 2;
}

// 2. RET cc
void ret_nz(GB *vm) {
  if (!(vm->r.f & 0x80)) {
    vm->r.pc = read_u16(vm, vm->r.sp);
    vm->r.sp += 2;
  }
}

void ret_z(GB *vm) {
  if (vm->r.f & 0x80) {
    vm->r.pc = read_u16(vm, vm->r.sp);
    vm->r.sp += 2;
  }
}

void ret_nc(GB *vm) {
  if (!(vm->r.f & 0x10)) {
    vm->r.pc = read_u16(vm, vm->r.sp);
    vm->r.sp += 2;
  }
}

void ret_c(GB *vm) {
  if (vm->r.f & 0x10) {
    vm->r.pc = read_u16(vm, vm->r.sp);
    vm->r.sp += 2;
  }
}

// 3. RETI
void reti(GB *vm) {
  vm->r.pc = read_u16(vm, vm->r.sp);
  vm->r.sp += 2;
  vm->flag.interrupt_master_enable = true;
}
