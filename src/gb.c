#include "gb.h"
#include "ops.h"
#include "tables.h"
#include "typedefs.h"

/// Gameboy initialisation
///
/// Malloc space for the vm, set registers to appropriate values, clears ram,
/// loads the cart, interprets the header, and sets appropriate flags.
///
/// @param rom_path Path to the rom to be played
/// @return A GB vm instanctep
GB *gb_init(const char *rom_path) {
  GB *vm = (GB *)malloc(sizeof(GB));
  *vm = (GB){
      .dummy = 1,
      .rom_path = rom_path,
      .r = {.h = 0,
            .a = 0,
            .b = 0,
            .c = 0,
            .d = 0,
            .e = 0,
            .f = 0,
            .pc = 0,
            .sp = 0},
      .mem =
          {
              .data = malloc(0x10000),
              .size = 0x10000,
          },
      .flag = {0},
      .cart =
          {
              .data = NULL,
              .size = 255,
          },
      .vram =
          {
              .item = NULL,
              .size = 255,
          },
      .timer_counter = 0,
      .divider_counter = 0,
      .tmc = 0,
      .divider_register = 0,
      .clock_frequency = 1024,
      .tima = 0,
      .tma = 0,
      .paused = false,
  };

  if (!vm->mem.data) {
    fprintf(stderr, "Error allocating memory for GB RAM\n");
    free(vm);
    return NULL;
  }

  vm->mem.boot_rom = vm->mem.data + 0x0000;
  vm->mem.rom_bank0 = vm->mem.data + 0x0000;
  vm->mem.rom_bank1 = vm->mem.data + 0x4000;
  vm->mem.vram = vm->mem.data + 0x8000;
  vm->mem.external_ram = vm->mem.data + 0xA000;
  vm->mem.wram0 = vm->mem.data + 0xC000;
  vm->mem.wram1 = vm->mem.data + 0xD000;
  vm->mem.echo_ram = vm->mem.data + 0xE000;
  vm->mem.oam = vm->mem.data + 0xFE00;
  vm->mem.hram = vm->mem.data + 0xFF80;
  vm->mem.interrupt_enable = vm->mem.data + 0xFFFF;

  memset(vm->mem.data, 0x00, 0x10000);

  FILE *fp = fopen(rom_path, "rb");
  if (fp == NULL) {
    fprintf(stderr, "Error opening rom at %s\n", rom_path);
    fclose(fp);
    free(vm);
    return NULL;
  }

  fseek(fp, 0, SEEK_END);
  vm->cart.size = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  vm->cart.data = malloc(vm->cart.size);
  if (!vm->cart.data) {
    fprintf(stderr, "Error allocating memory for ROM\n");
    fclose(fp);
    return NULL;
  }

  fread(vm->cart.data, vm->cart.size, sizeof(u8), fp);

  FILE *boot_fp = fopen("./roms/dmg_boot.bin", "rb");
  if (boot_fp == NULL) {
    fprintf(stderr, "Error opening boot ROM at ./roms/dmg_boot.bin\n");
    free(vm->cart.data);
    free(vm);
    return NULL;
  }

  vm->mem.boot_rom = malloc(0x0100);

  fread(vm->mem.boot_rom, 0x0100, sizeof(u8), boot_fp);
  fclose(boot_fp);

  cart_header_read(vm);
  logo_check(vm);
  if ((cart_header_checksum_calc(vm)) != vm->cart.header_checksum) {
    return NULL;
  };
  cart_header_set_flags(vm);
  if (!vm->flag.logo_match) {
    return NULL;
  };

  gb_power_on(vm);

  return vm;
}

/// Clean up the GB vm
///
/// @param vm GB *vm
///
/// @return 0 for success, -1 for failure
int gb_destroy(GB *vm) { return 0; }

/// Initialise the power on sequence
///
/// @param vm GB *vm
///
/// @return 0 for success, -1 for failure
int gb_power_on(GB *vm) {
  // To do boot rom, start with pc at 0x0000, then 'the boot ROM writes to the
  // BANK register at $FF50, which unmaps the boot ROM. The ldh [$FF50], a
  // instruction being located at $00FE (and being two bytes long), the first
  // instruction executed from the cartridge ROM is at $0100.'
  //
  // So, we need memory banks up and running first.

  memcpy(vm->mem.data, vm->cart.data, vm->cart.size);
  memcpy(vm->mem.data, vm->mem.boot_rom, 0x0100);

  vm->mem.data[0xFF50] = 1; // This unmaps the boot ROM, if I had that set up

  vm->flag.halt = false;
  vm->flag.stop = false;
  vm->flag.interrupt_master_enable = false;
  vm->flag.interrupt_disable_handling = false;
  vm->flag.interrupt_enable_handling = false;

  // TODO: fix this
  vm->flag.type.code = 0x00;
  vm->flag.type.type = "GB";

  // Make sure to do this for all types like MGB
  // GB, GB Pocket:
  if (vm->flag.type.code == 0x00) {
    u16 cart_byte_total = 25; // 25 dec added to total
    for (u16 i = 0x134; i <= 0x14d; i++) {
      cart_byte_total += vm->cart.data[i];
    }
    if ((cart_byte_total & 1) != 0) { // If LSB != 0, halt
      vm->flag.halt = true;
      fprintf(stderr, "cart_byte_total lsb != 0\n");
      return -1;
    }
  }
  // TODO: Super GB:
  // don't check

  // DMG
  vm->r.h = 0x01;
  vm->r.l = 0x4D;
  vm->r.a = 0x01;
  vm->r.b = 0x00;
  vm->r.c = 0x13;
  vm->r.d = 0x00;
  vm->r.e = 0xD8;
  if (vm->cart.header_checksum == 0) {
    reg_set_flag(vm, 1, 0, 0, 0);
  } else {
    reg_set_flag(vm, 1, 0, 1, 1);
  }
  vm->r.pc = 0x0000; // 100 to skip boot rom
  vm->r.sp = 0xFFFE;

  // DMG
  vm->mem.data[0xFF00] = 0xCF; // P1
  vm->mem.data[0xFF01] = 0x00; // SB
  vm->mem.data[0xFF02] = 0x7E; // SC
  vm->mem.data[0xFF04] = 0xAB; // DIV
  vm->mem.data[0xFF05] = 0x00; // TIMA
  vm->mem.data[0xFF06] = 0x00; // TMA
  vm->mem.data[0xFF07] = 0xF8; // TAC
  vm->mem.data[0xFF0F] = 0xE1; // IF
  vm->mem.data[0xFF10] = 0x80; // NR10
  vm->mem.data[0xFF11] = 0xBF; // NR11
  vm->mem.data[0xFF12] = 0xF3; // NR12
  vm->mem.data[0xFF13] = 0xFF; // NR13
  vm->mem.data[0xFF14] = 0xBF; // NR14
  vm->mem.data[0xFF16] = 0x3F; // NR21
  vm->mem.data[0xFF17] = 0x00; // NR22
  vm->mem.data[0xFF18] = 0xFF; // NR23
  vm->mem.data[0xFF19] = 0xBF; // NR24
  vm->mem.data[0xFF1A] = 0x7F; // NR30
  vm->mem.data[0xFF1B] = 0xFF; // NR31
  vm->mem.data[0xFF1C] = 0x9F; // NR32
  vm->mem.data[0xFF1E] = 0xBF; // NR33
  vm->mem.data[0xFF20] = 0xFF; // NR41
  vm->mem.data[0xFF21] = 0x00; // NR42
  vm->mem.data[0xFF22] = 0x00; // NR43
  vm->mem.data[0xFF23] = 0xBF; // NR30
  vm->mem.data[0xFF24] = 0x77; // NR50
  vm->mem.data[0xFF25] = 0xF3; // NR51
  vm->mem.data[0xFF26] = 0xF1; // NR52
  vm->mem.data[0xFF40] = 0x91; // LCDC
  vm->mem.data[0xFF41] = 0x85; // STAT
  vm->mem.data[0xFF42] = 0x00; // SCY
  vm->mem.data[0xFF43] = 0x00; // SCX
  vm->mem.data[0xFF44] = 0x00; // LY
  vm->mem.data[0xFF45] = 0x00; // LYC
  vm->mem.data[0xFF46] = 0xFF; // DMA
  vm->mem.data[0xFF47] = 0xFC; // BGP
  vm->mem.data[0xFF48] = 0xFF; // OBP0
  vm->mem.data[0xFF49] = 0xFF; // OBP1
  vm->mem.data[0xFF4A] = 0x00; // WY
  vm->mem.data[0xFF4B] = 0x00; // WX
  vm->mem.data[0xFFFF] = 0x00; // IE

  // read logo from the header, unpack it into VRAM, slowly scroll it down.
  // Once finished scrolling, play sound, read logo again, compare to copy,
  // compute header checksum.
  return 0;
}

/// Separates the cart header into distinct fields
///
/// @param vm GB *vm
///
/// @return 0 for success, -1 for failure
int cart_header_read(GB *vm) {
  vm->cart.entry_point = vm->cart.data + 0x0100;
  vm->cart.logo = vm->cart.data + 0x0104;
  vm->cart.title = vm->cart.data + 0x0134;
  vm->cart.manufacturer_code = vm->cart.data + 0x013F;
  vm->cart.cgb_flag = vm->cart.data[0x0143];
  vm->cart.new_licensee_code = vm->cart.data + 0x0144;
  vm->cart.sgb_flag = vm->cart.data[0x0146];
  vm->cart.cartridge_type = vm->cart.data[0x0147];
  vm->cart.rom_size = vm->cart.data[0x0148];
  vm->cart.ram_size = vm->cart.data[0x0149];
  vm->cart.destination_code = vm->cart.data[0x014A];
  vm->cart.old_licensee_code = vm->cart.data[0x014B];
  vm->cart.mask_rom_ver_number = vm->cart.data[0x014C];
  vm->cart.header_checksum = vm->cart.data[0x014D];
  vm->cart.global_checksum = vm->cart.data + 0x014E;

  return 0;
}

/// Free the cart memory
void free_cartridge(Cartridge *cart) {}

/// Convert cartridge type from hex to string
const char *cart_get_type_str(CartridgeType type) {
  if (type <= 0xFF && cartridgeTypeStrings[type] != NULL) {
    return cartridgeTypeStrings[type];
  } else {
    return "Unknown Cartridge Type";
  }
}

/// Convert licensee name from hex to string
///
/// @param vm GB *vm
///
/// @return 0 for success, -1 for failure
const char *licensee_get_name(GB *vm) {
  if (vm->cart.old_licensee_code == 0x33) {
    char code_str[3];
    snprintf(code_str, sizeof(code_str), "%2s", vm->cart.new_licensee_code);

    for (int i = 0; i < new_licensee_table_size; i++) {
      if (strcmp(new_licensees[i].code, code_str) == 0) {
        return new_licensees[i].publisher;
      }
    }
  } else {
    char code_str[3];
    snprintf(code_str, sizeof(code_str), "%02X", vm->cart.old_licensee_code);

    for (int i = 0; i < old_licensee_table_size; i++) {
      if (strcmp(old_licensees[i].code, code_str) == 0) {
        return old_licensees[i].publisher;
      }
    }
  }
  return "Unknown Publisher";
}

/// Convert ROM size into string
///
/// String in the form:
/// value = "$00";
/// rom_size = "32 KiB";
/// rom_banks = "2 (no banking)";
///
/// @param vm GB *vm
///
/// @return Rom size string
const char *rom_get_size_str(GB *vm) {
  static char buffer[128];
  const char *value = NULL;
  const char *rom_size = NULL;
  const char *rom_banks = NULL;

  switch (vm->cart.rom_size) {
  case 0x00:
    value = "$00";
    rom_size = "32 KiB";
    rom_banks = "2 (no banking)";
    break;
  case 0x01:
    value = "$01";
    rom_size = "64 KiB";
    rom_banks = "4";
    break;
  case 0x02:
    value = "$02";
    rom_size = "128 KiB";
    rom_banks = "8";
    break;
  case 0x03:
    value = "$03";
    rom_size = "256 KiB";
    rom_banks = "16";
    break;
  case 0x04:
    value = "$04";
    rom_size = "512 KiB";
    rom_banks = "32";
    break;
  case 0x05:
    value = "$05";
    rom_size = "1 MiB";
    rom_banks = "64";
    break;
  case 0x06:
    value = "$06";
    rom_size = "2 MiB";
    rom_banks = "128";
    break;
  case 0x07:
    value = "$07";
    rom_size = "4 MiB";
    rom_banks = "256";
    break;
  case 0x08:
    value = "$08";
    rom_size = "8 MiB";
    rom_banks = "512";
    break;
  case 0x52:
    value = "$52";
    rom_size = "1.1 MiB !!";
    rom_banks = "72";
    break;
  case 0x53:
    value = "$53";
    rom_size = "1.2 MiB !!";
    rom_banks = "80";
    break;
  case 0x54:
    value = "$54";
    rom_size = "1.5 MiB !!";
    rom_banks = "96";
    break;
  default:
    value = "Unknown";
    rom_size = "Unknown";
    rom_banks = "Unknown";
    break;
  }

  snprintf(buffer, sizeof(buffer), "%s, Banks: %s, (ID: %s)", rom_size,
           rom_banks, value);
  return buffer;
}

/// Convert RAM size into string
///
/// String in the form:
/// code = "$02";
/// sram_size = "8 KiB";
/// sram_banks = "1 bank";
///
/// @param vm GB *vm
///
/// @return RAM size string
const char *ram_get_size_str(GB *vm) {
  static char buffer[128];
  const char *code = NULL;
  const char *sram_size = NULL;
  const char *comment = NULL;

  switch (vm->cart.ram_size) {
  case 0x00:
    code = "$00";
    sram_size = "0 KiB";
    comment = "None";
    break;
  case 0x01:
    code = "$01";
    sram_size = "-";
    comment = "Unused";
    break;
  case 0x02:
    code = "$02";
    sram_size = "8 KiB";
    comment = "1 bank";
    break;
  case 0x03:
    code = "$03";
    sram_size = "32 KiB";
    comment = "4 banks of 8 KiB each";
    break;
  case 0x04:
    code = "$04";
    sram_size = "128 KiB";
    comment = "16 banks of 8 KiB each";
    break;
  case 0x05:
    code = "$05";
    sram_size = "64 KiB";
    comment = "8 banks of 8 KiB each";
    break;
  default:
    code = "Unknown";
    sram_size = "Unknown";
    comment = "Unknown";
    break;
  }

  snprintf(buffer, sizeof(buffer), "%s, Banks: %s, (ID: %s)", sram_size,
           comment, code);
  return buffer;
}

/// Convert desination code into string
///
/// String in the form:
/// code = "$01";
/// desination = "Overseas only";
///
/// @param vm GB *vm
///
/// @return Destination code string
const char *destination_code_get_str(GB *vm) {
  static char buffer[128];
  const char *code = NULL;
  const char *destination = NULL;

  switch (vm->cart.destination_code) {
  case 0x00:
    code = "$00";
    destination = "Japan (and possibly overseas)";
    break;
  case 0x01:
    code = "$01";
    destination = "Overseas only";
    break;
  default:
    code = "Unknown";
    destination = "Unknown";
    break;
  }

  snprintf(buffer, sizeof(buffer), "%s, (ID: %s)", destination, code);
  return buffer;
}

/// Calculate cart header checksum
///
/// @param vm GB *vm
///
/// @return Checksum as u8
u8 cart_header_checksum_calc(GB *vm) {
  u8 checksum = 0;
  for (u16 address = 0x0134; address <= 0x014C; address++) {
    checksum = checksum - vm->cart.data[address] - 1;
  }
  return checksum;
}

/// Calculate global cart checksum
///
/// Adds together cart header bytes, minus the last checksum itself
///
/// @param vm GB *vm
///
/// @return Checksum as u16
u16 cart_global_checksum_calc(GB *vm) {
  uint16_t checksum = 0;
  for (int i = 0; i < 0x014E; i++) {
    checksum += vm->cart.data[i];
  }

  return checksum;
}

/// Checks cart logo against known Nintendo logo
///
/// @param vm GB *vm
///
/// @return True if matching, false if not
bool logo_check(GB *vm) {
  /// The Nintendo Logo
  ///
  /// To be displayed on power on. The vm fails if its logo doesn't match
  u8 expected_logo[] = {
      0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83,
      0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
      0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63,
      0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E};

  for (u32 i = 0; i < 48; i++) {
    if (vm->cart.logo[i] != expected_logo[i]) {
      fprintf(stderr, "Logo mismatch at byte %02X: expected: %02X got: %02X\n",
              i, expected_logo[i], vm->cart.logo[i]);
      return false;
    }
  }
  return true;
}

/// Set various flags from cartridge header
///
/// @param vm GB *vm
void cart_header_set_flags(GB *vm) {
  logo_check(vm) ? (vm->flag.logo_match = true) : (vm->flag.logo_match = false);

  switch (vm->cart.cgb_flag) {
  case 0x00:
    vm->flag.cgb = NON_CGB;
  case 0x80:
    vm->flag.cgb = CGB_ENHANCED;
  case 0xC0:
    vm->flag.cgb = CGB_ONLY; // Bit 6 ignored, so = CGB_ENHANCED
  }

  vm->cart.sgb_flag == 0x03 ? (vm->flag.sgb = true) : (vm->flag.sgb = false);

  vm->flag.halt = false;
  vm->flag.stop = false;
}

void step(GB *vm) {
  // Gameboy doctor
  // FILE *logfile = fopen("cpu_log.txt", "a");
  // if (logfile) {
  //  fprintf(logfile,
  //          "A:%02X F:%02X B:%02X C:%02X D:%02X E:%02X H:%02X L:%02X SP:%04X "
  //          "PC:%04X PCMEM:%02X,%02X,%02X,%02X\n",
  //          vm->r.a, vm->r.f, vm->r.b, vm->r.c, vm->r.d, vm->r.e, vm->r.h,
  //          vm->r.l, vm->r.sp, vm->r.pc, read_u8(vm, vm->r.pc),
  //          read_u8(vm, vm->r.pc + 1), read_u8(vm, vm->r.pc + 2),
  //          read_u8(vm, vm->r.pc + 3));
  //  fclose(logfile);
  //}

  if (vm->flag.halt) {
    return;
  }

  u8 opcode = vm->mem.data[vm->r.pc++];
  const OPS *instr = &ops[opcode];
  int cycles = op_ticks[opcode];

  // printf("op: %02X, pushed: %02X\n", opcode, vm->mem.data[0xdffd - 4]);

  switch (instr->type) {
  case NO_OP:
    if (instr->func.func_no_op) {
      instr->func.func_no_op(vm);
    }
    break;
  case U8_OP:
    if (instr->func.func_u8_op) {
      u8 operand = read_u8(vm, vm->r.pc);
      instr->func.func_u8_op(vm, operand);
    }
    break;
  case U16_OP:
    if (instr->func.func_u16_op) {
      u16 operand = read_u16(vm, vm->r.pc);
      instr->func.func_u16_op(vm, operand);
    }
    break;
  }

  if (opcode != 0xC3) {
    vm->r.pc += instr->length;
  }

  vm->cycles += cycles;

  update_timers(vm, cycles);
  update_graphics(vm, cycles);
  // debug_tile_display(vm);

  // DI and EI have a delay.
  if (vm->flag.interrupt_disable_handling) {
    vm->flag.interrupt_master_enable = false;
    vm->flag.interrupt_disable_handling = false;
  }
  if (vm->flag.interrupt_enable_handling) {
    vm->flag.interrupt_master_enable = true;
    vm->flag.interrupt_enable_handling = false;
  }

  do_interrupts(vm);
}

void debug_tile_display(GB *vm) {
  int tile_index = 0;
  int tile_size = 16;

  for (int tile_y = 0; tile_y < 144; tile_y += 8) {
    for (int tile_x = 0; tile_x < 160; tile_x += 8) {
      for (int y = 0; y < 8; ++y) {
        uint8_t byte2 = vm->mem.data[0x8000 + tile_index * tile_size + y * 2];
        uint8_t byte1 =
            vm->mem.data[0x8000 + tile_index * tile_size + y * 2 + 1];

        for (int x = 0; x < 8; ++x) {
          uint8_t bit1 = (byte1 >> (7 - x)) & 1;
          uint8_t bit2 = (byte2 >> (7 - x)) & 1;
          uint8_t color = (bit2 << 1) | bit1;

          uint8_t r = 0, g = 0, b = 0;
          switch (color) {
          case 0:
            r = g = b = 255;
            break; // White
          case 1:
            r = g = b = 192;
            break; // Light gray
          case 2:
            r = g = b = 96;
            break; // Dark gray
          case 3:
            r = g = b = 0;
            break; // Black
          }

          vm->framebuffer[tile_y + y][tile_x + x][0] = r;
          vm->framebuffer[tile_y + y][tile_x + x][1] = g;
          vm->framebuffer[tile_y + y][tile_x + x][2] = b;
        }
      }
      tile_index++;
    }
  }
}

void update_graphics(GB *vm, int cycles) {
  set_lcd_status(vm);

  if (!is_lcd_enabled(vm)) {
    return;
  }

  vm->scanline_counter -= cycles;

  if (vm->scanline_counter <= 0) {
    vm->mem.data[LY]++;

    vm->scanline_counter = 456;

    if (vm->mem.data[LY] == 144) {
      vm->mem.data[0xFF85] = 0;
      request_interrupt(vm, 0);
    } else if (vm->mem.data[LY] > 153) {
      write_u8(vm, LY, 0);
    } else if (vm->mem.data[LY] < 144) {
      draw_scanline(vm);
    }
  }
  vm->mem.data[0xFF85] = 0xFF;
}

void set_lcd_status(GB *vm) {
  u8 status = read_u8(vm, 0xFF41);
  if (!is_lcd_enabled(vm)) {
    vm->scanline_counter = 456;
    vm->mem.data[LY] = 0;
    status &= 252;
    status |= 1;
    write_u8(vm, 0xFF41, status);
    return;
  }

  u8 current_line = read_u8(vm, LY);
  u8 current_mode = status & 0x3;
  u8 mode = 0;
  bool req_int = false;

  if (current_line >= 144) {
    mode = 1;
    status |= 1;
    status &= ~2;
    req_int = status & (1 << 4);
  } else {
    int mode2_bounds = 456 - 80;
    int mode3_bounds = mode2_bounds - 172;

    if (vm->scanline_counter >= mode2_bounds) {
      mode = 2;
      status |= 2;
      status &= ~1;
      req_int = status & (1 << 5);
    } else if (vm->scanline_counter >= mode3_bounds) {
      mode = 3;
      status |= 3;
    } else {
      mode = 0;
      status &= ~3;
      req_int = status & (1 << 3);
    }
  }

  if (req_int && (mode != current_mode)) {
    request_interrupt(vm, 1);
  }

  if (current_line == read_u8(vm, 0xFF45)) {
    status |= 4;
    if (status & (1 << 6)) {
      request_interrupt(vm, 1);
    }
  } else {
    status &= ~4;
  }

  write_u8(vm, 0xFF41, status);
}

/// Checks the LCD enable bit at [0xFF40]
///
/// @param vm GB *vm
///
/// @return True if LCD enabled, false if LCD disabled
bool is_lcd_enabled(GB *vm) { return (read_u8(vm, 0xFF40) & 0x80) != 0; }

/// Read a byte
///
/// @param vm GB *vm
/// @param addr Memory address to read from
///
/// @return Byte from memory address
u8 read_u8(GB *vm, u16 addr) {
  // TODO: odd timer behaviour when writing to TAC.

  // Gameboy doctor
  if (addr == 0xFF44) {
    // return 0x90;
  }

  if (addr == 0xFF00) {
    return get_joypad_state(vm);
  } else if (addr >= vm->mem.size) {
    return 0xFF;
  } else {
    return vm->mem.data[addr];
  }
}

u16 read_u16(GB *vm, u16 addr) {
  if (addr + 1 >= vm->mem.size) {
    return 0xFFFF;
  } else {
    return (vm->mem.data[addr + 1] << 8) | vm->mem.data[addr];
  }
}

/// Write an 8-bit value to memory
///
/// Handles restricted memory regions and mirroring for echo RAM
///
/// @param vm GB *vm
/// @param addr Memory address
/// @param value Value to write
void write_u8(GB *vm, u16 addr, u8 value) {
  if (addr == 0xFF46) {
    do_dma_transfer(vm, value);
  } else if (addr == 0xFF04) {
    vm->mem.data[0xFF04] = 0;
    vm->divider_counter = 0;
  } else if (addr == 0xFF00) {
    vm->mem.data[addr] = value;
  } else if (addr < 0x8000) {
    // Handle ROM and RAM banking
  } else if (addr >= 0xE000 && addr < 0xFE00) {
    vm->mem.data[addr] = value;
    vm->mem.data[addr - 0x2000] = value;
  } else if (addr >= 0xFEA0 && addr < 0xFEFF) {
    return;
  } else if (addr >= 0xFF80 && addr <= 0xFFFE) {
    vm->mem.data[addr] = value;
  } else {
    vm->mem.data[addr] = value;
  }
}

/// Write a 16-bit value to memory
///
/// Handles restricted memory regions and mirroring for echo RAM
///
/// @param vm GB *vm
/// @param addr Memory address
/// @param value Value to write
void write_u16(GB *vm, u16 addr, u16 value) {
  u8 low = value & 0xFF;
  u8 high = (value >> 8) & 0xFF;

  if (addr < 0x8000) {
    // Handle ROM and RAM banking
  } else if (addr >= 0xE000 && addr < 0xFE00) {
    vm->mem.data[addr] = low;
    vm->mem.data[addr - 0x2000] = low;
  } else if (addr >= 0xFEA0 && addr < 0xFEFF) {
    return;
  } else if (addr >= 0xFF80 && addr <= 0xFFFE) {
    vm->mem.data[addr] = low;
  } else {
    vm->mem.data[addr] = low;
  }

  if (addr + 1 < 0x8000) {
    // Handle ROM and RAM banking
  } else if (addr + 1 >= 0xE000 && addr + 1 < 0xFE00) {
    vm->mem.data[addr + 1] = high;
    vm->mem.data[addr + 1 - 0x2000] = high;
  } else if (addr + 1 >= 0xFEA0 && addr + 1 < 0xFEFF) {
    return;
  } else if (addr + 1 >= 0xFF80 && addr + 1 <= 0xFFFE) {
    vm->mem.data[addr + 1] = high;
  } else {
    vm->mem.data[addr + 1] = high;
  }
}

void do_dma_transfer(GB *vm, u8 data) {
  u16 address = data << 8;
  for (int i = 0; i < 0xA0; i++) {
    write_u8(vm, 0xFE00 + i, read_u8(vm, address + i));
  }
}

/// Update timers based on the number of cycles passed
///
/// @param vm GB *vm
/// @param cycles Number of cycles to update timers with
void update_timers(GB *vm, u16 cycles) {
  if (is_clock_enabled(vm)) {
    vm->timer_counter += cycles;

    if (vm->timer_counter >= vm->clock_frequency) {
      vm->timer_counter = 0;
      bool overflow = false;

      if (read_u8(vm, 0xFF05) == 0xFF) {
        overflow = true;
      }
      vm->mem.data[0xFF05]++;

      if (overflow) {
        vm->mem.data[0xFF05] = vm->mem.data[0xFF06];

        request_interrupt(vm, 2);
      }
    }
  }

  do_divider_register(vm, cycles);
}

/// Handle the divider register
///
/// Incremented at 16384Hz.
///
/// @param vm GB *vm
/// @param cycles Number of cycles to update divider with
void do_divider_register(GB *vm, u16 cycles) {
  vm->divider_counter += cycles;
  if (vm->divider_counter >= 256) { // 255?
    vm->divider_counter = 0;
    vm->mem.data[0xFF04]++;
  }
}

/// Check if the clock is enabled
///
/// Reads bit 2 of TAC, 0xFF07
///
/// @param vm GB *vm
/// @return True if the clock is enabled, false otherwise
bool is_clock_enabled(GB *vm) { return (read_u8(vm, 0xFF07) & 0x04) != 0; }

/// Get the clock frequency
///
/// Reads bit 1 of TAC, 0xFF07
///
/// @param vm GB *vm
/// @return Clock frequency as an 8-bit value
u8 get_clock_freq(GB *vm) { return read_u8(vm, 0xFF07) & 0x03; }

/// Set the clock frequency
///
/// Based on bit 1 of TAC, we increment our timer_counter at various M-cycle
/// frequencies.
///
/// @param vm GB *vm
void set_clock_freq(GB *vm) {
  switch (get_clock_freq(vm)) {
  case 0:
    vm->clock_frequency = 1024;
    break;
  case 1:
    vm->clock_frequency = 16;
    break;
  case 2:
    vm->clock_frequency = 64;
    break;
  case 3:
    vm->clock_frequency = 256;
    break;
  }
}

/// Request an interrupt
///
/// Read interrupt flags, set a bit, and write flags back with new set bit
///
/// @param vm GB *vm
/// @param interrupt_flag Interrupt flag to set
void request_interrupt(GB *vm, u8 interrupt_flag) {
  u8 req = read_u8(vm, 0xFF0F);
  req |= (1 << interrupt_flag);
  write_u8(vm, 0xFF0F, req);
}

/// Do interrupt routine
///
/// Checks if IME is set, then checks for enabled AND requested interrupts, and
/// services all of them
///
/// @param vm GB *vm
void do_interrupts(GB *vm) {
  if (vm->flag.interrupt_master_enable) {
    u8 req = read_u8(vm, 0xFF0F);
    u8 enabled = read_u8(vm, 0xFFFF);
    u8 fire = req & enabled;

    if (fire > 0) {
      for (int i = 0; i < 8; i++) {
        if (test_bit(fire, i)) {
          service_interrupt(vm, i);
          break;
        }
      }
    }
  }
}

void service_interrupt(GB *vm, u8 interrupt) {
  vm->flag.interrupt_master_enable = false;

  vm->r.sp -= 2;
  write_u16(vm, vm->r.sp, vm->r.pc); // Push PC to stack, interrupts are calls

  switch (interrupt) {
  case 0:
    vm->r.pc = 0x0040;
    break; // V-Blank
  case 1:
    vm->r.pc = 0x0048;
    break; // LCD-STAT
  case 2:
    vm->r.pc = 0x0050;
    break; // Timer
  case 3:
    vm->r.pc = 0x0058;
    break; // Serial
  case 4:
    vm->r.pc = 0x0060;
    break; // Joypad
  default:
    break;
  }

  u8 req = read_u8(vm, 0xFF0F);
  req &= ~(1 << interrupt);
  write_u8(vm, 0xFF0F, req);

  vm->cycles += 20;
}

void draw_scanline(GB *vm) {
  u8 control = read_u8(vm, 0xFF40);

  if (test_bit(control, 0)) {
    render_tiles(vm);
  }
  if (test_bit(control, 1)) {
    render_sprites(vm);
  }
}

void render_tiles(GB *vm) {
  if (test_bit(vm->mem.data[0xFF40], 0)) {
    u16 tile_data = 0;
    u16 background_memory = 0;
    bool unsig = true;

    u8 scroll_y = read_u8(vm, 0xFF42);
    u8 scroll_x = read_u8(vm, 0xFF43);
    u8 window_x = read_u8(vm, 0xFF4A);
    u8 window_y = read_u8(vm, 0xFF4B) - 7;

    bool using_window = false;

    if (test_bit(vm->mem.data[0xFF40], 5)) {
      if (window_y <= read_u8(vm, 0xFF44))
        using_window = true;
    } else {
      using_window = false;
    }

    if (test_bit(vm->mem.data[0xFF40], 4)) {
      tile_data = 0x8000;
    } else {
      tile_data = 0x8800;
      unsig = false;
    }

    if (!using_window) {
      if (test_bit(vm->mem.data[0xFF40], 3)) {
        background_memory = 0x9C00;
      } else {
        background_memory = 0x9800;
      }
    } else {
      if (test_bit(vm->mem.data[0xFF40], 6)) {
        background_memory = 0x9C00;
      } else {
        background_memory = 0x9800;
      }
    }

    u8 y_pos = 0;

    if (!using_window) {
      y_pos = scroll_y + read_u8(vm, LY);
    } else {
      y_pos = read_u8(vm, LY) - window_y;
    }

    u16 tile_row = (((u8)(y_pos / 8)) * 32);

    for (int pixel = 0; pixel < 160; pixel++) {
      u8 x_pos = pixel + scroll_x;

      if (using_window) {
        if (pixel >= window_x) {
          x_pos = pixel - window_x;
        }
      }

      u16 tile_col = (x_pos / 8);
      i16 tile_num;

      u16 tile_address = background_memory + tile_row + tile_col;
      if (unsig) {
        tile_num = (u8)read_u8(vm, tile_address);
      } else {
        tile_num = (i8)read_u8(vm, tile_address);
      }

      u16 tile_location = tile_data;

      if (unsig) {
        tile_location += (tile_num * 16);
      } else {
        tile_location += ((tile_num + 128) * 16);
      }

      u8 line = y_pos % 8;
      line *= 2;

      u8 data_1 = read_u8(vm, (tile_location + line));
      u8 data_2 = read_u8(vm, (tile_location + line + 1));

      i16 colour_bit = x_pos % 8;
      colour_bit = 7 - colour_bit;

      i16 colour_num = ((data_2 >> colour_bit) & 1) << 1;
      colour_num |= (data_1 >> colour_bit) & 1;

      COLOUR col = get_colour(vm, colour_num, 0xFF47);
      i16 red = 0;
      i16 green = 0;
      i16 blue = 0;

      switch (col) {
      case WHITE:
        red = (WHITE >> 16) & 0xFF;
        green = (WHITE >> 8) & 0xFF;
        blue = WHITE & 0xFF;
        break;
      case LIGHT_GRAY:
        red = (LIGHT_GRAY >> 16) & 0xFF;
        green = (LIGHT_GRAY >> 8) & 0xFF;
        blue = LIGHT_GRAY & 0xFF;
        break;
      case DARK_GRAY:
        red = (DARK_GRAY >> 16) & 0xFF;
        green = (DARK_GRAY >> 8) & 0xFF;
        blue = DARK_GRAY & 0xFF;
        break;
      case BLACK:
        red = (BLACK >> 16) & 0xFF;
        green = (BLACK >> 8) & 0xFF;
        blue = BLACK & 0xFF;
        break;
      }

      i16 finally = read_u8(vm, LY);
      if ((finally < 0) || (finally > 143) || (pixel < 0) || (pixel > 159)) {
        continue;
      }

      vm->framebuffer[finally][pixel][0] = red;
      vm->framebuffer[finally][pixel][1] = green;
      vm->framebuffer[finally][pixel][2] = blue;
    }
  }
}

COLOUR get_colour(GB *vm, u8 colour_num, u16 address) {
  COLOUR res = WHITE;
  u8 palette = read_u8(vm, address);
  int hi = 0;
  int lo = 0;

  switch (colour_num) {
  case 0:
    hi = 1;
    lo = 0;
    break;
  case 1:
    hi = 3;
    lo = 2;
    break;
  case 2:
    hi = 5;
    lo = 4;
    break;
  case 3:
    hi = 7;
    lo = 6;
    break;
  }

  int colour = 0;
  colour = ((palette >> hi) & 1) << 1;
  colour |= (palette >> lo) & 1;

  switch (colour) {
  case 0:
    res = WHITE;
    break;
  case 1:
    res = LIGHT_GRAY;
    break;
  case 2:
    res = DARK_GRAY;
    break;
  case 3:
    res = BLACK;
    break;
  }

  return res;
}

void set_pixel(GB *vm, int x, int y, u32 color) {}

void render_sprites(GB *vm) {
  if (test_bit(vm->mem.data[0xFF40], 1)) {
    bool use_8x16 = false;

    if (test_bit(vm->mem.data[0xFF40], 2)) {
      use_8x16 = true;
    }

    for (int sprite = 0; sprite < 40; sprite++) {
      u8 index = sprite * 4;
      u8 y_pos = read_u8(vm, 0xFE00 + index) - 16;
      u8 x_pos = read_u8(vm, 0xFE00 + index + 1) - 8;
      u8 tile_location = read_u8(vm, 0xFE00 + index + 2);
      u8 attributes = read_u8(vm, 0xFE00 + index + 3);

      bool x_flip = test_bit(attributes, 6);
      bool y_flip = test_bit(attributes, 5);

      i16 scanline = read_u16(vm, LY);

      i16 y_size = use_8x16 ? 16 : 8;

      if ((scanline >= y_pos) && (scanline < (y_pos + y_size))) {
        i16 line = scanline - y_pos;

        if (y_flip) {
          line = y_size - 1 - line;
        }

        line *= 2;
        u16 data_address = (0x8000 + (tile_location * 16)) + line;
        u8 data_1 = read_u8(vm, data_address);
        u8 data_2 = read_u8(vm, data_address + 1);

        for (int tile_pixel = 7; tile_pixel >= 0; tile_pixel--) {
          i16 colour_bit = x_flip ? tile_pixel : 7 - tile_pixel;

          i16 colour_num = ((data_2 >> colour_bit) & 1) << 1;
          colour_num |= (data_1 >> colour_bit) & 1;

          COLOUR col = get_colour(vm, colour_num,
                                  test_bit(attributes, 4) ? 0xFF49 : 0xFF48);

          if (col == WHITE) {
            continue;
          }

          i16 red = 0;
          i16 green = 0;
          i16 blue = 0;

          switch (col) {
          case WHITE:
            red = (WHITE >> 16) & 0xFF;
            green = (WHITE >> 8) & 0xFF;
            blue = WHITE & 0xFF;
            break;
          case LIGHT_GRAY:
            red = (LIGHT_GRAY >> 16) & 0xFF;
            green = (LIGHT_GRAY >> 8) & 0xFF;
            blue = LIGHT_GRAY & 0xFF;
            break;
          case DARK_GRAY:
            red = (DARK_GRAY >> 16) & 0xFF;
            green = (DARK_GRAY >> 8) & 0xFF;
            blue = DARK_GRAY & 0xFF;
            break;
          case BLACK:
            red = (BLACK >> 16) & 0xFF;
            green = (BLACK >> 8) & 0xFF;
            blue = BLACK & 0xFF;
            break;
          }

          i16 x_pix = 0 - tile_pixel;
          x_pix += 7;

          i16 pixel = x_pos + x_pix;

          if ((scanline < 0) || (scanline > 143) || (pixel < 0) ||
              (pixel > 159)) {
            continue;
          }

          if (test_bit(attributes, 7) &&
              (vm->framebuffer[scanline][x_pix][0] != 255 ||
               vm->framebuffer[scanline][x_pix][1] != 255 ||
               vm->framebuffer[scanline][x_pix][2] != 255)) {
            continue;
          }

          vm->framebuffer[scanline][x_pix][0] = red;
          vm->framebuffer[scanline][x_pix][1] = green;
          vm->framebuffer[scanline][x_pix][2] = blue;
        }
      }
    }
  }
}

/// Get bit value
///
/// @param byte Byte containing the bit to get
/// @param bit Bit to get
///
/// @return Bit
int bit_get_val(u8 byte, int bit) { return (byte >> bit) & 1; }

/// Test bit value
///
/// @param byte Byte containing the bit to test
/// @param bit Bit to test
///
/// @return True if bit is 1, false if bit is 0
bool test_bit(u8 byte, u8 bit) { return (byte & (1 << bit)) != 0; }

/// Get joypad state
///
/// In the joypad bit, 0xFF00, the high nibble is directional buttons and the
/// low nibble is action buttons (start, select)
///
/// @param vm GB *vm
///
/// @return Joypad state bit
u8 get_joypad_state(GB *vm) {
  u8 res = vm->mem.data[0xFF00];
  res ^= 0xFF;

  if (!test_bit(res, 4)) {
    u8 top_joypad = vm->joypad_state >> 4;
    res |= 0xF0;
    res &= top_joypad;
  } else if (!test_bit(res, 5)) {
    u8 bottom_joypad = vm->joypad_state & 0x0F;
    res |= 0xF0;
    res &= bottom_joypad;
  }

  return res;
}

/// Handles a joypad key press
///
/// Called in gui.cpp, during Dear ImGUI key handling
///
/// @param vm GB *vm
/// @param key Key to process
void key_pressed(GB *vm, u8 key) {
  bool previously_unset = false;

  if (!test_bit(vm->joypad_state, key)) {
    previously_unset = true;
  }

  vm->joypad_state = vm->joypad_state &= ~(1 << key);

  bool button = true;
  if (key > 3) {
    button = true;
  } else {
    button = false;
  }

  u8 key_req = vm->mem.data[0xFF00];
  bool request_interrupt_bool = false;

  if (button && !test_bit(key_req, 5)) {
    request_interrupt_bool = true;
  } else if (!button && !test_bit(key_req, 4)) {
    request_interrupt_bool = true;
  }

  if (request_interrupt_bool && !previously_unset) {
    request_interrupt(vm, 4);
  }
}

/// Handles a joypad key release
///
/// Called in gui.cpp, during Dear ImGUI key handling, unsets joypad key bit
///
/// @param vm GB *vm
/// @param key Key to process
void key_released(GB *vm, int key) {
  vm->joypad_state |= (1 << key);
  vm->mem.data[0xFF00] = vm->joypad_state;
}

/// Set the flags in the registers
///
/// @param vm GB *vm
/// @param z Zero flag
/// @param n Subtract flag
/// @param h Half carry flag
/// @param c Carry flag
void reg_set_flag(GB *vm, u8 z, u8 n, u8 h, u8 c) {
  // 0 remains 0, other values become 1
  z = !!z;
  n = !!n;
  h = !!h;
  c = !!c;

  vm->r.f &= 0x0F;

  vm->r.f |= (z << 7) | (n << 6) | (h << 5) | (c << 4);
}
