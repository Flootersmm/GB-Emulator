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
/// @return A GB vm instance
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
              .data = malloc(0x10000), // TODO: actual size
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
      .timer_counter = 0x400,
      .divider_counter = 0,
      .tmc = 0,
      .divider_register = 0,
      .tima = 0,
      .tma = 0,
      .current_scanline = 0,
  };

  if (!vm->mem.data) {
    fprintf(stderr, "Error allocating memory for GB RAM\n");
    free(vm);
    return NULL;
  }

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

  // Gameboy doctor
  FILE *logfile = fopen("cpu_log.txt", "w");
  if (logfile) {
    fclose(logfile);
  }

  fclose(fp);

  // TODO: place 256 byte program into 0x0000, this is the poweron program
  // get from disassembly online
  // marc.rawer.de/Gameboy/Docs/GBCPUman.pdf 2.7
  // Then set registers to values and set ram to 0x0

  _cart_header_read(vm);
  _logo_check(vm);
  if ((cart_header_checksum_calc(vm)) != vm->cart.header_checksum) {
    return NULL;
  };
  _cart_header_set_flags(vm);
  if (!vm->flag.logo_match) {
    return NULL;
  };

  _gb_power_on(vm);

  return vm;
}

/// Clean up the GB vm
///
/// @param vm GB vm
///
/// @return 0 for success, -1 for failure
int gb_destroy(GB *vm) { return 0; }

/// Initialise the power on sequence
///
/// @param vm GB vm
///
/// @return 0 for success, -1 for failure
int _gb_power_on(GB *vm) {

  memcpy(vm->mem.data, vm->cart.data, vm->cart.size);

  vm->flag.halt = false;
  vm->flag.interrupt_master_enable = false;
  vm->flag.interrupt_disable_pending = false;
  vm->flag.interrupt_enable_pending = false;

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
    _reg_set_flag(vm, 1, 0, 0, 0);
  } else {
    _reg_set_flag(vm, 1, 0, 1, 1);
  }
  vm->r.pc = 0x0100;
  vm->r.sp = 0xFFFE;

  // DMG
  vm->mem.data[0xFF00] = 0xCF;      // P1
  vm->mem.data[0xFF01] = 0x00;      // SB
  vm->mem.data[0xFF02] = 0x7E;      // SC
  vm->mem.data[0xFF04] = 0xAB;      // DIV
  vm->mem.data[0xFF05] = 0x00;      // TIMA
  vm->mem.data[0xFF06] = 0x00;      // TMA
  vm->mem.data[0xFF07] = 0x00;      // TAC
  vm->mem.data[0xFF10] = 0x80;      // NR10
  vm->mem.data[0xFF11] = 0xBF;      // NR11
  vm->mem.data[0xFF12] = 0xF3;      // NR12
  vm->mem.data[0xFF14] = 0xBF;      // NR14
  vm->mem.data[0xFF16] = 0x3F;      // NR21
  vm->mem.data[0xFF17] = 0x00;      // NR22
  vm->mem.data[0xFF19] = 0xBF;      // NR24
  vm->mem.data[0xFF1A] = 0x7F;      // NR30
  vm->mem.data[0xFF1B] = 0xFF;      // NR31
  vm->mem.data[0xFF1C] = 0x9F;      // NR32
  vm->mem.data[0xFF1E] = 0xBF;      // NR33
  vm->mem.data[0xFF20] = 0xFF;      // NR41
  vm->mem.data[0xFF21] = 0x00;      // NR42
  vm->mem.data[0xFF22] = 0x00;      // NR43
  vm->mem.data[0xFF23] = 0xBF;      // NR30
  vm->mem.data[0xFF24] = 0x77;      // NR50
  vm->mem.data[0xFF25] = 0xF3;      // NR51
  vm->mem.data[0xFF26] = 0xF1;      // NR52
  vm->mem.data[0xFF40] = 0x91;      // LCDC
  vm->mem.data[0xFF42] = 0x00;      // SCY
  vm->mem.data[0xFF43] = 0x00;      // SCX
  vm->mem.data[0xFF45] = 0x00;      // LYC
  vm->mem.data[0xFF47] = 0xFC;      // BGP
  vm->mem.data[0xFF48] = 0xFF;      // OBP0
  vm->mem.data[0xFF49] = 0xFF;      // OBP1
  vm->mem.data[0xFF4A] = 0x00;      // WY
  vm->mem.data[0xFF4B] = 0x00;      // WX
  *vm->mem.interrupt_enable = 0x00; // IE
  write_u8(vm, 0xFF0F, 0x00);
  vm->mem.data[0xFF41] = 0x80; // STAT
  //
  // read logo from the header, unpack it into VRAM, slowly scroll it down.
  // Once finished scrolling, play sound, read logo again, compare to copy,
  // compute header checksum.
  return 0;
}

/// Separates the cart header into distinct fields
///
/// @param vm GB vm
///
/// @return 0 for success, -1 for failure
int _cart_header_read(GB *vm) {
  vm->cart.entry_point = vm->cart.data + 0x0100;
  vm->cart.logo = vm->cart.data + 0x0104;
  // If less than 16 characters, pad with 0s.
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
void _free_cartridge(Cartridge *cart) {}

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
/// @param vm GB vm
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
/// @param vm GB vm
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
/// @param vm GB vm
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
/// @param vm GB vm
///
/// @return Destination code string
const char *destination_code_get_str(GB *vm) {
  static char buffer[128]; // Adjust size as needed
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
/// @param vm GB vm
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
/// @param vm GB vm
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
/// @param vm GB vm
///
/// @return True if matching, false if not
bool _logo_check(GB *vm) {
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
/// @param vm GB vm
void _cart_header_set_flags(GB *vm) {
  _logo_check(vm) ? (vm->flag.logo_match = true)
                  : (vm->flag.logo_match = false);

  switch (vm->cart.cgb_flag) {
  case 0x00:
    vm->flag.cgb = NON_CGB;
  case 0x80:
    vm->flag.cgb = CGB_ENHANCED;
  case 0xC0:
    vm->flag.cgb = CGB_ONLY; ///< Bit 6 ignored, so = CGB_ENHANCED
  }

  vm->cart.sgb_flag == 0x03 ? (vm->flag.sgb = true) : (vm->flag.sgb = false);

  vm->flag.halt = false;
  vm->flag.stop = false;
}

void step(GB *vm) {
  FILE *logfile = fopen("cpu_log.txt", "a");
  if (logfile) {
    fprintf(logfile,
            "A:%02X F:%02X B:%02X C:%02X D:%02X E:%02X H:%02X L:%02X SP:%04X "
            "PC:%04X PCMEM:%02X,%02X,%02X,%02X\n",
            vm->r.a, vm->r.f, vm->r.b, vm->r.c, vm->r.d, vm->r.e, vm->r.h,
            vm->r.l, vm->r.sp, vm->r.pc, read_u8(vm, vm->r.pc),
            read_u8(vm, vm->r.pc + 1), read_u8(vm, vm->r.pc + 2),
            read_u8(vm, vm->r.pc + 3));
    fclose(logfile);
  }

  if (vm->flag.halt) {
    return;
  }

  // printf("ROM:%02X   ", 0);
  // for (int i = 1; i < 0x176; i++) {
  //   if (vm->r.pc == i - 1) {
  //     printf("\033[31m%02X \033[0m", vm->cart.data[i - 1]);
  //   } else {
  //     printf("%02X ", vm->cart.data[i - 1]);
  //   }
  //   if (i % 16 == 0) {
  //     printf("\n");
  //     printf("ROM:%02X   ", i);
  //   }
  // }
  // printf("\n\n\n");

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

  // update_timers(vm, cycles);
  //  update_graphics(vm, cycles);

  // DI and EI have a delay.
  if (vm->flag.interrupt_disable_pending) {
    vm->flag.interrupt_master_enable = false;
    vm->flag.interrupt_disable_pending = false;
  }
  if (vm->flag.interrupt_enable_pending) {
    vm->flag.interrupt_master_enable = true;
    vm->flag.interrupt_enable_pending = false;
  }

  // do_interrupts(vm);
}

void update_graphics(GB *vm, int cycles) {
  set_lcd_status(vm);

  if (!is_lcd_enabled(vm)) {
    vm->scanline_counter = 456;
    vm->mem.data[0xFF44] = 0;
    return;
  }

  vm->scanline_counter -= cycles;

  if (vm->scanline_counter <= 0) {
    // Move to next scanline
    vm->mem.data[0xFF44]++;
    vm->current_scanline = read_u8(vm, 0xFF44);

    vm->scanline_counter = 456;

    if (vm->current_scanline == 144) {
      request_interrupt(vm, 0); // Request V-Blank interrupt
      set_ppu_mode(vm, 1);      // Enter V-Blank mode
    } else if (vm->current_scanline > 153) {
      // End of V-Blank period, reset scanline
      vm->mem.data[0xFF44] = 0;
      set_ppu_mode(vm, 2); // Enter OAM Scan mode for the next frame
    } else if (vm->current_scanline < 144) {
      draw_scanline(vm);   // Draw current scanline
      set_ppu_mode(vm, 0); // Enter H-Blank mode
    }
  }
}

void reset_lcd(GB *vm) {
  vm->scanline_counter = 456;
  write_u8(vm, 0xFF44, 0);
  write_u8(vm, 0xFF41, read_u8(vm, 0xFF41) & ~3);
}

void set_ppu_mode(GB *vm, u8 mode) {
  u8 status = read_u8(vm, 0xFF41);
  status &= 0xFC;          // Clear the lower 2 bits
  status |= (mode & 0x03); // Set the mode bits
  write_u8(vm, 0xFF41, status);

  if (mode == 0 && (status & 0x08)) { // H-Blank interrupt
    request_interrupt(vm, 1);
  } else if (mode == 1 && (status & 0x10)) { // V-Blank interrupt
    request_interrupt(vm, 1);
  } else if (mode == 2 && (status & 0x20)) { // OAM interrupt
    request_interrupt(vm, 1);
  }
}

void set_lcd_status(GB *vm) {
  u8 status = read_u8(vm, 0xFF41);
  if (!is_lcd_enabled(vm)) {
    reset_lcd(vm);
    return;
  }

  u8 current_line = read_u8(vm, 0xFF44);
  u8 current_mode = status & 0x3;
  u8 mode = 0;
  bool req_int = false;

  if (current_line >= 144) {
    mode = 1; // V-Blank
    status = (status & ~3) | 1;
    req_int = (status & (1 << 4)) != 0;
  } else {
    int mode2_bounds = 456 - 80;
    int mode3_bounds = mode2_bounds - 172;

    if (vm->scanline_counter >= mode2_bounds) {
      mode = 2; // OAM Scan
      status = (status & ~3) | 2;
      req_int = (status & (1 << 5)) != 0;
    } else if (vm->scanline_counter >= mode3_bounds) {
      mode = 3; // Drawing
      status = (status & ~3) | 3;
    } else {
      mode = 0; // H-Blank
      status = (status & ~3);
      req_int = (status & (1 << 3)) != 0;
    }
  }

  if (req_int && (mode != current_mode)) {
    request_interrupt(vm, 1); // LCD STAT interrupt
  }

  if (current_line == read_u8(vm, 0xFF45)) {
    status |= 4;
    if (status & (1 << 6)) {
      request_interrupt(vm, 1); // LY=LYC interrupt
    }
  } else {
    status &= ~4;
  }

  write_u8(vm, 0xFF41, status);
}

bool is_lcd_enabled(GB *vm) { return (read_u8(vm, 0xFF40) & 0x80) != 0; }

u8 read_u8(GB *vm, u16 addr) {
  if (addr == 0xFF44) {
    // return vm->current_scanline;

    // Gameboy doctor
    return 0x90;
  } else if (addr == 0xFF00) {
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
/// @param vm GB vm
/// @param addr Memory address
/// @param value Value to write
void write_u8(GB *vm, u16 addr, u8 value) {
  if (addr == 0xFF44) {
    vm->mem.data[0xFF44] = 0;
  } else if (addr == 0xFF46) {
    do_dma_transfer(vm, value);
  } else if (addr == 0xFF00) {
    vm->mem.data[addr] = value;
  } else if (addr < 0x8000) {
    return;
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
/// @param vm GB vm
/// @param addr Memory address
/// @param value Value to write
void write_u16(GB *vm, u16 addr, u16 value) {
  u8 low = value & 0xFF;
  u8 high = (value >> 8) & 0xFF;

  if (addr < 0x8000) {
    return;
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
    return;
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
/// @param vm GB vm
/// @param cycles Number of cycles to update timers with
void update_timers(GB *vm, u16 cycles) {
  do_divider_register(vm, cycles);

  if (is_clock_enabled(vm)) {
    vm->timer_counter -= cycles;

    if (vm->timer_counter <= 0) {
      set_clock_freq(vm);

      if (read_u8(vm, 0xFF05) == 255) {
        write_u8(vm, 0xFF05, read_u8(vm, 0xFF06));
        request_interrupt(vm, 2);
      } else {
        write_u8(vm, 0xFF05, read_u8(vm, 0xFF05) + 1);
      }
    }
  }
}

/// Handle the divider register
///
/// @param vm GB vm
/// @param cycles Number of cycles to update divider with
void do_divider_register(GB *vm, u16 cycles) {
  vm->divider_register += cycles;
  if (vm->divider_counter >= 256) { // 255?
    vm->divider_counter = 0;
    vm->mem.data[0xFF04]++;
  }
}

/// Check if the clock is enabled
///
/// @param vm GB vm
/// @return True if the clock is enabled, false otherwise
bool is_clock_enabled(GB *vm) { return (read_u8(vm, 0xFF07) & 0x04) != 0; }

/// Get the clock frequency
///
/// @param vm GB vm
/// @return Clock frequency as an 8-bit value
u8 get_clock_freq(GB *vm) { return read_u8(vm, 0xFF07) & 0x03; }

/// Set the clock frequency
///
/// @param vm GB vm
void set_clock_freq(GB *vm) {
  switch (get_clock_freq(vm)) {
  case 0:
    vm->timer_counter = 1024;
    break;
  case 1:
    vm->timer_counter = 16;
    break;
  case 2:
    vm->timer_counter = 64;
    break;
  case 3:
    vm->timer_counter = 256;
    break;
  }
}

/// Request an interrupt
///
/// @param vm GB vm
/// @param interrupt_flag Interrupt flag to set
void request_interrupt(GB *vm, u8 interrupt_flag) {
  u8 req = read_u8(vm, 0xFF0F);
  req |= (1 << interrupt_flag);
  write_u8(vm, 0xFF0F, req);
}

void do_interrupts(GB *vm) {
  if (vm->flag.interrupt_master_enable) {
    u8 req = read_u8(vm, 0xFF0F);     // Interrupt Request Register
    u8 enabled = read_u8(vm, 0xFFFF); // Interrupt Enable Register
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
  // Disable interrupts
  vm->flag.interrupt_master_enable = false;

  // Save current program counter
  vm->r.sp -= 2;
  write_u16(vm, vm->r.sp, vm->r.pc);

  // Log the interrupt service
  printf("Servicing interrupt %d\n", interrupt);

  // Set the program counter to the interrupt vector
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

  if (control & 0x01) {
    render_background(vm);
  }
  if (control & 0x02) {
    render_sprites(vm);
  }
}

void render_tiles(GB *vm) {
  u16 tile_data = 0;
  u16 background_memory = 0;
  bool unsig = true;

  u8 scroll_x = read_u8(vm, 0xFF42);
  u8 scroll_y = read_u8(vm, 0xFF43);
  u8 window_x = read_u8(vm, 0xFF4A);
  u8 window_y = read_u8(vm, 0xFF4B) - 7;

  bool using_window = false;

  if (test_bit(vm->mem.data[0xFF40], 4)) {
    tile_data = 0x8000;
  } else {
    tile_data = 0x8800;
    unsig = false;
  }

  if (false == using_window) {
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
    y_pos = scroll_y + read_u8(vm, 0xFF44);
  } else {
    y_pos = read_u8(vm, 0xFF44) - window_y;
  }

  u16 tile_row = (((u8)(y_pos / 8)) * 8);

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
    colour_bit -= 7;
    colour_bit *= -1;

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

    i16 finally = read_u8(vm, 0xFF44);
    if ((finally < 0) || (finally > 143) || (pixel < 0) || (pixel > 159)) {
      continue;
    }

    vm->framebuffer[pixel][finally][0] = red;
    vm->framebuffer[pixel][finally][1] = green;
    vm->framebuffer[pixel][finally][2] = blue;
  }
}

void render_background(GB *vm) {
  u8 scroll_y = read_u8(vm, 0xFF42);
  u8 scroll_x = read_u8(vm, 0xFF43);
  u8 window_y = read_u8(vm, 0xFF4A) - 7;
  u8 window_x = read_u8(vm, 0xFF4B) - 7;
  u8 control = read_u8(vm, 0xFF40);

  bool using_window = false;

  if (control & 0x20 && window_y <= read_u8(vm, 0xFF44)) {
    using_window = true;
  }

  u16 tile_data = (control & 0x10) ? 0x8000 : 0x8800;
  u16 bg_memory = (control & 0x08) ? 0x9C00 : 0x9800;

  u8 y_pos = using_window ? read_u8(vm, 0xFF44) - window_y
                          : scroll_y + read_u8(vm, 0xFF44);
  u16 tile_row = ((y_pos / 8) * 32);

  for (int x = 0; x < 160; x++) {
    u8 x_pos = x + (using_window ? window_x : scroll_x);
    u16 tile_col = x_pos / 8;
    u16 tile_addr = bg_memory + tile_row + tile_col;

    i8 tile_num = (control & 0x10) ? (u8)read_u8(vm, tile_addr)
                                   : (i8)read_u8(vm, tile_addr);
    u16 tile_location =
        tile_data + ((tile_num + (control & 0x10 ? 0 : 128)) * 16);

    u8 line = y_pos % 8;
    line *= 2;
    u8 data1 = read_u8(vm, tile_location + line);
    u8 data2 = read_u8(vm, tile_location + line + 1);

    int color_bit = ((x_pos % 8) - 7) * -1;
    int color_num =
        ((data2 >> color_bit) & 1) << 1 | ((data1 >> color_bit) & 1);

    COLOUR col = get_colour(vm, color_num, 0xFF47);
    set_pixel(vm, x, read_u8(vm, 0xFF44), col);
  }
}

COLOUR get_colour(GB *vm, u8 colour_num, u16 address) {
  u8 palette = read_u8(vm, address);
  int hi = 0, lo = 0;

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

  int colour = ((palette >> hi) & 1) << 1 | ((palette >> lo) & 1);
  switch (colour) {
  case 0:
    return WHITE;
  case 1:
    return LIGHT_GRAY;
  case 2:
    return DARK_GRAY;
  case 3:
    return BLACK;
  default:
    return WHITE; // Default case
  }
}

void set_pixel(GB *vm, int x, int y, COLOUR col) {
  if (x < 0 || x >= 160 || y < 0 || y >= 144) {
    return;
  }

  vm->framebuffer[y][x][0] = (col >> 16) & 0xFF; // Red
  vm->framebuffer[y][x][1] = (col >> 8) & 0xFF;  // Green
  vm->framebuffer[y][x][2] = col & 0xFF;         // Blue
}

void render_sprites(GB *vm) {
  u8 sprite_height = (read_u8(vm, 0xFF40) & 0x04) ? 16 : 8;

  for (int i = 0; i < 40; i++) {
    u8 y_pos = read_u8(vm, 0xFE00 + (i * 4)) - 16;
    u8 x_pos = read_u8(vm, 0xFE00 + (i * 4) + 1) - 8;
    u8 tile_num = read_u8(vm, 0xFE00 + (i * 4) + 2);
    u8 attributes = read_u8(vm, 0xFE00 + (i * 4) + 3);

    bool y_flip = attributes & 0x40;
    bool x_flip = attributes & 0x20;

    if (read_u8(vm, 0xFF44) >= y_pos &&
        read_u8(vm, 0xFF44) < (y_pos + sprite_height)) {
      int line = read_u8(vm, 0xFF44) - y_pos;
      if (y_flip) {
        line -= sprite_height;
        line *= -1;
      }

      line *= 2;
      u16 data_addr = 0x8000 + (tile_num * 16) + line;
      u8 data1 = read_u8(vm, data_addr);
      u8 data2 = read_u8(vm, data_addr + 1);

      for (int x = 0; x < 8; x++) {
        int color_bit = x;
        if (x_flip) {
          color_bit -= 7;
          color_bit *= -1;
        }

        int color_num =
            ((data2 >> color_bit) & 1) << 1 | ((data1 >> color_bit) & 1);

        if (color_num == 0) {
          continue;
        }

        COLOUR col =
            get_colour(vm, color_num, attributes & 0x10 ? 0xFF49 : 0xFF48);
        set_pixel(vm, x_pos + x, read_u8(vm, 0xFF44), col);
      }
    }
  }
}

int bit_get_val(u8 byte, int bit) { return (byte >> bit) & 1; }

bool test_bit(u8 byte, int bit) { return (byte & (1 << bit)) != 0; }

u8 get_joypad_state(GB *vm) {
  u8 res = vm->mem.data[0xFF00];
  res ^= 0xFF; // Flip all bits

  if (!(res & 0x10)) {
    u8 top_joypad = vm->joypad_state >> 4;
    top_joypad |= 0xF0; // Turn the top 4 bits on
    res &= top_joypad;  // Show what buttons are pressed
  } else if (!(res & 0x20)) {
    u8 bottom_joypad = vm->joypad_state & 0x0F;
    bottom_joypad |= 0xF0;
    res &= bottom_joypad;
  }

  return res;
}

void key_pressed(GB *vm, int key) {
  bool previously_unset = !(vm->joypad_state & (1 << key));

  vm->joypad_state &= ~(1 << key); // Set bit to 0 (pressed)

  bool button = (key > 3);
  u8 key_req = vm->mem.data[0xFF00];
  bool request_interrupt_bool = false;

  if (button && !(key_req & 0x20)) {
    request_interrupt_bool = true;
  } else if (!button && !(key_req & 0x10)) {
    request_interrupt_bool = true;
  }

  if (request_interrupt_bool && !previously_unset) {
    request_interrupt(vm, 4);
  }
}

void key_released(GB *vm, int key) {
  vm->joypad_state |= (1 << key); // Set bit to 1 (released)
}

/// Set the flags in the registers
///
/// @param vm GB vm
/// @param z Zero flag
/// @param n Subtract flag
/// @param h Half carry flag
/// @param c Carry flag
void _reg_set_flag(GB *vm, u8 z, u8 n, u8 h, u8 c) {
  // 0 remains 0, other values become 1
  z = !!z;
  n = !!n;
  h = !!h;
  c = !!c;

  vm->r.f &= 0x0F;

  vm->r.f |= (z << 7) | (n << 6) | (h << 5) | (c << 4);
}
