#include "gb.h"

// http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf
void nop(GB *vm) {}

// 3.3.1 8-Bit Loads
// 1. LD nn,n
void ln_b_n(GB *vm, u8 operand) { vm->r.b = operand; }
void ln_c_n(GB *vm, u8 operand) { vm->r.c = operand; }
void ln_d_n(GB *vm, u8 operand) { vm->r.d = operand; }
void ln_e_n(GB *vm, u8 operand) { vm->r.e = operand; }
void ln_h_n(GB *vm, u8 operand) { vm->r.h = operand; }
void ln_l_n(GB *vm, u8 operand) { vm->r.l = operand; }
void ld_a_a(GB *vm, u8 operand) { vm->r.a = vm->r.a; }

// 2. LD r1,r2
void ld_a_b(GB *vm, u8 operand) { vm->r.a = vm->r.b; }
void ld_a_c(GB *vm, u8 operand) { vm->r.a = vm->r.c; }
void ld_a_d(GB *vm, u8 operand) { vm->r.a = vm->r.d; }
void ld_a_e(GB *vm, u8 operand) { vm->r.a = vm->r.e; }
void ld_a_h(GB *vm, u8 operand) { vm->r.a = vm->r.h; }
void ld_a_l(GB *vm, u8 operand) { vm->r.a = vm->r.l; }
void ld_a_hl(GB *vm, u8 operand) { vm->r.a = read_u8(vm, vm->r.hl); }
void ld_b_b(GB *vm, u8 operand) { vm->r.b = vm->r.b; }
void ld_b_c(GB *vm, u8 operand) { vm->r.b = vm->r.c; }
void ld_b_d(GB *vm, u8 operand) { vm->r.b = vm->r.d; }
void ld_b_e(GB *vm, u8 operand) { vm->r.b = vm->r.e; }
void ld_b_h(GB *vm, u8 operand) { vm->r.b = vm->r.h; }
void ld_b_l(GB *vm, u8 operand) { vm->r.b = vm->r.l; }
void ld_b_hl(GB *vm, u8 operand) { vm->r.b = read_u8(vm, vm->r.hl); }
void ld_b_a(GB *vm, u8 operand) { vm->r.b = vm->r.a; }
void ld_c_b(GB *vm, u8 operand) { vm->r.c = vm->r.b; }
void ld_c_c(GB *vm, u8 operand) { vm->r.c = vm->r.c; }
void ld_c_d(GB *vm, u8 operand) { vm->r.c = vm->r.d; }
void ld_c_e(GB *vm, u8 operand) { vm->r.c = vm->r.e; }
void ld_c_h(GB *vm, u8 operand) { vm->r.c = vm->r.h; }
void ld_c_l(GB *vm, u8 operand) { vm->r.c = vm->r.l; }
void ld_c_hl(GB *vm, u8 operand) { vm->r.c = read_u8(vm, vm->r.hl); }
void ld_c_a(GB *vm, u8 operand) { vm->r.c = vm->r.a; }
void ld_d_b(GB *vm, u8 operand) { vm->r.d = vm->r.b; }
void ld_d_c(GB *vm, u8 operand) { vm->r.d = vm->r.c; }
void ld_d_d(GB *vm, u8 operand) { vm->r.d = vm->r.d; }
void ld_d_e(GB *vm, u8 operand) { vm->r.d = vm->r.e; }
void ld_d_h(GB *vm, u8 operand) { vm->r.d = vm->r.h; }
void ld_d_l(GB *vm, u8 operand) { vm->r.d = vm->r.l; }
void ld_d_hl(GB *vm, u8 operand) { vm->r.d = read_u8(vm, vm->r.hl); }
void ld_d_a(GB *vm, u8 operand) { vm->r.d = vm->r.a; }
void ld_e_b(GB *vm, u8 operand) { vm->r.e = vm->r.b; }
void ld_e_c(GB *vm, u8 operand) { vm->r.e = vm->r.c; }
void ld_e_d(GB *vm, u8 operand) { vm->r.e = vm->r.d; }
void ld_e_e(GB *vm, u8 operand) { vm->r.e = vm->r.e; }
void ld_e_h(GB *vm, u8 operand) { vm->r.e = vm->r.h; }
void ld_e_l(GB *vm, u8 operand) { vm->r.e = vm->r.l; }
void ld_e_hl(GB *vm, u8 operand) { vm->r.e = read_u8(vm, vm->r.hl); }
void ld_e_a(GB *vm, u8 operand) { vm->r.e = vm->r.a; }
void ld_h_b(GB *vm, u8 operand) { vm->r.h = vm->r.b; }
void ld_h_c(GB *vm, u8 operand) { vm->r.h = vm->r.c; }
void ld_h_d(GB *vm, u8 operand) { vm->r.h = vm->r.d; }
void ld_h_e(GB *vm, u8 operand) { vm->r.h = vm->r.e; }
void ld_h_h(GB *vm, u8 operand) { vm->r.h = vm->r.h; }
void ld_h_l(GB *vm, u8 operand) { vm->r.h = vm->r.l; }
void ld_h_hl(GB *vm, u8 operand) { vm->r.h = read_u8(vm, vm->r.hl); }
void ld_h_a(GB *vm, u8 operand) { vm->r.h = vm->r.a; }
void ld_l_b(GB *vm, u8 operand) { vm->r.l = vm->r.b; }
void ld_l_c(GB *vm, u8 operand) { vm->r.l = vm->r.c; }
void ld_l_d(GB *vm, u8 operand) { vm->r.l = vm->r.d; }
void ld_l_e(GB *vm, u8 operand) { vm->r.l = vm->r.e; }
void ld_l_h(GB *vm, u8 operand) { vm->r.l = vm->r.h; }
void ld_l_l(GB *vm, u8 operand) { vm->r.l = vm->r.l; }
void ld_l_hl(GB *vm, u8 operand) { vm->r.l = read_u8(vm, vm->r.hl); }
void ld_l_a(GB *vm, u8 operand) { vm->r.l = vm->r.a; }
void ld_hl_b(GB *vm, u8 operand) { write_u8(vm, vm->r.hl, vm->r.b); }
void ld_hl_c(GB *vm, u8 operand) { write_u8(vm, vm->r.hl, vm->r.c); }
void ld_hl_d(GB *vm, u8 operand) { write_u8(vm, vm->r.hl, vm->r.d); }
void ld_hl_e(GB *vm, u8 operand) { write_u8(vm, vm->r.hl, vm->r.e); }
void ld_hl_h(GB *vm, u8 operand) { write_u8(vm, vm->r.hl, vm->r.h); }
void ld_hl_l(GB *vm, u8 operand) { write_u8(vm, vm->r.hl, vm->r.l); }
void ld_hl_a(GB *vm, u8 operand) { write_u8(vm, vm->r.hl, vm->r.a); }
void ld_hl_n(GB *vm, u8 operand) { write_u8(vm, vm->r.hl, operand); }

// 4. LD n, A
void ld_bc_a(GB *vm, u8 operand) { write_u8(vm, vm->r.bc, vm->r.a); }
void ld_de_a(GB *vm, u8 operand) { write_u8(vm, vm->r.de, vm->r.a); }
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

// ADC A,n
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
void sub_a_a(GB *vm) {
  u16 result = vm->r.a - vm->r.a;
  _reg_set_flag(vm, 1, 1, 0, 0);
  vm->r.a = result & 0xFF;
}

void sub_a_b(GB *vm) {
  u16 result = vm->r.a - vm->r.b;
  _reg_set_flag(vm, (result & 0xFF) == 0, 1,
                (vm->r.a & 0x0F) < (vm->r.b & 0x0F), vm->r.a < vm->r.b);
  vm->r.a = result & 0xFF;
}

void sub_a_c(GB *vm) {
  u16 result = vm->r.a - vm->r.c;
  _reg_set_flag(vm, (result & 0xFF) == 0, 1,
                (vm->r.a & 0x0F) < (vm->r.c & 0x0F), vm->r.a < vm->r.c);
  vm->r.a = result & 0xFF;
}

void sub_a_d(GB *vm) {
  u16 result = vm->r.a - vm->r.d;
  _reg_set_flag(vm, (result & 0xFF) == 0, 1,
                (vm->r.a & 0x0F) < (vm->r.d & 0x0F), vm->r.a < vm->r.d);
  vm->r.a = result & 0xFF;
}

void sub_a_e(GB *vm) {
  u16 result = vm->r.a - vm->r.e;
  _reg_set_flag(vm, (result & 0xFF) == 0, 1,
                (vm->r.a & 0x0F) < (vm->r.e & 0x0F), vm->r.a < vm->r.e);
  vm->r.a = result & 0xFF;
}

void sub_a_h(GB *vm) {
  u16 result = vm->r.a - vm->r.h;
  _reg_set_flag(vm, (result & 0xFF) == 0, 1,
                (vm->r.a & 0x0F) < (vm->r.h & 0x0F), vm->r.a < vm->r.h);
  vm->r.a = result & 0xFF;
}

void sub_a_l(GB *vm) {
  u16 result = vm->r.a - vm->r.l;
  _reg_set_flag(vm, (result & 0xFF) == 0, 1,
                (vm->r.a & 0x0F) < (vm->r.l & 0x0F), vm->r.a < vm->r.l);
  vm->r.a = result & 0xFF;
}

void sub_a_hl(GB *vm) {
  u8 value = read_u8(vm, vm->r.hl);
  u16 result = vm->r.a - value;
  _reg_set_flag(vm, (result & 0xFF) == 0, 1, (vm->r.a & 0x0F) < (value & 0x0F),
                vm->r.a < value);
  vm->r.a = result & 0xFF;
}

void sub_a_n(GB *vm, u8 operand) {
  u16 result = vm->r.a - operand;
  _reg_set_flag(vm, (result & 0xFF) == 0, 1,
                (vm->r.a & 0x0F) < (operand & 0x0F), vm->r.a < operand);
  vm->r.a = result & 0xFF;
}

void jp_nn(GB *vm, u16 operand) {
  // TODO: grab operand with an external function actually
  operand = (vm->cart.data[vm->r.pc]) | (vm->cart.data[vm->r.pc + 0x01] << 8);
  vm->r.pc = operand;
}

void inc_a(GB *vm, u8 operand) {
  vm->r.a++;
  _reg_set_flag(vm, vm->r.a == 0, 0, (vm->r.a & 0x0F) == 0, vm->r.f & 0x10);
}

void inc_b(GB *vm, u8 operand) {
  vm->r.b++;
  _reg_set_flag(vm, vm->r.b == 0, 0, (vm->r.b & 0x0F) == 0, vm->r.f & 0x10);
}

void inc_c(GB *vm, u8 operand) {
  vm->r.c++;
  _reg_set_flag(vm, vm->r.c == 0, 0, (vm->r.c & 0x0F) == 0, vm->r.f & 0x10);
}

void inc_d(GB *vm, u8 operand) {
  vm->r.d++;
  _reg_set_flag(vm, vm->r.d == 0, 0, (vm->r.d & 0x0F) == 0, vm->r.f & 0x10);
}

void inc_e(GB *vm, u8 operand) {
  vm->r.e++;
  _reg_set_flag(vm, vm->r.e == 0, 0, (vm->r.e & 0x0F) == 0, vm->r.f & 0x10);
}

void inc_h(GB *vm, u8 operand) {
  vm->r.h++;
  _reg_set_flag(vm, vm->r.h == 0, 0, (vm->r.h & 0x0F) == 0, vm->r.f & 0x10);
}

void inc_l(GB *vm, u8 operand) {
  vm->r.l++;
  _reg_set_flag(vm, vm->r.l == 0, 0, (vm->r.l & 0x0F) == 0, vm->r.f & 0x10);
}

void inc_hl(GB *vm, u8 operand) {
  u16 address = (vm->r.h << 8) | vm->r.l;
  vm->mem.data[address]++;
  _reg_set_flag(vm, vm->mem.data[address] == 0, 0,
                (vm->mem.data[address] & 0x0F) == 0, vm->r.f & 0x10);
}

void dec_a(GB *vm, u8 operand) {
  vm->r.a--;
  _reg_set_flag(vm, vm->r.a == 0, 1, (vm->r.a & 0x0F) == 0, vm->r.f & 0x10);
}

void dec_b(GB *vm, u8 operand) {
  vm->r.b--;
  _reg_set_flag(vm, vm->r.b == 0, 1, (vm->r.b & 0x0F) == 0, vm->r.f & 0x10);
}

void dec_c(GB *vm, u8 operand) {
  vm->r.c--;
  _reg_set_flag(vm, vm->r.c == 0, 1, (vm->r.c & 0x0F) == 0, vm->r.f & 0x10);
}

void dec_d(GB *vm, u8 operand) {
  vm->r.d--;
  _reg_set_flag(vm, vm->r.d == 0, 1, (vm->r.d & 0x0F) == 0, vm->r.f & 0x10);
}

void dec_e(GB *vm, u8 operand) {
  vm->r.e--;
  _reg_set_flag(vm, vm->r.e == 0, 1, (vm->r.e & 0x0F) == 0, vm->r.f & 0x10);
}

void dec_h(GB *vm, u8 operand) {
  vm->r.h--;
  _reg_set_flag(vm, vm->r.h == 0, 1, (vm->r.h & 0x0F) == 0, vm->r.f & 0x10);
}

void dec_l(GB *vm, u8 operand) {
  vm->r.l--;
  _reg_set_flag(vm, vm->r.l == 0, 1, (vm->r.l & 0x0F) == 0, vm->r.f & 0x10);
}

void dec_hl(GB *vm, u8 operand) {
  u8 value = read_u8(vm, vm->r.hl);
  value--;
  write_u8(vm, vm->r.hl, value);
  _reg_set_flag(vm, value == 0, 1, (value & 0x0F) == 0, vm->r.f & 0x10);
}
