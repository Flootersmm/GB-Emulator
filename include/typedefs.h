#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <stdbool.h>
#include <stdint.h>

/// Cartridge type enum
typedef enum {
  ROM_ONLY = 0x00,
  ROM_MBC1 = 0x01,
  ROM_MBC1_RAM = 0x02,
  ROM_MBC1_RAM_BATT = 0x03,
  ROM_MBC2 = 0x05,
  ROM_MBC2_BATT = 0x06,
  ROM_RAM = 0x08,
  ROM_RAM_BATT = 0x09,
  ROM_MMM01 = 0x0B,
  ROM_MMM01_SRAM = 0x0C,
  ROM_MMM01_SRAM_BATT = 0x0D,
  ROM_MBC3_TIMER_BATT = 0x0F,
  ROM_MBC3_TIMER_RAM_BATT = 0x10,
  ROM_MBC3 = 0x11,
  ROM_MBC3_RAM = 0x12,
  ROM_MBC3_RAM_BATT = 0x13,
  ROM_MBC5 = 0x19,
  ROM_MBC5_RAM = 0x1A,
  ROM_MBC5_RAM_BATT = 0x1B,
  ROM_MBC5_RUMBLE = 0x1C,
  ROM_MBC5_RUMBLE_SRAM = 0x1D,
  ROM_MBC5_RUMBLE_SRAM_BATT = 0x1E,
  POCKET_CAMERA = 0x1F,
  BANDAI_TAMA5 = 0xFD,
  HUDSON_HUC3 = 0xFE,
  HUDSON_HUC1 = 0xFF
} CartridgeType;

/// Licensee map
typedef struct {
  const char *code;
  const char *publisher;
} Licensee;

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

typedef enum {
  WHITE = 0x9BBC0F,
  LIGHT_GRAY = 0x8BAC0F,
  DARK_GRAY = 0x306230,
  BLACK = 0x0F380F
} COLOUR;

/// CGB flags
typedef enum {
  NON_CGB = 0x00,
  CGB_ENHANCED = 0x80,
  CGB_ONLY = 0xC0,
} CGB;

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
  int timer_counter;
  int divider_counter;
  u8 tmc;
  u8 divider_register;
  u8 tima;
  u8 tma;
  u32 cycles;
  u16 scanline_counter;
  u8 current_scanline;
  u8 framebuffer[144][160][3];
  u8 tiles[384][8][8];
  COLOUR background_palette[4];
  u8 joypad_state;
};

#endif
