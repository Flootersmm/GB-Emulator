#ifndef GB_H
#define GB_H

#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Definitions
#define CARTRIDGE_TYPE_LIST                                                    \
  X(ROM_ONLY, 0x00, "ROM ONLY")                                                \
  X(ROM_MBC1, 0x01, "ROM MBC1")                                                \
  X(ROM_MBC1_RAM, 0x02, "ROM MBC1 + RAM")                                      \
  X(ROM_MBC1_RAM_BATT, 0x03, "ROM MBC1 + RAM + Battery")                       \
  X(ROM_MBC2, 0x05, "ROM MBC2")                                                \
  X(ROM_MBC2_BATT, 0x06, "ROM MBC2 + Battery")                                 \
  X(ROM_RAM, 0x08, "ROM + RAM")                                                \
  X(ROM_RAM_BATT, 0x09, "ROM + RAM + Battery")                                 \
  X(ROM_MMM01, 0x0B, "ROM MMM01")                                              \
  X(ROM_MMM01_SRAM, 0x0C, "ROM MMM01 + SRAM")                                  \
  X(ROM_MMM01_SRAM_BATT, 0x0D, "ROM MMM01 + SRAM + Battery")                   \
  X(ROM_MBC3_TIMER_BATT, 0x0F, "ROM MBC3 + Timer + Battery")                   \
  X(ROM_MBC3_TIMER_RAM_BATT, 0x10, "ROM MBC3 + Timer + RAM + Battery")         \
  X(ROM_MBC3, 0x11, "ROM MBC3")                                                \
  X(ROM_MBC3_RAM, 0x12, "ROM MBC3 + RAM")                                      \
  X(ROM_MBC3_RAM_BATT, 0x13, "ROM MBC3 + RAM + Battery")                       \
  X(ROM_MBC5, 0x19, "ROM MBC5")                                                \
  X(ROM_MBC5_RAM, 0x1A, "ROM MBC5 + RAM")                                      \
  X(ROM_MBC5_RAM_BATT, 0x1B, "ROM MBC5 + RAM + Battery")                       \
  X(ROM_MBC5_RUMBLE, 0x1C, "ROM MBC5 + Rumble")                                \
  X(ROM_MBC5_RUMBLE_SRAM, 0x1D, "ROM MBC5 + Rumble + SRAM")                    \
  X(ROM_MBC5_RUMBLE_SRAM_BATT, 0x1E, "ROM MBC5 + Rumble + SRAM + Battery")     \
  X(POCKET_CAMERA, 0x1F, "Pocket Camera")                                      \
  X(BANDAI_TAMA5, 0xFD, "Bandai TAMA5")                                        \
  X(HUDSON_HUC3, 0xFE, "Hudson HuC-3")                                         \
  X(HUDSON_HUC1, 0xFF, "Hudson HuC-1")

/// RAM and High RAM definitions
#define GB_RAM_SIZE 0x2000    /// 8 KB internal RAM
#define GB_HIGH_RAM_SIZE 0x7F /// High RAM from FF80 to FFFE
#define GB_IO_REG_SIZE 0x80   /// I/O Registers from FF00 to FF7F
#define GB_TOTAL_RAM_SIZE (GB_RAM_SIZE + GB_HIGH_RAM_SIZE + GB_IO_REG_SIZE)

/// Rust-like type defs
typedef uint8_t u8;
typedef int8_t i8;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint32_t u32;
typedef int32_t i32;
typedef unsigned int u32;
typedef int i32;

typedef struct GB GB;

/// Cartridge type enum
///
/// Uses a macro to populate
typedef enum {
#define X(name, value, string) name = value,
  CARTRIDGE_TYPE_LIST
#undef X
} CartridgeType;

/// CGB flags
typedef enum {
  NON_CGB = 0x00,
  CGB_ENHANCED = 0x80,
  CGB_ONLY = 0xC0,
} CGB;

/// Licensee map
typedef struct {
  const char *code;
  const char *publisher;
} Licensee;

/// Rom size triple
typedef struct {
  const char *value;
  const char *rom_size;
  const char *rom_banks;
} RomSizeEntry;

typedef struct {
  u8 code;
  char *type;
} ConsoleType;

/// Registers a, b, c, d, e, f, h, l, af, bc, de, hl, sp, pc
///
/// Uses unions and structs to allow easy access to af, bc, de, hl
typedef struct {
  struct {
    union {
      struct {
        u8 f;
        u8 a;
      };
      u16 af;
    };
  };

  struct {
    union {
      struct {
        u8 c;
        u8 b;
      };
      u16 bc;
    };
  };

  struct {
    union {
      struct {
        u8 e;
        u8 d;
      };
      u16 de;
    };
  };

  struct {
    union {
      struct {
        u8 l;
        u8 h;
      };
      u16 hl;
    };
  };

  u16 sp;
  u16 pc;
} Registers;

/// Flags, bitwise
typedef struct {
  bool logo_match;
  CGB cgb;
  bool sgb;
  bool halt;
  bool stop;
  bool interrupt_master_enable;
  bool interrupt_disable_pending;
  bool interrupt_enable_pending;
  ConsoleType type;
} Flags;

/// Memory map, with u8 items and u32 size
typedef struct {
  u8 *data;             ///> 0x0000-FFFF
  u8 *rom_bank0;        ///> 0x0000-3FFF: ROM Bank 0
  u8 *rom_bank1;        ///> 0x4000-7FFF: ROM Bank 1 (switchable)
  u8 *vram;             ///> 0x8000-9FFF: Video RAM
  u8 *external_ram;     ///> 0xA000-BFFF: External RAM (cartridge RAM)
  u8 *wram0;            ///> 0xC000-CFFF: Work RAM Bank 0
  u8 *wram1;            ///> 0xD000-DFFF: Work RAM Bank 1 (switchable)
  u8 *echo_ram;         ///> 0xE000-FDFF: Echo RAM (mirrored from C000-DDFF)
  u8 *oam;              ///> 0xFE00-FE9F: Object Attribute Memory (OAM)
  u8 *io_registers;     ///> 0xFF00-FF7F: I/O Registers
  u8 *hram;             ///> 0xFF80-FFFE: High RAM (HRAM)
  u8 *interrupt_enable; ///> 0xFFFF: Interrupt Enable Register
  u32 size;
} Memory;

/// ROM, with u8 items and u32 size
typedef struct {
  u8 *data;                     ///> Full cartridge data
  u32 size;                     ///> Size of cartridge
  u8 *entry_point;              ///> 0x0100-0103
  u8 *logo;                     ///> 0x0104-0133
  u8 *title;                    ///> 0x0134-0143
  u8 *manufacturer_code;        ///> 0x013F-0142
  u8 cgb_flag;                  ///> 0x0143
  u8 *new_licensee_code;        ///> 0x0144-0145
  u8 sgb_flag;                  ///> 0x0146
  u8 cartridge_type;            ///> 0x0147
  CartridgeType cartridge_enum; ///> 0x0147
  u8 rom_size;                  ///> 0x0148
  u8 ram_size;                  ///> 0x0149
  u8 destination_code;          ///> 0x014A
  u8 old_licensee_code;         ///> 0x014B
  u8 mask_rom_ver_number;       ///> 0x014C
  u8 header_checksum;           ///> 0x014D
  u8 *global_checksum;          ///> 0x014E-014F
} Cartridge;

/// Zero page memory, with u8 items and u32 size
typedef struct {
  u8 *item;
  u32 size;
} ZPM;

/// VRAM, with u8 items and u32 size
typedef struct {
  u8 *item;
  u32 size;
} VRAM;

typedef void (*FuncNoOp)(GB *vm);
typedef void (*FuncU8Op)(GB *vm, u8 operand);
typedef void (*FuncU16Op)(GB *vm, u16 operand);

/// Enum for function types in OPS struct
typedef enum {
  NO_OP,
  U8_OP,
  U16_OP,
} OpType;

/// Opcode, with function pointer array
typedef struct {
  const char *debug_str;
  u8 length;
  OpType type;
  union {
    FuncNoOp func_no_op;
    FuncU8Op func_u8_op;
    FuncU16Op func_u16_op;
  } func;
} OPS;

typedef struct {
  const char *debug_str;
  u8 length;
  OpType type;
  union {
    FuncNoOp func_no_op;
    FuncU8Op func_u8_op;
    FuncU16Op func_u16_op;
  } func;
} OPS_extended;

/// Main GB struct
struct GB {
  i32 dummy;
  const char *rom_path;
  Registers r;
  Memory mem;
  Flags flag;
  Cartridge cart;
  ZPM zpm;
  VRAM vram;
  OPS op;
};

extern const OPS ops[256];
extern const OPS_extended ops_extended[256];

// gb.c forward declarations
GB *gb_init(const char *rom_path);
int gb_destroy(GB *vm);
int _gb_power_on(GB *vm);
int _cart_header_read(GB *vm);
void _free_cartridge(Cartridge *cart);
const char *cart_get_type_str(CartridgeType type);
const char *licensee_get_name(GB *vm);
const char *rom_get_size_str(GB *vm);
const char *ram_get_size_str(GB *vm);
const char *destination_code_get_str(GB *vm);
u8 cart_header_checksum_calc(GB *vm);
u16 cart_global_checksum_calc(GB *vm);
bool _logo_check(GB *vm);
void _cart_header_set_flags(GB *vm);
void step(GB *vm);
u8 read_u8(GB *vm, u16 addr);
u16 read_u16(GB *vm, u16 addr);
void write_u8(GB *vm, u16 addr, u8 value);
void write_u16(GB *vm, u16 addr, u16 value);
void _reg_set_flag(GB *vm, u8 z, u8 n, u8 h, u8 c);

// ops.c forward declarations
void ld_a_n(GB *vm, u8 operand);
void ln_b_n(GB *vm, u8 operand);
void ln_c_n(GB *vm, u8 operand);
void ln_d_n(GB *vm, u8 operand);
void ln_e_n(GB *vm, u8 operand);
void ln_h_n(GB *vm, u8 operand);
void ln_l_n(GB *vm, u8 operand);
void ld_a_a(GB *vm, u8 operand);
void ld_a_b(GB *vm, u8 operand);
void ld_a_c(GB *vm, u8 operand);
void ld_a_d(GB *vm, u8 operand);
void ld_a_e(GB *vm, u8 operand);
void ld_a_h(GB *vm, u8 operand);
void ld_a_l(GB *vm, u8 operand);
void ld_a_bc(GB *vm, u8 operand);
void ld_a_de(GB *vm, u8 operand);
void ld_a_hl(GB *vm, u8 operand);
void ld_a_nn(GB *vm, u16 operand);
void ld_b_b(GB *vm, u8 operand);
void ld_b_c(GB *vm, u8 operand);
void ld_b_d(GB *vm, u8 operand);
void ld_b_e(GB *vm, u8 operand);
void ld_b_h(GB *vm, u8 operand);
void ld_b_l(GB *vm, u8 operand);
void ld_b_hl(GB *vm, u8 operand);
void ld_b_a(GB *vm, u8 operand);
void ld_c_b(GB *vm, u8 operand);
void ld_c_c(GB *vm, u8 operand);
void ld_c_d(GB *vm, u8 operand);
void ld_c_e(GB *vm, u8 operand);
void ld_c_h(GB *vm, u8 operand);
void ld_c_l(GB *vm, u8 operand);
void ld_c_hl(GB *vm, u8 operand);
void ld_c_a(GB *vm, u8 operand);
void ld_d_b(GB *vm, u8 operand);
void ld_d_c(GB *vm, u8 operand);
void ld_d_d(GB *vm, u8 operand);
void ld_d_e(GB *vm, u8 operand);
void ld_d_h(GB *vm, u8 operand);
void ld_d_l(GB *vm, u8 operand);
void ld_d_hl(GB *vm, u8 operand);
void ld_d_a(GB *vm, u8 operand);
void ld_e_b(GB *vm, u8 operand);
void ld_e_c(GB *vm, u8 operand);
void ld_e_d(GB *vm, u8 operand);
void ld_e_e(GB *vm, u8 operand);
void ld_e_h(GB *vm, u8 operand);
void ld_e_l(GB *vm, u8 operand);
void ld_e_hl(GB *vm, u8 operand);
void ld_e_a(GB *vm, u8 operand);
void ld_h_b(GB *vm, u8 operand);
void ld_h_c(GB *vm, u8 operand);
void ld_h_d(GB *vm, u8 operand);
void ld_h_e(GB *vm, u8 operand);
void ld_h_h(GB *vm, u8 operand);
void ld_h_l(GB *vm, u8 operand);
void ld_h_hl(GB *vm, u8 operand);
void ld_h_a(GB *vm, u8 operand);
void ld_l_b(GB *vm, u8 operand);
void ld_l_c(GB *vm, u8 operand);
void ld_l_d(GB *vm, u8 operand);
void ld_l_e(GB *vm, u8 operand);
void ld_l_h(GB *vm, u8 operand);
void ld_l_l(GB *vm, u8 operand);
void ld_l_hl(GB *vm, u8 operand);
void ld_l_a(GB *vm, u8 operand);
void ld_hl_b(GB *vm, u8 operand);
void ld_hl_c(GB *vm, u8 operand);
void ld_hl_d(GB *vm, u8 operand);
void ld_hl_e(GB *vm, u8 operand);
void ld_hl_h(GB *vm, u8 operand);
void ld_hl_l(GB *vm, u8 operand);
void ld_hl_a(GB *vm, u8 operand);
void ld_hl_n(GB *vm, u8 operand);
void ld_bc_a(GB *vm, u8 operand);
void ld_de_a(GB *vm, u8 operand);
void ld_nn_a(GB *vm, u16 operand);
void ld_a_0xffc(GB *vm, u8 low_addr);
void ld_0xffc_a(GB *vm, u8 low_addr);
void ld_a_hld(GB *vm);
void ld_a_hlmin(GB *vm);
void ldd_a_hl(GB *vm);
void ld_hld_a(GB *vm);
void ld_hlmin_a(GB *vm);
void ldd_hl_a(GB *vm);
void ld_a_hli(GB *vm);
void ld_a_hlplus(GB *vm);
void ldi_a_hl(GB *vm);
void ld_hli_a(GB *vm);
void ld_hlplus_a(GB *vm);
void ldi_hl_a(GB *vm);
void ldh_n_a(GB *vm, u8 lowaddr);
void ldh_a_n(GB *vm, u8 lowaddr);
void ld_bc_nn(GB *vm, u16 operand);
void ld_de_nn(GB *vm, u16 operand);
void ld_hl_nn(GB *vm, u16 operand);
void ld_sp_nn(GB *vm, u16 operand);
void ld_sp_hl(GB *vm);
void ld_hl_sp_n(GB *vm, u8 offset);
void ldhl_sp_n(GB *vm, u8 offset);
void ld_nn_sp(GB *vm, u16 address);
void push_af(GB *vm);
void push_bc(GB *vm);
void push_de(GB *vm);
void push_hl(GB *vm);
void pop_af(GB *vm);
void pop_bc(GB *vm);
void pop_de(GB *vm);
void pop_hl(GB *vm);
void add_a_a(GB *vm);
void add_a_b(GB *vm);
void add_a_c(GB *vm);
void add_a_d(GB *vm);
void add_a_e(GB *vm);
void add_a_h(GB *vm);
void add_a_l(GB *vm);
void add_a_hl(GB *vm);
void add_a_n(GB *vm, u8 operand);
void adc_a_a(GB *vm);
void adc_a_b(GB *vm);
void adc_a_c(GB *vm);
void adc_a_d(GB *vm);
void adc_a_e(GB *vm);
void adc_a_h(GB *vm);
void adc_a_l(GB *vm);
void adc_a_hl(GB *vm);
void adc_a_n(GB *vm, u8 operand);
void sub_a(GB *vm);
void sub_b(GB *vm);
void sub_c(GB *vm);
void sub_d(GB *vm);
void sub_e(GB *vm);
void sub_h(GB *vm);
void sub_l(GB *vm);
void sub_hl(GB *vm);
void sub_n(GB *vm, u8 operand);
void sbc_a_a(GB *vm, u8 operand);
void sbc_a_b(GB *vm, u8 operand);
void sbc_a_c(GB *vm, u8 operand);
void sbc_a_d(GB *vm, u8 operand);
void sbc_a_e(GB *vm, u8 operand);
void sbc_a_h(GB *vm, u8 operand);
void sbc_a_l(GB *vm, u8 operand);
void sbc_a_hl(GB *vm, u8 operand);
void sbc_a_n(GB *vm, u8 operand);
void and_a(GB *vm);
void and_b(GB *vm);
void and_c(GB *vm);
void and_d(GB *vm);
void and_e(GB *vm);
void and_h(GB *vm);
void and_l(GB *vm);
void and_hl(GB *vm);
void and_n(GB *vm, u8 operand);
void or_a(GB *vm);
void or_b(GB *vm);
void or_c(GB *vm);
void or_d(GB *vm);
void or_e(GB *vm);
void or_h(GB *vm);
void or_l(GB *vm);
void or_hl(GB *vm);
void or_n(GB *vm, u8 operand);
void xor_a(GB *vm);
void xor_b(GB *vm);
void xor_c(GB *vm);
void xor_d(GB *vm);
void xor_e(GB *vm);
void xor_h(GB *vm);
void xor_l(GB *vm);
void xor_hl(GB *vm);
void xor_n(GB *vm, u8 operand);
void cp_a(GB *vm);
void cp_b(GB *vm);
void cp_c(GB *vm);
void cp_d(GB *vm);
void cp_e(GB *vm);
void cp_h(GB *vm);
void cp_l(GB *vm);
void cp_hl(GB *vm);
void cp_n(GB *vm, u8 operand);
void inc_a(GB *vm);
void inc_b(GB *vm);
void inc_c(GB *vm);
void inc_d(GB *vm);
void inc_e(GB *vm);
void inc_h(GB *vm);
void inc_l(GB *vm);
void inc_hl(GB *vm);
void dec_a(GB *vm);
void dec_b(GB *vm);
void dec_c(GB *vm);
void dec_d(GB *vm);
void dec_e(GB *vm);
void dec_h(GB *vm);
void dec_l(GB *vm);
void dec_hl(GB *vm);
void add_hl_bc(GB *vm);
void add_hl_de(GB *vm);
void add_hl_hl(GB *vm);
void add_hl_sp(GB *vm);
void add_sp_n(GB *vm, i8 operand);
void inc_bc(GB *vm);
void inc_de(GB *vm);
void inc_hl_16(GB *vm);
void inc_sp(GB *vm);
void dec_bc(GB *vm);
void dec_de(GB *vm);
void dec_hl_16(GB *vm);
void dec_sp(GB *vm);
void swap_a(GB *vm);
void swap_b(GB *vm);
void swap_c(GB *vm);
void swap_d(GB *vm);
void swap_e(GB *vm);
void swap_h(GB *vm);
void swap_l(GB *vm);
void swap_hl(GB *vm);
void daa(GB *vm);
void cpl(GB *vm);
void ccf(GB *vm);
void scf(GB *vm);
void nop(GB *vm);
void halt(GB *vm);
void stop(GB *vm);
void di(GB *vm);
void ei(GB *vm);
void rlca(GB *vm);
void rla(GB *vm);
void rrca(GB *vm);
void rra(GB *vm);
void rlc_a(GB *vm);
void rlc_b(GB *vm);
void rlc_c(GB *vm);
void rlc_d(GB *vm);
void rlc_e(GB *vm);
void rlc_h(GB *vm);
void rlc_l(GB *vm);
void rlc_hl(GB *vm);
void rl_a(GB *vm);
void rl_b(GB *vm);
void rl_c(GB *vm);
void rl_d(GB *vm);
void rl_e(GB *vm);
void rl_h(GB *vm);
void rl_l(GB *vm);
void rl_hl(GB *vm);
void rrc_a(GB *vm);
void rrc_b(GB *vm);
void rrc_c(GB *vm);
void rrc_d(GB *vm);
void rrc_e(GB *vm);
void rrc_h(GB *vm);
void rrc_l(GB *vm);
void rrc_hl(GB *vm);
void rr_a(GB *vm);
void rr_b(GB *vm);
void rr_c(GB *vm);
void rr_d(GB *vm);
void rr_e(GB *vm);
void rr_h(GB *vm);
void rr_l(GB *vm);
void rr_hl(GB *vm);
void sla_a(GB *vm);
void sla_b(GB *vm);
void sla_c(GB *vm);
void sla_d(GB *vm);
void sla_e(GB *vm);
void sla_h(GB *vm);
void sla_l(GB *vm);
void sla_hl(GB *vm);
void sra_a(GB *vm);
void sra_b(GB *vm);
void sra_c(GB *vm);
void sra_d(GB *vm);
void sra_e(GB *vm);
void sra_h(GB *vm);
void sra_l(GB *vm);
void sra_hl(GB *vm);
void srl_a(GB *vm);
void srl_b(GB *vm);
void srl_c(GB *vm);
void srl_d(GB *vm);
void srl_e(GB *vm);
void srl_h(GB *vm);
void srl_l(GB *vm);
void srl_hl(GB *vm);
void bit_0_a(GB *vm);
void bit_1_a(GB *vm);
void bit_2_a(GB *vm);
void bit_3_a(GB *vm);
void bit_4_a(GB *vm);
void bit_5_a(GB *vm);
void bit_6_a(GB *vm);
void bit_7_a(GB *vm);
void bit_0_b(GB *vm);
void bit_1_b(GB *vm);
void bit_2_b(GB *vm);
void bit_3_b(GB *vm);
void bit_4_b(GB *vm);
void bit_5_b(GB *vm);
void bit_6_b(GB *vm);
void bit_7_b(GB *vm);
void bit_0_c(GB *vm);
void bit_1_c(GB *vm);
void bit_2_c(GB *vm);
void bit_3_c(GB *vm);
void bit_4_c(GB *vm);
void bit_5_c(GB *vm);
void bit_6_c(GB *vm);
void bit_7_c(GB *vm);
void bit_0_d(GB *vm);
void bit_1_d(GB *vm);
void bit_2_d(GB *vm);
void bit_3_d(GB *vm);
void bit_4_d(GB *vm);
void bit_5_d(GB *vm);
void bit_6_d(GB *vm);
void bit_7_d(GB *vm);
void bit_0_e(GB *vm);
void bit_1_e(GB *vm);
void bit_2_e(GB *vm);
void bit_3_e(GB *vm);
void bit_4_e(GB *vm);
void bit_5_e(GB *vm);
void bit_6_e(GB *vm);
void bit_7_e(GB *vm);
void bit_0_h(GB *vm);
void bit_1_h(GB *vm);
void bit_2_h(GB *vm);
void bit_3_h(GB *vm);
void bit_4_h(GB *vm);
void bit_5_h(GB *vm);
void bit_6_h(GB *vm);
void bit_7_h(GB *vm);
void bit_0_l(GB *vm);
void bit_1_l(GB *vm);
void bit_2_l(GB *vm);
void bit_3_l(GB *vm);
void bit_4_l(GB *vm);
void bit_5_l(GB *vm);
void bit_6_l(GB *vm);
void bit_7_l(GB *vm);
void bit_0_hlp(GB *vm);
void bit_1_hlp(GB *vm);
void bit_2_hlp(GB *vm);
void bit_3_hlp(GB *vm);
void bit_4_hlp(GB *vm);
void bit_5_hlp(GB *vm);
void bit_6_hlp(GB *vm);
void bit_7_hlp(GB *vm);
void set_0_a(GB *vm);
void set_1_a(GB *vm);
void set_2_a(GB *vm);
void set_3_a(GB *vm);
void set_4_a(GB *vm);
void set_5_a(GB *vm);
void set_6_a(GB *vm);
void set_7_a(GB *vm);
void set_0_b(GB *vm);
void set_1_b(GB *vm);
void set_2_b(GB *vm);
void set_3_b(GB *vm);
void set_4_b(GB *vm);
void set_5_b(GB *vm);
void set_6_b(GB *vm);
void set_7_b(GB *vm);
void set_0_c(GB *vm);
void set_1_c(GB *vm);
void set_2_c(GB *vm);
void set_3_c(GB *vm);
void set_4_c(GB *vm);
void set_5_c(GB *vm);
void set_6_c(GB *vm);
void set_7_c(GB *vm);
void set_0_d(GB *vm);
void set_1_d(GB *vm);
void set_2_d(GB *vm);
void set_3_d(GB *vm);
void set_4_d(GB *vm);
void set_5_d(GB *vm);
void set_6_d(GB *vm);
void set_7_d(GB *vm);
void set_0_e(GB *vm);
void set_1_e(GB *vm);
void set_2_e(GB *vm);
void set_3_e(GB *vm);
void set_4_e(GB *vm);
void set_5_e(GB *vm);
void set_6_e(GB *vm);
void set_7_e(GB *vm);
void set_0_h(GB *vm);
void set_1_h(GB *vm);
void set_2_h(GB *vm);
void set_3_h(GB *vm);
void set_4_h(GB *vm);
void set_5_h(GB *vm);
void set_6_h(GB *vm);
void set_7_h(GB *vm);
void set_0_l(GB *vm);
void set_1_l(GB *vm);
void set_2_l(GB *vm);
void set_3_l(GB *vm);
void set_4_l(GB *vm);
void set_5_l(GB *vm);
void set_6_l(GB *vm);
void set_7_l(GB *vm);
void set_0_hlp(GB *vm);
void set_1_hlp(GB *vm);
void set_2_hlp(GB *vm);
void set_3_hlp(GB *vm);
void set_4_hlp(GB *vm);
void set_5_hlp(GB *vm);
void set_6_hlp(GB *vm);
void set_7_hlp(GB *vm);
void res_0_a(GB *vm);
void res_1_a(GB *vm);
void res_2_a(GB *vm);
void res_3_a(GB *vm);
void res_4_a(GB *vm);
void res_5_a(GB *vm);
void res_6_a(GB *vm);
void res_7_a(GB *vm);
void res_0_b(GB *vm);
void res_1_b(GB *vm);
void res_2_b(GB *vm);
void res_3_b(GB *vm);
void res_4_b(GB *vm);
void res_5_b(GB *vm);
void res_6_b(GB *vm);
void res_7_b(GB *vm);
void res_0_c(GB *vm);
void res_1_c(GB *vm);
void res_2_c(GB *vm);
void res_3_c(GB *vm);
void res_4_c(GB *vm);
void res_5_c(GB *vm);
void res_6_c(GB *vm);
void res_7_c(GB *vm);
void res_0_d(GB *vm);
void res_1_d(GB *vm);
void res_2_d(GB *vm);
void res_3_d(GB *vm);
void res_4_d(GB *vm);
void res_5_d(GB *vm);
void res_6_d(GB *vm);
void res_7_d(GB *vm);
void res_0_e(GB *vm);
void res_1_e(GB *vm);
void res_2_e(GB *vm);
void res_3_e(GB *vm);
void res_4_e(GB *vm);
void res_5_e(GB *vm);
void res_6_e(GB *vm);
void res_7_e(GB *vm);
void res_0_h(GB *vm);
void res_1_h(GB *vm);
void res_2_h(GB *vm);
void res_3_h(GB *vm);
void res_4_h(GB *vm);
void res_5_h(GB *vm);
void res_6_h(GB *vm);
void res_7_h(GB *vm);
void res_0_l(GB *vm);
void res_1_l(GB *vm);
void res_2_l(GB *vm);
void res_3_l(GB *vm);
void res_4_l(GB *vm);
void res_5_l(GB *vm);
void res_6_l(GB *vm);
void res_7_l(GB *vm);
void res_0_hlp(GB *vm);
void res_1_hlp(GB *vm);
void res_2_hlp(GB *vm);
void res_3_hlp(GB *vm);
void res_4_hlp(GB *vm);
void res_5_hlp(GB *vm);
void res_6_hlp(GB *vm);
void res_7_hlp(GB *vm);
void jp_nn(GB *vm, u16 address);
void jp_nz_nn(GB *vm, u16 address);
void jp_z_nn(GB *vm, u16 address);
void jp_nc_nn(GB *vm, u16 address);
void jp_c_nn(GB *vm, u16 address);
void jp_hl(GB *vm);
void jr_n(GB *vm, u8 operand);
void jr_nz_n(GB *vm, u8 offset);
void jr_z_n(GB *vm, u8 offset);
void jr_nc_n(GB *vm, u8 offset);
void jr_c_n(GB *vm, u8 offset);
void call_nn(GB *vm, u16 address);
void call_nz_nn(GB *vm, u16 address);
void call_z_nn(GB *vm, u16 address);
void call_nc_nn(GB *vm, u16 address);
void call_c_nn(GB *vm, u16 address);
void rst_00(GB *vm);
void rst_08(GB *vm);
void rst_10(GB *vm);
void rst_18(GB *vm);
void rst_20(GB *vm);
void rst_28(GB *vm);
void rst_30(GB *vm);
void rst_38(GB *vm);
void ret(GB *vm);
void ret_nz(GB *vm);
void ret_z(GB *vm);
void ret_nc(GB *vm);
void ret_c(GB *vm);
void reti(GB *vm);

// gui.c forward declarations
GLFWwindow *window_init(void);
void main_loop(GLFWwindow *window, GB *vm);
void draw_window_crude_debug(GB *vm);
void draw_window_texture(void);
int texture_init(void);
int texture_destroy(void);

#ifdef __cplusplus
}
#endif
#endif
