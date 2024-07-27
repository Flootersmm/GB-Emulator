#ifndef GB_H
#define GB_H

#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/// Rust-like type defs
typedef uint8_t u8;
typedef int8_t i8;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint32_t u32;
typedef int32_t i32;
typedef unsigned int u32;
typedef int i32;

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
  ConsoleType type;
} Flags;

/// RAM, with u8 items and u32 size
typedef struct {
  u8 *item;
  u32 size;
} RAM;

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

/// Opcode, with function pointer array
typedef struct {
  const char *debug_str;
  u8 length;
  void (*exec)();
} OPS[256];

/// Main GB struct
typedef struct {
  i32 dummy;
  const char *rom_path;
  Registers r;
  RAM ram;
  Flags flag;
  Cartridge cart;
  ZPM zpm;
  VRAM vram;
  OPS op;
} GB;

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

// ops.c forward declarations
void nop();

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
