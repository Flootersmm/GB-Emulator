#ifndef GB_H
#define GB_H

#include "tables.h"
#include "typedefs.h"
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

/// RAM and High RAM definitions
#define GB_RAM_SIZE 0x2000    /// 8 KB internal RAM
#define GB_HIGH_RAM_SIZE 0x7F /// High RAM from FF80 to FFFE
#define GB_IO_REG_SIZE 0x80   /// I/O Registers from FF00 to FF7F
#define GB_TOTAL_RAM_SIZE (GB_RAM_SIZE + GB_HIGH_RAM_SIZE + GB_IO_REG_SIZE)

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
void set_lcd_status(GB *vm);
bool is_lcd_enabled(GB *vm);
u8 read_u8(GB *vm, u16 addr);
u16 read_u16(GB *vm, u16 addr);
void write_u8(GB *vm, u16 addr, u8 value);
void write_u16(GB *vm, u16 addr, u16 value);
void do_dma_transfer(GB *vm, u8 data);
void _reg_set_flag(GB *vm, u8 z, u8 n, u8 h, u8 c);
void update_timers(GB *vm, u16 cycles);
void update_graphics(GB *vm, int cycles);
void draw_current_line(GB *vm);
void issue_vertical_blank(GB *vm);
void set_ppu_mode(GB *vm, u8 mode);
void set_clock_freq(GB *vm);
u8 get_clock_freq(GB *vm);
bool is_clock_enabled(GB *vm);
void do_divider_register(GB *vm, u16 cycles);
void do_interrupts(GB *vm);
void request_interrupt(GB *vm, u8 interrupt_flag);
void clear_interrupt(GB *vm, u8 interrupt_flag);
void service_interrupt(GB *vm, u8 interrupt);
void return_from_interrupt(GB *vm);
void draw_scanline(GB *vm);
void render_tiles(GB *vm);
void render_background(GB *vm);
COLOUR get_colour(GB *vm, u8 colour_num, u16 address);
void set_pixel(GB *vm, int x, int y, u32 color);
void render_sprites(GB *vm);
int bit_get_val(u8 byte, int bit);
bool test_bit(u8 byte, u8 bit);
u8 get_joypad_state(GB *vm);
void key_pressed(GB *vm, u8 key);
void key_released(GB *vm, int key);
void debug_tile_display(GB *vm);

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
