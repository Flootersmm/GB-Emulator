#include "gb.h"

/// Uses a macro in gb.h to map strings to hex
const char *cartridgeTypeStrings[] = {
#define X(name, value, string) [value] = string,
    CARTRIDGE_TYPE_LIST
#undef X
};

/// New licensee map populated
Licensee new_licensees[] = {
    {"00", "None"},
    {"01", "Nintendo Research & Development 1"},
    {"08", "Capcom"},
    {"13", "EA (Electronic Arts)"},
    {"18", "Hudson Soft"},
    {"19", "B-AI"},
    {"20", "KSS"},
    {"22", "Planning Office WADA"},
    {"24", "PCM Complete"},
    {"25", "San-X"},
    {"28", "Kemco"},
    {"29", "SETA Corporation"},
    {"30", "Viacom"},
    {"31", "Nintendo"},
    {"32", "Bandai"},
    {"33", "Ocean Software/Acclaim Entertainment"},
    {"34", "Konami"},
    {"35", "HectorSoft"},
    {"37", "Taito"},
    {"38", "Hudson Soft"},
    {"39", "Banpresto"},
    {"41", "Ubi Soft"},
    {"42", "Atlus"},
    {"44", "Malibu Interactive"},
    {"46", "Angel"},
    {"47", "Bullet-Proof Software"},
    {"49", "Irem"},
    {"50", "Absolute"},
    {"51", "Acclaim Entertainment"},
    {"52", "Activision"},
    {"53", "Sammy USA Corporation"},
    {"54", "Konami"},
    {"55", "Hi Tech Expressions"},
    {"56", "LJN"},
    {"57", "Matchbox"},
    {"58", "Mattel"},
    {"59", "Milton Bradley Company"},
    {"60", "Titus Interactive"},
    {"61", "Virgin Games Ltd."},
    {"64", "Lucasfilm Games"},
    {"67", "Ocean Software"},
    {"69", "EA (Electronic Arts)"},
    {"70", "Infogrames"},
    {"71", "Interplay Entertainment"},
    {"72", "Broderbund"},
    {"73", "Sculptured Software"},
    {"75", "The Sales Curve Limited"},
    {"78", "THQ"},
    {"79", "Accolade"},
    {"80", "Misawa Entertainment"},
    {"83", "lozc"},
    {"86", "Tokuma Shoten"},
    {"87", "Tsukuda Original"},
    {"91", "Chunsoft Co."},
    {"92", "Video System"},
    {"93", "Ocean Software/Acclaim Entertainment"},
    {"95", "Varie"},
    {"96", "Yonezawa/s'pal"},
    {"97", "Kaneko"},
    {"99", "Pack-In-Video"},
    {"9H", "Bottom Up"},
    {"A4", "Konami (Yu-Gi-Oh!)"},
    {"BL", "MTO"},
    {"DK", "Kodansha"},
};

/// Old licensee map populated
Licensee old_licensees[] = {
    {"00", "None"},
    {"01", "Nintendo"},
    {"08", "Capcom"},
    {"09", "HOT・B"},
    {"0A", "Jaleco"},
    {"0B", "Coconuts Japan"},
    {"0C", "Elite Systems"},
    {"13", "EA (Electronic Arts)"},
    {"18", "Hudson Soft"},
    {"19", "ITC Entertainment"},
    {"1A", "Yanoman"},
    {"1D", "Japan Clary"},
    {"1F", "Virgin Games Ltd."},
    {"24", "PCM Complete"},
    {"25", "San-X"},
    {"28", "Kemco"},
    {"29", "SETA Corporation"},
    {"30", "Infogrames"},
    {"31", "Nintendo"},
    {"32", "Bandai"},
    {"33", "Indicates that the New licensee code should be used instead."},
    {"34", "Konami"},
    {"35", "HectorSoft"},
    {"38", "Capcom"},
    {"39", "Banpresto"},
    {"3C", ".Entertainment i"},
    {"3E", "Gremlin"},
    {"41", "Ubi Soft"},
    {"42", "Atlus"},
    {"44", "Malibu Interactive"},
    {"46", "Angel"},
    {"47", "Spectrum Holoby"},
    {"49", "Irem"},
    {"4A", "Virgin Games Ltd."},
    {"4D", "Malibu Interactive"},
    {"4F", "U.S. Gold"},
    {"50", "Absolute"},
    {"51", "Acclaim Entertainment"},
    {"52", "Activision"},
    {"53", "Sammy USA Corporation"},
    {"54", "GameTek"},
    {"55", "Park Place"},
    {"56", "LJN"},
    {"57", "Matchbox"},
    {"59", "Milton Bradley Company"},
    {"5A", "Mindscape"},
    {"5B", "Romstar"},
    {"5C", "Naxat Soft"},
    {"5D", "Tradewest"},
    {"60", "Titus Interactive"},
    {"61", "Virgin Games Ltd."},
    {"67", "Ocean Software"},
    {"69", "EA (Electronic Arts)"},
    {"6E", "Elite Systems"},
    {"6F", "Electro Brain"},
    {"70", "Infogrames"},
    {"71", "Interplay Entertainment"},
    {"72", "Broderbund"},
    {"73", "Sculptured Software"},
    {"75", "The Sales Curve Limited"},
    {"78", "THQ"},
    {"79", "Accolade"},
    {"7A", "Triffix Entertainment"},
    {"7C", "Microprose"},
    {"7F", "Kemco"},
    {"80", "Misawa Entertainment"},
    {"83", "Lozc"},
    {"86", "Tokuma Shoten"},
    {"8B", "Bullet-Proof Software"},
    {"8C", "Vic Tokai"},
    {"8E", "Ape"},
    {"8F", "I'Max"},
    {"91", "Chunsoft Co."},
    {"92", "Video System"},
    {"93", "Tsuburaya Productions"},
    {"95", "Varie"},
    {"96", "Yonezawa/S'Pal"},
    {"97", "Kemco"},
    {"99", "Arc"},
    {"9A", "Nihon Bussan"},
    {"9B", "Tecmo"},
    {"9C", "Imagineer"},
    {"9D", "Banpresto"},
    {"9F", "Nova"},
    {"A1", "Hori Electric"},
    {"A2", "Bandai"},
    {"A4", "Konami"},
    {"A6", "Kawada"},
    {"A7", "Takara"},
    {"A9", "Technos Japan"},
    {"AA", "Broderbund"},
    {"AC", "Toei Animation"},
    {"AD", "Toho"},
    {"AF", "Namco"},
    {"B0", "Acclaim Entertainment"},
    {"B1", "ASCII Corporation or Nexsoft"},
    {"B2", "Bandai"},
    {"B4", "Square Enix"},
    {"B6", "HAL Laboratory"},
    {"B7", "SNK"},
    {"B9", "Pony Canyon"},
    {"BA", "Culture Brain"},
    {"BB", "Sunsoft"},
    {"BD", "Sony Imagesoft"},
    {"BF", "Sammy Corporation"},
    {"C0", "Taito"},
    {"C2", "Kemco"},
    {"C3", "Square"},
    {"C4", "Tokuma Shoten"},
    {"C5", "Data East"},
    {"C6", "Tonkinhouse"},
    {"C8", "Koei"},
    {"C9", "UFL"},
    {"CA", "Ultra"},
    {"CB", "Vap"},
    {"CC", "Use Corporation"},
    {"CD", "Meldac"},
    {"CE", "Pony Canyon"},
    {"CF", "Angel"},
    {"D0", "Taito"},
    {"D1", "Sofel"},
    {"D2", "Quest"},
    {"D3", "Sigma Enterprises"},
    {"D4", "ASK Kodansha Co."},
    {"D6", "Naxat Soft"},
    {"D7", "Copya System"},
    {"D9", "Banpresto"},
    {"DA", "Tomy"},
    {"DB", "LJN"},
    {"DD", "NCS"},
    {"DE", "Human"},
    {"DF", "Altron"},
    {"E0", "Jaleco"},
    {"E1", "Towa Chiki"},
    {"E2", "Yutaka"},
    {"E3", "Varie"},
    {"E5", "Epoch"},
    {"E7", "Athena"},
    {"E8", "Asmik Ace Entertainment"},
    {"E9", "Natsume"},
    {"EA", "King Records"},
    {"EB", "Atlus"},
    {"EC", "Epic/Sony Records"},
    {"EE", "IGS"},
    {"F0", "A Wave"},
    {"F3", "Extreme Entertainment"},
    {"FF", "LJN"},
};

/// New licensee table size
const int new_licensee_table_size =
    sizeof(new_licensees) / sizeof(new_licensees[0]);

/// Old licensee table size
const int old_licensee_table_size =
    sizeof(old_licensees) / sizeof(old_licensees[0]);

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
      .ram =
          {
              .item = NULL,
              .size = 255,
          },
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
  };

  printf("%s\n", vm->rom_path);
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

  vm->cart.data = malloc(vm->cart.size + 1);
  fread(vm->cart.data, vm->cart.size, sizeof(u8), fp);
  fclose(fp);

  // TODO: place 256 byte program into 0x0000, this is the poweron program
  // get from disassembly online
  // marc.rawer.de/Gameboy/Docs/GBCPUman.pdf 2.7
  // Then set registers to values and set ram to 0x0

  _cart_header_read(vm);
  _logo_check(vm);
  if (cart_header_checksum_calc(vm) != vm->cart.header_checksum) {
    exit(EXIT_FAILURE);
  };
  _cart_header_set_flags(vm);
  if (!vm->flag.logo_match) {
    exit(EXIT_FAILURE);
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
      exit(EXIT_FAILURE);
    }
  }
  // TODO: Super GB:
  // don't check

  vm->r.h = 0;
  vm->r.a = 0;
  vm->r.b = 0;
  vm->r.c = 0;
  vm->r.d = 0;
  vm->r.e = 0;
  vm->r.f = 0;
  vm->r.af = 0x0001; // Dependent on console type!!
  vm->r.bc = 0x0013;
  vm->r.de = 0x00D8;
  vm->r.hl = 0x014D;
  vm->r.pc = 0;
  vm->r.sp = 0xFFFE;

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
void _free_cartridge(Cartridge *cart) {
  free(cart->data);
  cart->data = NULL;
  cart->size = 0;
}

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
}

void step(GB *vm) {
  u8 instruction = 0;
  u16 operand = 0;

  if (vm->flag.halt) {
    return;
  }
}

u8 read_u8(GB *vm, u16 addr) { return 0; }

u16 read_u16(GB *vm, u16 addr) { return 0; }
