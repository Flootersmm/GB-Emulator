#include "tables.h"
#include "gb.h"
#include "ops.h"

const unsigned char op_ticks[256] = {
    2, 6, 4, 4, 4, 4, 4,  4, // 0x00 - 0x07
    2, 2, 2, 4, 4, 4, 10, 4, // 0x08 - 0x0F
    4, 4, 4, 4, 4, 4, 4,  4, // 0x10 - 0x17
    2, 2, 2, 4, 4, 4, 10, 4, // 0x18 - 0x1F
    6, 6, 6, 6, 6, 6, 6,  6, // 0x20 - 0x27
    6, 6, 6, 6, 6, 6, 6,  6, // 0x28 - 0x2F
    6, 6, 6, 6, 6, 6, 6,  6, // 0x30 - 0x37
    6, 6, 6, 6, 6, 6, 6,  6, // 0x38 - 0x3F
    4, 4, 4, 4, 4, 4, 4,  4, // 0x40 - 0x47
    4, 4, 4, 4, 4, 4, 4,  4, // 0x48 - 0x4F
    4, 4, 4, 4, 4, 4, 4,  4, // 0x50 - 0x57
    4, 4, 4, 4, 4, 4, 4,  4, // 0x58 - 0x5F
    4, 4, 4, 4, 4, 4, 4,  4, // 0x60 - 0x67
    4, 4, 4, 4, 4, 4, 4,  4, // 0x68 - 0x6F
    4, 4, 4, 4, 4, 4, 4,  4, // 0x70 - 0x77
    4, 4, 4, 4, 4, 4, 4,  4, // 0x78 - 0x7F
    2, 2, 2, 2, 2, 2, 2,  2, // 0x80 - 0x87
    2, 2, 2, 2, 2, 2, 2,  2, // 0x88 - 0x8F
    2, 2, 2, 2, 2, 2, 2,  2, // 0x90 - 0x97
    2, 2, 2, 2, 2, 2, 2,  2, // 0x98 - 0x9F
    2, 2, 2, 2, 2, 2, 2,  2, // 0xA0 - 0xA7
    2, 2, 2, 2, 2, 2, 2,  2, // 0xA8 - 0xAF
    2, 2, 2, 2, 2, 2, 2,  2, // 0xB0 - 0xB7
    2, 2, 2, 2, 2, 2, 2,  2, // 0xB8 - 0xBF
    0, 0, 0, 0, 8, 8, 8,  8, // 0xC0 - 0xC7
    0, 0, 0, 0, 8, 8, 8,  8, // 0xC8 - 0xCF
    8, 8, 8, 8, 8, 8, 8,  8, // 0xD0 - 0xD7
    8, 8, 8, 8, 8, 8, 8,  8, // 0xD8 - 0xDF
    8, 8, 8, 8, 8, 8, 8,  8, // 0xE0 - 0xE7
    8, 8, 8, 8, 8, 8, 8,  8, // 0xE8 - 0xEF
    8, 8, 8, 8, 8, 8, 8,  8, // 0xF0 - 0xF7
    8, 8, 8, 8, 8, 8, 8,  8  // 0xF8 - 0xFF
};

const unsigned char op_extended_ticks[256] = {
    8, 8, 8, 8, 8, 8, 8,  8, // 0x00 - 0x07
    8, 8, 8, 8, 8, 8, 16, 8, // 0x08 - 0x0F
    8, 8, 8, 8, 8, 8, 8,  8, // 0x10 - 0x17
    8, 8, 8, 8, 8, 8, 16, 8, // 0x18 - 0x1F
    8, 8, 8, 8, 8, 8, 8,  8, // 0x20 - 0x27
    8, 8, 8, 8, 8, 8, 16, 8, // 0x28 - 0x2F
    8, 8, 8, 8, 8, 8, 8,  8, // 0x30 - 0x37
    8, 8, 8, 8, 8, 8, 16, 8, // 0x38 - 0x3F
    8, 8, 8, 8, 8, 8, 8,  8, // 0x40 - 0x47
    8, 8, 8, 8, 8, 8, 8,  8, // 0x48 - 0x4F
    8, 8, 8, 8, 8, 8, 8,  8, // 0x50 - 0x57
    8, 8, 8, 8, 8, 8, 8,  8, // 0x58 - 0x5F
    8, 8, 8, 8, 8, 8, 8,  8, // 0x60 - 0x67
    8, 8, 8, 8, 8, 8, 8,  8, // 0x68 - 0x6F
    8, 8, 8, 8, 8, 8, 8,  8, // 0x70 - 0x77
    8, 8, 8, 8, 8, 8, 8,  8, // 0x78 - 0x7F
    8, 8, 8, 8, 8, 8, 8,  8, // 0x80 - 0x87
    8, 8, 8, 8, 8, 8, 8,  8, // 0x88 - 0x8F
    8, 8, 8, 8, 8, 8, 8,  8, // 0x90 - 0x97
    8, 8, 8, 8, 8, 8, 8,  8, // 0x98 - 0x9F
    8, 8, 8, 8, 8, 8, 8,  8, // 0xA0 - 0xA7
    8, 8, 8, 8, 8, 8, 8,  8, // 0xA8 - 0xAF
    8, 8, 8, 8, 8, 8, 8,  8, // 0xB0 - 0xB7
    8, 8, 8, 8, 8, 8, 8,  8, // 0xB8 - 0xBF
    8, 8, 8, 8, 8, 8, 8,  8, // 0xC0 - 0xC7
    8, 8, 8, 8, 8, 8, 8,  8, // 0xC8 - 0xCF
    8, 8, 8, 8, 8, 8, 8,  8, // 0xD0 - 0xD7
    8, 8, 8, 8, 8, 8, 8,  8, // 0xD8 - 0xDF
    8, 8, 8, 8, 8, 8, 8,  8, // 0xE0 - 0xE7
    8, 8, 8, 8, 8, 8, 8,  8, // 0xE8 - 0xEF
    8, 8, 8, 8, 8, 8, 8,  8, // 0xF0 - 0xF7
    8, 8, 8, 8, 8, 8, 8,  8  // 0xF8 - 0xFF
};

const unsigned char ioReset[0x100] = {
    0x0F, 0x00, 0x7C, 0xFF, 0x00, 0x00, 0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0x01, 0x80, 0xBF, 0xF3, 0xFF, 0xBF, 0xFF, 0x3F, 0x00,
    0xFF, 0xBF, 0x7F, 0xFF, 0x9F, 0xFF, 0xBF, 0xFF, 0xFF, 0x00, 0x00, 0xBF,
    0x77, 0xF3, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
    0x00, 0xFF, 0x00, 0xFF, 0x91, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC,
    0x00, 0x00, 0x00, 0x00, 0xFF, 0x7E, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0x3E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0xFF, 0xC1, 0x00,
    0xFE, 0xFF, 0xFF, 0xFF, 0xF8, 0xFF, 0x00, 0x00, 0x00, 0x8F, 0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xCE, 0xED, 0x66, 0x66,
    0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
    0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6,
    0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
    0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 0x45, 0xEC, 0x52, 0xFA,
    0x08, 0xB7, 0x07, 0x5D, 0x01, 0xFD, 0xC0, 0xFF, 0x08, 0xFC, 0x00, 0xE5,
    0x0B, 0xF8, 0xC2, 0xCE, 0xF4, 0xF9, 0x0F, 0x7F, 0x45, 0x6D, 0x3D, 0xFE,
    0x46, 0x97, 0x33, 0x5E, 0x08, 0xEF, 0xF1, 0xFF, 0x86, 0x83, 0x24, 0x74,
    0x12, 0xFC, 0x00, 0x9F, 0xB4, 0xB7, 0x06, 0xD5, 0xD0, 0x7A, 0x00, 0x9E,
    0x04, 0x5F, 0x41, 0x2F, 0x1D, 0x77, 0x36, 0x75, 0x81, 0xAA, 0x70, 0x3A,
    0x98, 0xD1, 0x71, 0x02, 0x4D, 0x01, 0xC1, 0xFF, 0x0D, 0x00, 0xD3, 0x05,
    0xF9, 0x00, 0x0B, 0x00};

const OPS ops[256] = {
    {"NOP", 0, NO_OP, {.func_no_op = nop}},                         // 0x00
    {"LD BC, 0x%04X", 2, U16_OP, {.func_u16_op = ld_bc_nn}},        // 0x01
    {"LD (BC), A", 0, NO_OP, {.func_no_op = ld_bc_a}},              // 0x02
    {"INC BC", 0, NO_OP, {.func_no_op = inc_bc}},                   // 0x03
    {"INC B", 0, NO_OP, {.func_no_op = inc_b}},                     // 0x04
    {"DEC B", 0, NO_OP, {.func_no_op = dec_b}},                     // 0x05
    {"LD B, 0x%02X", 1, U8_OP, {.func_u8_op = ld_b_n}},             // 0x06
    {"RLCA", 0, NO_OP, {.func_no_op = rlca}},                       // 0x07
    {"LD (0x%04X), SP", 2, U16_OP, {.func_u16_op = ld_nn_sp}},      // 0x08
    {"ADD HL, BC", 0, NO_OP, {.func_no_op = add_hl_bc}},            // 0x09
    {"LD A, (BC)", 0, NO_OP, {.func_no_op = ld_a_bc}},              // 0x0a
    {"DEC BC", 0, NO_OP, {.func_no_op = dec_bc}},                   // 0x0b
    {"INC C", 0, NO_OP, {.func_no_op = inc_c}},                     // 0x0c
    {"DEC C", 0, NO_OP, {.func_no_op = dec_c}},                     // 0x0d
    {"LD C, 0x%02X", 1, U8_OP, {.func_u8_op = ld_c_n}},             // 0x0e
    {"RRCA", 0, NO_OP, {.func_no_op = rrca}},                       // 0x0f
    {"STOP", 0, NO_OP, {.func_no_op = stop}},                       // 0x10
    {"LD DE, 0x%04X", 2, U16_OP, {.func_u16_op = ld_de_nn}},        // 0x11
    {"LD (DE), A", 0, NO_OP, {.func_no_op = ld_de_a}},              // 0x12
    {"INC DE", 0, NO_OP, {.func_no_op = inc_de}},                   // 0x13
    {"INC D", 0, NO_OP, {.func_no_op = inc_d}},                     // 0x14
    {"DEC D", 0, NO_OP, {.func_no_op = dec_d}},                     // 0x15
    {"LD D, 0x%02X", 1, U8_OP, {.func_u8_op = ld_d_n}},             // 0x16
    {"RLA", 0, NO_OP, {.func_no_op = rla}},                         // 0x17
    {"JR 0x%02X", 1, U8_OP, {.func_u8_op = jr_n}},                  // 0x18
    {"ADD HL, DE", 0, NO_OP, {.func_no_op = add_hl_de}},            // 0x19
    {"LD A, (DE)", 0, NO_OP, {.func_no_op = ld_a_de}},              // 0x1a
    {"DEC DE", 0, NO_OP, {.func_no_op = dec_de}},                   // 0x1b
    {"INC E", 0, NO_OP, {.func_no_op = inc_e}},                     // 0x1c
    {"DEC E", 0, NO_OP, {.func_no_op = dec_e}},                     // 0x1d
    {"LD E, 0x%02X", 1, U8_OP, {.func_u8_op = ld_e_n}},             // 0x1e
    {"RRA", 0, NO_OP, {.func_no_op = rra}},                         // 0x1f
    {"JR NZ, 0x%02X", 1, U8_OP, {.func_u8_op = jr_nz_n}},           // 0x20
    {"LD HL, 0x%04X", 2, U16_OP, {.func_u16_op = ld_hl_nn}},        // 0x21
    {"LDI (HL), A", 0, NO_OP, {.func_no_op = ldi_hl_a}},            // 0x22
    {"INC HL", 0, NO_OP, {.func_no_op = inc_hl_16}},                // 0x23
    {"INC H", 0, NO_OP, {.func_no_op = inc_h}},                     // 0x24
    {"DEC H", 0, NO_OP, {.func_no_op = dec_h}},                     // 0x25
    {"LD H, 0x%02X", 1, U8_OP, {.func_u8_op = ld_h_n}},             // 0x26
    {"DAA", 0, NO_OP, {.func_no_op = daa}},                         // 0x27
    {"JR Z, 0x%02X", 1, U8_OP, {.func_u8_op = jr_z_n}},             // 0x28
    {"ADD HL, HL", 0, NO_OP, {.func_no_op = add_hl_hl}},            // 0x29
    {"LDI A, (HL)", 0, NO_OP, {.func_no_op = ldi_a_hl}},            // 0x2a
    {"DEC HL", 0, NO_OP, {.func_no_op = dec_hl_16}},                // 0x2b
    {"INC L", 0, NO_OP, {.func_no_op = inc_l}},                     // 0x2c
    {"DEC L", 0, NO_OP, {.func_no_op = dec_l}},                     // 0x2d
    {"LD L, 0x%02X", 1, U8_OP, {.func_u8_op = ld_l_n}},             // 0x2e
    {"CPL", 0, NO_OP, {.func_no_op = cpl}},                         // 0x2f
    {"JR NC, 0x%02X", 1, U8_OP, {.func_u8_op = jr_nc_n}},           // 0x30
    {"LD SP, 0x%04X", 2, U16_OP, {.func_u16_op = ld_sp_nn}},        // 0x31
    {"LDD (HL), A", 0, NO_OP, {.func_no_op = ldd_hl_a}},            // 0x32
    {"INC SP", 0, NO_OP, {.func_no_op = inc_sp}},                   // 0x33
    {"INC (HL)", 0, NO_OP, {.func_no_op = inc_hl}},                 // 0x34
    {"DEC (HL)", 0, NO_OP, {.func_no_op = dec_hl}},                 // 0x35
    {"LD (HL), 0x%02X", 1, U8_OP, {.func_u8_op = ld_hl_n}},         // 0x36
    {"SCF", 0, NO_OP, {.func_no_op = scf}},                         // 0x37
    {"JR C, 0x%02X", 1, U8_OP, {.func_u8_op = jr_c_n}},             // 0x38
    {"ADD HL, SP", 0, NO_OP, {.func_no_op = add_hl_sp}},            // 0x39
    {"LDD A, (HL)", 0, NO_OP, {.func_no_op = ldd_a_hl}},            // 0x3a
    {"DEC SP", 0, NO_OP, {.func_no_op = dec_sp}},                   // 0x3b
    {"INC A", 0, NO_OP, {.func_no_op = inc_a}},                     // 0x3c
    {"DEC A", 0, NO_OP, {.func_no_op = dec_a}},                     // 0x3d
    {"LD A, 0x%02X", 1, U8_OP, {.func_u8_op = ld_a_n}},             // 0x3e
    {"CCF", 0, NO_OP, {.func_no_op = ccf}},                         // 0x3f
    {"LD B, B", 0, NO_OP, {.func_no_op = ld_b_b}},                  // 0x40
    {"LD B, C", 0, NO_OP, {.func_no_op = ld_b_c}},                  // 0x41
    {"LD B, D", 0, NO_OP, {.func_no_op = ld_b_d}},                  // 0x42
    {"LD B, E", 0, NO_OP, {.func_no_op = ld_b_e}},                  // 0x43
    {"LD B, H", 0, NO_OP, {.func_no_op = ld_b_h}},                  // 0x44
    {"LD B, L", 0, NO_OP, {.func_no_op = ld_b_l}},                  // 0x45
    {"LD B, (HL)", 0, NO_OP, {.func_no_op = ld_b_hl}},              // 0x46
    {"LD B, A", 0, NO_OP, {.func_no_op = ld_b_a}},                  // 0x47
    {"LD C, B", 0, NO_OP, {.func_no_op = ld_c_b}},                  // 0x48
    {"LD C, C", 0, NO_OP, {.func_no_op = ld_c_c}},                  // 0x49
    {"LD C, D", 0, NO_OP, {.func_no_op = ld_c_d}},                  // 0x4a
    {"LD C, E", 0, NO_OP, {.func_no_op = ld_c_e}},                  // 0x4b
    {"LD C, H", 0, NO_OP, {.func_no_op = ld_c_h}},                  // 0x4c
    {"LD C, L", 0, NO_OP, {.func_no_op = ld_c_l}},                  // 0x4d
    {"LD C, (HL)", 0, NO_OP, {.func_no_op = ld_c_hl}},              // 0x4e
    {"LD C, A", 0, NO_OP, {.func_no_op = ld_c_a}},                  // 0x4f
    {"LD D, B", 0, NO_OP, {.func_no_op = ld_d_b}},                  // 0x50
    {"LD D, C", 0, NO_OP, {.func_no_op = ld_d_c}},                  // 0x51
    {"LD D, D", 0, NO_OP, {.func_no_op = ld_d_d}},                  // 0x52
    {"LD D, E", 0, NO_OP, {.func_no_op = ld_d_e}},                  // 0x53
    {"LD D, H", 0, NO_OP, {.func_no_op = ld_d_h}},                  // 0x54
    {"LD D, L", 0, NO_OP, {.func_no_op = ld_d_l}},                  // 0x55
    {"LD D, (HL)", 0, NO_OP, {.func_no_op = ld_d_hl}},              // 0x56
    {"LD D, A", 0, NO_OP, {.func_no_op = ld_d_a}},                  // 0x57
    {"LD E, B", 0, NO_OP, {.func_no_op = ld_e_b}},                  // 0x58
    {"LD E, C", 0, NO_OP, {.func_no_op = ld_e_c}},                  // 0x59
    {"LD E, D", 0, NO_OP, {.func_no_op = ld_e_d}},                  // 0x5a
    {"LD E, E", 0, NO_OP, {.func_no_op = ld_e_e}},                  // 0x5b
    {"LD E, H", 0, NO_OP, {.func_no_op = ld_e_h}},                  // 0x5c
    {"LD E, L", 0, NO_OP, {.func_no_op = ld_e_l}},                  // 0x5d
    {"LD E, (HL)", 0, NO_OP, {.func_no_op = ld_e_hl}},              // 0x5e
    {"LD E, A", 0, NO_OP, {.func_no_op = ld_e_a}},                  // 0x5f
    {"LD H, B", 0, NO_OP, {.func_no_op = ld_h_b}},                  // 0x60
    {"LD H, C", 0, NO_OP, {.func_no_op = ld_h_c}},                  // 0x61
    {"LD H, D", 0, NO_OP, {.func_no_op = ld_h_d}},                  // 0x62
    {"LD H, E", 0, NO_OP, {.func_no_op = ld_h_e}},                  // 0x63
    {"LD H, H", 0, NO_OP, {.func_no_op = ld_h_h}},                  // 0x64
    {"LD H, L", 0, NO_OP, {.func_no_op = ld_h_l}},                  // 0x65
    {"LD H, (HL)", 0, NO_OP, {.func_no_op = ld_h_hl}},              // 0x66
    {"LD H, A", 0, NO_OP, {.func_no_op = ld_h_a}},                  // 0x67
    {"LD L, B", 0, NO_OP, {.func_no_op = ld_l_b}},                  // 0x68
    {"LD L, C", 0, NO_OP, {.func_no_op = ld_l_c}},                  // 0x69
    {"LD L, D", 0, NO_OP, {.func_no_op = ld_l_d}},                  // 0x6a
    {"LD L, E", 0, NO_OP, {.func_no_op = ld_l_e}},                  // 0x6b
    {"LD L, H", 0, NO_OP, {.func_no_op = ld_l_h}},                  // 0x6c
    {"LD L, L", 0, NO_OP, {.func_no_op = ld_l_l}},                  // 0x6d
    {"LD L, (HL)", 0, NO_OP, {.func_no_op = ld_l_hl}},              // 0x6e
    {"LD L, A", 0, NO_OP, {.func_no_op = ld_l_a}},                  // 0x6f
    {"LD (HL), B", 0, NO_OP, {.func_no_op = ld_hl_b}},              // 0x70
    {"LD (HL), C", 0, NO_OP, {.func_no_op = ld_hl_c}},              // 0x71
    {"LD (HL), D", 0, NO_OP, {.func_no_op = ld_hl_d}},              // 0x72
    {"LD (HL), E", 0, NO_OP, {.func_no_op = ld_hl_e}},              // 0x73
    {"LD (HL), H", 0, NO_OP, {.func_no_op = ld_hl_h}},              // 0x74
    {"LD (HL), L", 0, NO_OP, {.func_no_op = ld_hl_l}},              // 0x75
    {"HALT", 0, NO_OP, {.func_no_op = halt}},                       // 0x76
    {"LD (HL), A", 0, NO_OP, {.func_no_op = ld_hl_a}},              // 0x77
    {"LD A, B", 0, NO_OP, {.func_no_op = ld_a_b}},                  // 0x78
    {"LD A, C", 0, NO_OP, {.func_no_op = ld_a_c}},                  // 0x79
    {"LD A, D", 0, NO_OP, {.func_no_op = ld_a_d}},                  // 0x7a
    {"LD A, E", 0, NO_OP, {.func_no_op = ld_a_e}},                  // 0x7b
    {"LD A, H", 0, NO_OP, {.func_no_op = ld_a_h}},                  // 0x7c
    {"LD A, L", 0, NO_OP, {.func_no_op = ld_a_l}},                  // 0x7d
    {"LD A, (HL)", 0, NO_OP, {.func_no_op = ld_a_hl}},              // 0x7e
    {"LD A, A", 0, NO_OP, {.func_no_op = ld_a_a}},                  // 0x7f
    {"ADD A, B", 0, NO_OP, {.func_no_op = add_a_b}},                // 0x80
    {"ADD A, C", 0, NO_OP, {.func_no_op = add_a_c}},                // 0x81
    {"ADD A, D", 0, NO_OP, {.func_no_op = add_a_d}},                // 0x82
    {"ADD A, E", 0, NO_OP, {.func_no_op = add_a_e}},                // 0x83
    {"ADD A, H", 0, NO_OP, {.func_no_op = add_a_h}},                // 0x84
    {"ADD A, L", 0, NO_OP, {.func_no_op = add_a_l}},                // 0x85
    {"ADD A, (HL)", 0, NO_OP, {.func_no_op = add_a_hl}},            // 0x86
    {"ADD A, A", 0, NO_OP, {.func_no_op = add_a_a}},                // 0x87
    {"ADC B", 0, NO_OP, {.func_no_op = adc_a_b}},                   // 0x88
    {"ADC C", 0, NO_OP, {.func_no_op = adc_a_c}},                   // 0x89
    {"ADC D", 0, NO_OP, {.func_no_op = adc_a_d}},                   // 0x8a
    {"ADC E", 0, NO_OP, {.func_no_op = adc_a_e}},                   // 0x8b
    {"ADC H", 0, NO_OP, {.func_no_op = adc_a_h}},                   // 0x8c
    {"ADC L", 0, NO_OP, {.func_no_op = adc_a_l}},                   // 0x8d
    {"ADC (HL)", 0, NO_OP, {.func_no_op = adc_a_hl}},               // 0x8e
    {"ADC A", 0, NO_OP, {.func_no_op = adc_a_a}},                   // 0x8f
    {"SUB B", 0, NO_OP, {.func_no_op = sub_b}},                     // 0x90
    {"SUB C", 0, NO_OP, {.func_no_op = sub_c}},                     // 0x91
    {"SUB D", 0, NO_OP, {.func_no_op = sub_d}},                     // 0x92
    {"SUB E", 0, NO_OP, {.func_no_op = sub_e}},                     // 0x93
    {"SUB H", 0, NO_OP, {.func_no_op = sub_h}},                     // 0x94
    {"SUB L", 0, NO_OP, {.func_no_op = sub_l}},                     // 0x95
    {"SUB (HL)", 0, NO_OP, {.func_no_op = sub_hl}},                 // 0x96
    {"SUB A", 0, NO_OP, {.func_no_op = sub_a}},                     // 0x97
    {"SBC B", 0, NO_OP, {.func_no_op = sbc_a_b}},                   // 0x98
    {"SBC C", 0, NO_OP, {.func_no_op = sbc_a_c}},                   // 0x99
    {"SBC D", 0, NO_OP, {.func_no_op = sbc_a_d}},                   // 0x9a
    {"SBC E", 0, NO_OP, {.func_no_op = sbc_a_e}},                   // 0x9b
    {"SBC H", 0, NO_OP, {.func_no_op = sbc_a_h}},                   // 0x9c
    {"SBC L", 0, NO_OP, {.func_no_op = sbc_a_l}},                   // 0x9d
    {"SBC (HL)", 0, NO_OP, {.func_no_op = sbc_a_hl}},               // 0x9e
    {"SBC A", 0, NO_OP, {.func_no_op = sbc_a_a}},                   // 0x9f
    {"AND B", 0, NO_OP, {.func_no_op = and_b}},                     // 0xa0
    {"AND C", 0, NO_OP, {.func_no_op = and_c}},                     // 0xa1
    {"AND D", 0, NO_OP, {.func_no_op = and_d}},                     // 0xa2
    {"AND E", 0, NO_OP, {.func_no_op = and_e}},                     // 0xa3
    {"AND H", 0, NO_OP, {.func_no_op = and_h}},                     // 0xa4
    {"AND L", 0, NO_OP, {.func_no_op = and_l}},                     // 0xa5
    {"AND (HL)", 0, NO_OP, {.func_no_op = and_hl}},                 // 0xa6
    {"AND A", 0, NO_OP, {.func_no_op = and_a}},                     // 0xa7
    {"XOR B", 0, NO_OP, {.func_no_op = xor_b}},                     // 0xa8
    {"XOR C", 0, NO_OP, {.func_no_op = xor_c}},                     // 0xa9
    {"XOR D", 0, NO_OP, {.func_no_op = xor_d}},                     // 0xaa
    {"XOR E", 0, NO_OP, {.func_no_op = xor_e}},                     // 0xab
    {"XOR H", 0, NO_OP, {.func_no_op = xor_h}},                     // 0xac
    {"XOR L", 0, NO_OP, {.func_no_op = xor_l}},                     // 0xad
    {"XOR (HL)", 0, NO_OP, {.func_no_op = xor_hl}},                 // 0xae
    {"XOR A", 0, NO_OP, {.func_no_op = xor_a}},                     // 0xaf
    {"OR B", 0, NO_OP, {.func_no_op = or_b}},                       // 0xb0
    {"OR C", 0, NO_OP, {.func_no_op = or_c}},                       // 0xb1
    {"OR D", 0, NO_OP, {.func_no_op = or_d}},                       // 0xb2
    {"OR E", 0, NO_OP, {.func_no_op = or_e}},                       // 0xb3
    {"OR H", 0, NO_OP, {.func_no_op = or_h}},                       // 0xb4
    {"OR L", 0, NO_OP, {.func_no_op = or_l}},                       // 0xb5
    {"OR (HL)", 0, NO_OP, {.func_no_op = or_hl}},                   // 0xb6
    {"OR A", 0, NO_OP, {.func_no_op = or_a}},                       // 0xb7
    {"CP B", 0, NO_OP, {.func_no_op = cp_b}},                       // 0xb8
    {"CP C", 0, NO_OP, {.func_no_op = cp_c}},                       // 0xb9
    {"CP D", 0, NO_OP, {.func_no_op = cp_d}},                       // 0xba
    {"CP E", 0, NO_OP, {.func_no_op = cp_e}},                       // 0xbb
    {"CP H", 0, NO_OP, {.func_no_op = cp_h}},                       // 0xbc
    {"CP L", 0, NO_OP, {.func_no_op = cp_l}},                       // 0xbd
    {"CP (HL)", 0, NO_OP, {.func_no_op = cp_hl}},                   // 0xbe
    {"CP A", 0, NO_OP, {.func_no_op = cp_a}},                       // 0xbf
    {"RET NZ", 0, NO_OP, {.func_no_op = ret_nz}},                   // 0xc0
    {"POP BC", 0, NO_OP, {.func_no_op = pop_bc}},                   // 0xc1
    {"JP NZ, 0x%04X", 2, U16_OP, {.func_u16_op = jp_nz_nn}},        // 0xc2
    {"JP 0x%04X", 2, U16_OP, {.func_u16_op = jp_nn}},               // 0xc3
    {"CALL NZ, 0x%04X", 2, U16_OP, {.func_u16_op = call_nz_nn}},    // 0xc4
    {"PUSH BC", 0, NO_OP, {.func_no_op = push_bc}},                 // 0xc5
    {"ADD A, 0x%02X", 1, U8_OP, {.func_u8_op = add_a_n}},           // 0xc6
    {"RST 0x00", 0, NO_OP, {.func_no_op = rst_00}},                 // 0xc7
    {"RET Z", 0, NO_OP, {.func_no_op = ret_z}},                     // 0xc8
    {"RET", 0, NO_OP, {.func_no_op = ret}},                         // 0xc9
    {"JP Z, 0x%04X", 2, U16_OP, {.func_u16_op = jp_z_nn}},          // 0xca
    {"CB %02X", 1, U8_OP, {.func_u8_op = cb_n}},                    // 0xcb
    {"CALL Z, 0x%04X", 2, U16_OP, {.func_u16_op = call_z_nn}},      // 0xcc
    {"CALL 0x%04X", 2, U16_OP, {.func_u16_op = call_nn}},           // 0xcd
    {"ADC 0x%02X", 1, U8_OP, {.func_u8_op = adc_a_n}},              // 0xce
    {"RST 0x08", 0, NO_OP, {.func_no_op = rst_08}},                 // 0xcf
    {"RET NC", 0, NO_OP, {.func_no_op = ret_nc}},                   // 0xd0
    {"POP DE", 0, NO_OP, {.func_no_op = pop_de}},                   // 0xd1
    {"JP NC, 0x%04X", 2, U16_OP, {.func_u16_op = jp_nc_nn}},        // 0xd2
    {"UNKNOWN", 0, NO_OP, {.func_no_op = nop}},                     // 0xd3
    {"CALL NC, 0x%04X", 2, U16_OP, {.func_u16_op = call_nc_nn}},    // 0xd4
    {"PUSH DE", 0, NO_OP, {.func_no_op = push_de}},                 // 0xd5
    {"SUB 0x%02X", 1, U8_OP, {.func_u8_op = sub_n}},                // 0xd6
    {"RST 0x10", 0, NO_OP, {.func_no_op = rst_10}},                 // 0xd7
    {"RET C", 0, NO_OP, {.func_no_op = ret_c}},                     // 0xd8
    {"RETI", 0, NO_OP, {.func_no_op = reti}},                       // 0xd9
    {"JP C, 0x%04X", 2, U16_OP, {.func_u16_op = jp_c_nn}},          // 0xda
    {"UNKNOWN", 0, NO_OP, {.func_no_op = nop}},                     // 0xdb
    {"CALL C, 0x%04X", 2, U16_OP, {.func_u16_op = call_c_nn}},      // 0xdc
    {"UNKNOWN", 0, NO_OP, {.func_no_op = nop}},                     // 0xdd
    {"SBC 0x%02X", 1, U8_OP, {.func_u8_op = sbc_a_n}},              // 0xde
    {"RST 0x18", 0, NO_OP, {.func_no_op = rst_18}},                 // 0xdf
    {"LD (0xFF00 + 0x%02X), A", 1, U8_OP, {.func_u8_op = ldh_n_a}}, // 0xe0
    {"POP HL", 0, NO_OP, {.func_no_op = pop_hl}},                   // 0xe1
    {"LD (0xFF00 + C), A", 0, NO_OP, {.func_no_op = ld_0xffc_a}},   // 0xe2
    {"UNKNOWN", 0, NO_OP, {.func_no_op = nop}},                     // 0xe3
    {"UNKNOWN", 0, NO_OP, {.func_no_op = nop}},                     // 0xe4
    {"PUSH HL", 0, NO_OP, {.func_no_op = push_hl}},                 // 0xe5
    {"AND 0x%02X", 1, U8_OP, {.func_u8_op = and_n}},                // 0xe6
    {"RST 0x20", 0, NO_OP, {.func_no_op = rst_20}},                 // 0xe7
    {"ADD SP,0x%02X", 1, U8_OP, {.func_u8_op = add_sp_n}},          // 0xe8
    {"JP HL", 0, NO_OP, {.func_no_op = jp_hl}},                     // 0xe9
    {"LD (0x%04X), A", 2, U16_OP, {.func_u16_op = ld_nn_a}},        // 0xea
    {"UNKNOWN", 0, NO_OP, {.func_no_op = nop}},                     // 0xeb
    {"UNKNOWN", 0, NO_OP, {.func_no_op = nop}},                     // 0xec
    {"UNKNOWN", 0, NO_OP, {.func_no_op = nop}},                     // 0xed
    {"XOR 0x%02X", 1, U8_OP, {.func_u8_op = xor_n}},                // 0xee
    {"RST 0x28", 0, NO_OP, {.func_no_op = rst_28}},                 // 0xef
    {"LD A, (0xFF00 + 0x%02X)", 1, U8_OP, {.func_u8_op = ldh_a_n}}, // 0xf0
    {"POP AF", 0, NO_OP, {.func_no_op = pop_af}},                   // 0xf0
    {"LD A, (0xFF00 + C)", 0, NO_OP, {.func_no_op = ld_a_0xffc}},   // 0xf2
    {"DI", 0, NO_OP, {.func_no_op = di}},                           // 0xf3
    {"UNKNOWN", 0, NO_OP, {.func_no_op = nop}},                     // 0xf4
    {"PUSH AF", 0, NO_OP, {.func_no_op = push_af}},                 // 0xf5
    {"OR 0x%02X", 1, U8_OP, {.func_u8_op = or_n}},                  // 0xf6
    {"RST 0x30", 0, NO_OP, {.func_no_op = rst_30}},                 // 0xf7
    {"LD HL, SP+0x%02X", 1, U8_OP, {.func_u8_op = ld_hl_sp_n}},     // 0xf8
    {"LD SP, HL", 0, NO_OP, {.func_no_op = ld_sp_hl}},              // 0xf9
    {"LD A, (0x%04X)", 2, U16_OP, {.func_u16_op = ld_a_nn}},        // 0xfa
    {"EI", 0, NO_OP, {.func_no_op = ei}},                           // 0xfb
    {"UNKNOWN", 0, NO_OP, {.func_no_op = nop}},                     // 0xfc
    {"UNKNOWN", 0, NO_OP, {.func_no_op = nop}},                     // 0xfd
    {"CP 0x%02X", 1, U8_OP, {.func_u8_op = cp_n}},                  // 0xfe
    {"RST 0x38", 0, NO_OP, {.func_no_op = rst_38}}                  // 0xff
};

const OPS_extended ops_extended[256] = {
    {"RLC B", 0, NO_OP, {.func_no_op = rlc_b}},           // 0x00
    {"RLC C", 0, NO_OP, {.func_no_op = rlc_c}},           // 0x01
    {"RLC D", 0, NO_OP, {.func_no_op = rlc_d}},           // 0x02
    {"RLC E", 0, NO_OP, {.func_no_op = rlc_e}},           // 0x03
    {"RLC H", 0, NO_OP, {.func_no_op = rlc_h}},           // 0x04
    {"RLC L", 0, NO_OP, {.func_no_op = rlc_l}},           // 0x05
    {"RLC (HL)", 0, NO_OP, {.func_no_op = rlc_hl}},       // 0x06
    {"RLC A", 0, NO_OP, {.func_no_op = rlc_a}},           // 0x07
    {"RRC B", 0, NO_OP, {.func_no_op = rrc_b}},           // 0x08
    {"RRC C", 0, NO_OP, {.func_no_op = rrc_c}},           // 0x09
    {"RRC D", 0, NO_OP, {.func_no_op = rrc_d}},           // 0x0a
    {"RRC E", 0, NO_OP, {.func_no_op = rrc_e}},           // 0x0b
    {"RRC H", 0, NO_OP, {.func_no_op = rrc_h}},           // 0x0c
    {"RRC L", 0, NO_OP, {.func_no_op = rrc_l}},           // 0x0d
    {"RRC (HL)", 0, NO_OP, {.func_no_op = rrc_hl}},       // 0x0e
    {"RRC A", 0, NO_OP, {.func_no_op = rrc_a}},           // 0x0f
    {"RL B", 0, NO_OP, {.func_no_op = rl_b}},             // 0x10
    {"RL C", 0, NO_OP, {.func_no_op = rl_c}},             // 0x11
    {"RL D", 0, NO_OP, {.func_no_op = rl_d}},             // 0x12
    {"RL E", 0, NO_OP, {.func_no_op = rl_e}},             // 0x13
    {"RL H", 0, NO_OP, {.func_no_op = rl_h}},             // 0x14
    {"RL L", 0, NO_OP, {.func_no_op = rl_l}},             // 0x15
    {"RL (HL)", 0, NO_OP, {.func_no_op = rl_hl}},         // 0x16
    {"RL A", 0, NO_OP, {.func_no_op = rl_a}},             // 0x17
    {"RR B", 0, NO_OP, {.func_no_op = rr_b}},             // 0x18
    {"RR C", 0, NO_OP, {.func_no_op = rr_c}},             // 0x19
    {"RR D", 0, NO_OP, {.func_no_op = rr_d}},             // 0x1a
    {"RR E", 0, NO_OP, {.func_no_op = rr_e}},             // 0x1b
    {"RR H", 0, NO_OP, {.func_no_op = rr_h}},             // 0x1c
    {"RR L", 0, NO_OP, {.func_no_op = rr_l}},             // 0x1d
    {"RR (HL)", 0, NO_OP, {.func_no_op = rr_hl}},         // 0x1e
    {"RR A", 0, NO_OP, {.func_no_op = rr_a}},             // 0x1f
    {"SLA B", 0, NO_OP, {.func_no_op = sla_b}},           // 0x20
    {"SLA C", 0, NO_OP, {.func_no_op = sla_c}},           // 0x21
    {"SLA D", 0, NO_OP, {.func_no_op = sla_d}},           // 0x22
    {"SLA E", 0, NO_OP, {.func_no_op = sla_e}},           // 0x23
    {"SLA H", 0, NO_OP, {.func_no_op = sla_h}},           // 0x24
    {"SLA L", 0, NO_OP, {.func_no_op = sla_l}},           // 0x25
    {"SLA (HL)", 0, NO_OP, {.func_no_op = sla_hl}},       // 0x26
    {"SLA A", 0, NO_OP, {.func_no_op = sla_a}},           // 0x27
    {"SRA B", 0, NO_OP, {.func_no_op = sra_b}},           // 0x28
    {"SRA C", 0, NO_OP, {.func_no_op = sra_c}},           // 0x29
    {"SRA D", 0, NO_OP, {.func_no_op = sra_d}},           // 0x2a
    {"SRA E", 0, NO_OP, {.func_no_op = sra_e}},           // 0x2b
    {"SRA H", 0, NO_OP, {.func_no_op = sra_h}},           // 0x2c
    {"SRA L", 0, NO_OP, {.func_no_op = sra_l}},           // 0x2d
    {"SRA (HL)", 0, NO_OP, {.func_no_op = sra_hl}},       // 0x2e
    {"SRA A", 0, NO_OP, {.func_no_op = sra_a}},           // 0x2f
    {"SWAP B", 0, NO_OP, {.func_no_op = swap_b}},         // 0x30
    {"SWAP C", 0, NO_OP, {.func_no_op = swap_c}},         // 0x31
    {"SWAP D", 0, NO_OP, {.func_no_op = swap_d}},         // 0x32
    {"SWAP E", 0, NO_OP, {.func_no_op = swap_e}},         // 0x33
    {"SWAP H", 0, NO_OP, {.func_no_op = swap_h}},         // 0x34
    {"SWAP L", 0, NO_OP, {.func_no_op = swap_l}},         // 0x35
    {"SWAP (HL)", 0, NO_OP, {.func_no_op = swap_hl}},     // 0x36
    {"SWAP A", 0, NO_OP, {.func_no_op = swap_a}},         // 0x37
    {"SRL B", 0, NO_OP, {.func_no_op = srl_b}},           // 0x38
    {"SRL C", 0, NO_OP, {.func_no_op = srl_c}},           // 0x39
    {"SRL D", 0, NO_OP, {.func_no_op = srl_d}},           // 0x3a
    {"SRL E", 0, NO_OP, {.func_no_op = srl_e}},           // 0x3b
    {"SRL H", 0, NO_OP, {.func_no_op = srl_h}},           // 0x3c
    {"SRL L", 0, NO_OP, {.func_no_op = srl_l}},           // 0x3d
    {"SRL (HL)", 0, NO_OP, {.func_no_op = srl_hl}},       // 0x3e
    {"SRL A", 0, NO_OP, {.func_no_op = srl_a}},           // 0x3f
    {"BIT 0, B", 0, NO_OP, {.func_no_op = bit_0_b}},      // 0x40
    {"BIT 0, C", 0, NO_OP, {.func_no_op = bit_0_c}},      // 0x41
    {"BIT 0, D", 0, NO_OP, {.func_no_op = bit_0_d}},      // 0x42
    {"BIT 0, E", 0, NO_OP, {.func_no_op = bit_0_e}},      // 0x43
    {"BIT 0, H", 0, NO_OP, {.func_no_op = bit_0_h}},      // 0x44
    {"BIT 0, L", 0, NO_OP, {.func_no_op = bit_0_l}},      // 0x45
    {"BIT 0, (HL)", 0, NO_OP, {.func_no_op = bit_0_hlp}}, // 0x46
    {"BIT 0, A", 0, NO_OP, {.func_no_op = bit_0_a}},      // 0x47
    {"BIT 1, B", 0, NO_OP, {.func_no_op = bit_1_b}},      // 0x48
    {"BIT 1, C", 0, NO_OP, {.func_no_op = bit_1_c}},      // 0x49
    {"BIT 1, D", 0, NO_OP, {.func_no_op = bit_1_d}},      // 0x4a
    {"BIT 1, E", 0, NO_OP, {.func_no_op = bit_1_e}},      // 0x4b
    {"BIT 1, H", 0, NO_OP, {.func_no_op = bit_1_h}},      // 0x4c
    {"BIT 1, L", 0, NO_OP, {.func_no_op = bit_1_l}},      // 0x4d
    {"BIT 1, (HL)", 0, NO_OP, {.func_no_op = bit_1_hlp}}, // 0x4e
    {"BIT 1, A", 0, NO_OP, {.func_no_op = bit_1_a}},      // 0x4f
    {"BIT 2, B", 0, NO_OP, {.func_no_op = bit_2_b}},      // 0x50
    {"BIT 2, C", 0, NO_OP, {.func_no_op = bit_2_c}},      // 0x51
    {"BIT 2, D", 0, NO_OP, {.func_no_op = bit_2_d}},      // 0x52
    {"BIT 2, E", 0, NO_OP, {.func_no_op = bit_2_e}},      // 0x53
    {"BIT 2, H", 0, NO_OP, {.func_no_op = bit_2_h}},      // 0x54
    {"BIT 2, L", 0, NO_OP, {.func_no_op = bit_2_l}},      // 0x55
    {"BIT 2, (HL)", 0, NO_OP, {.func_no_op = bit_2_hlp}}, // 0x56
    {"BIT 2, A", 0, NO_OP, {.func_no_op = bit_2_a}},      // 0x57
    {"BIT 3, B", 0, NO_OP, {.func_no_op = bit_3_b}},      // 0x58
    {"BIT 3, C", 0, NO_OP, {.func_no_op = bit_3_c}},      // 0x59
    {"BIT 3, D", 0, NO_OP, {.func_no_op = bit_3_d}},      // 0x5a
    {"BIT 3, E", 0, NO_OP, {.func_no_op = bit_3_e}},      // 0x5b
    {"BIT 3, H", 0, NO_OP, {.func_no_op = bit_3_h}},      // 0x5c
    {"BIT 3, L", 0, NO_OP, {.func_no_op = bit_3_l}},      // 0x5d
    {"BIT 3, (HL)", 0, NO_OP, {.func_no_op = bit_3_hlp}}, // 0x5e
    {"BIT 3, A", 0, NO_OP, {.func_no_op = bit_3_a}},      // 0x5f
    {"BIT 4, B", 0, NO_OP, {.func_no_op = bit_4_b}},      // 0x60
    {"BIT 4, C", 0, NO_OP, {.func_no_op = bit_4_c}},      // 0x61
    {"BIT 4, D", 0, NO_OP, {.func_no_op = bit_4_d}},      // 0x62
    {"BIT 4, E", 0, NO_OP, {.func_no_op = bit_4_e}},      // 0x63
    {"BIT 4, H", 0, NO_OP, {.func_no_op = bit_4_h}},      // 0x64
    {"BIT 4, L", 0, NO_OP, {.func_no_op = bit_4_l}},      // 0x65
    {"BIT 4, (HL)", 0, NO_OP, {.func_no_op = bit_4_hlp}}, // 0x66
    {"BIT 4, A", 0, NO_OP, {.func_no_op = bit_4_a}},      // 0x67
    {"BIT 5, B", 0, NO_OP, {.func_no_op = bit_5_b}},      // 0x68
    {"BIT 5, C", 0, NO_OP, {.func_no_op = bit_5_c}},      // 0x69
    {"BIT 5, D", 0, NO_OP, {.func_no_op = bit_5_d}},      // 0x6a
    {"BIT 5, E", 0, NO_OP, {.func_no_op = bit_5_e}},      // 0x6b
    {"BIT 6, H", 0, NO_OP, {.func_no_op = bit_5_h}},      // 0x6c
    {"BIT 6, L", 0, NO_OP, {.func_no_op = bit_5_l}},      // 0x6d
    {"BIT 5, (HL)", 0, NO_OP, {.func_no_op = bit_5_hlp}}, // 0x6e
    {"BIT 5, A", 0, NO_OP, {.func_no_op = bit_5_a}},      // 0x6f
    {"BIT 6, B", 0, NO_OP, {.func_no_op = bit_6_b}},      // 0x70
    {"BIT 6, C", 0, NO_OP, {.func_no_op = bit_6_c}},      // 0x71
    {"BIT 6, D", 0, NO_OP, {.func_no_op = bit_6_d}},      // 0x72
    {"BIT 6, E", 0, NO_OP, {.func_no_op = bit_6_e}},      // 0x73
    {"BIT 6, H", 0, NO_OP, {.func_no_op = bit_6_h}},      // 0x74
    {"BIT 6, L", 0, NO_OP, {.func_no_op = bit_6_l}},      // 0x75
    {"BIT 6, (HL)", 0, NO_OP, {.func_no_op = bit_6_hlp}}, // 0x76
    {"BIT 6, A", 0, NO_OP, {.func_no_op = bit_6_a}},      // 0x77
    {"BIT 7, B", 0, NO_OP, {.func_no_op = bit_7_b}},      // 0x78
    {"BIT 7, C", 0, NO_OP, {.func_no_op = bit_7_c}},      // 0x79
    {"BIT 7, D", 0, NO_OP, {.func_no_op = bit_7_d}},      // 0x7a
    {"BIT 7, E", 0, NO_OP, {.func_no_op = bit_7_e}},      // 0x7b
    {"BIT 7, H", 0, NO_OP, {.func_no_op = bit_7_h}},      // 0x7c
    {"BIT 7, L", 0, NO_OP, {.func_no_op = bit_7_l}},      // 0x7d
    {"BIT 7, (HL)", 0, NO_OP, {.func_no_op = bit_7_hlp}}, // 0x7e
    {"BIT 7, A", 0, NO_OP, {.func_no_op = bit_7_a}},      // 0x7f
    {"RES 0, B", 0, NO_OP, {.func_no_op = res_0_b}},      // 0x80
    {"RES 0, C", 0, NO_OP, {.func_no_op = res_0_c}},      // 0x81
    {"RES 0, D", 0, NO_OP, {.func_no_op = res_0_d}},      // 0x82
    {"RES 0, E", 0, NO_OP, {.func_no_op = res_0_e}},      // 0x83
    {"RES 0, H", 0, NO_OP, {.func_no_op = res_0_h}},      // 0x84
    {"RES 0, L", 0, NO_OP, {.func_no_op = res_0_l}},      // 0x85
    {"RES 0, (HL)", 0, NO_OP, {.func_no_op = res_0_hlp}}, // 0x86
    {"RES 0, A", 0, NO_OP, {.func_no_op = res_0_a}},      // 0x87
    {"RES 1, B", 0, NO_OP, {.func_no_op = res_1_b}},      // 0x88
    {"RES 1, C", 0, NO_OP, {.func_no_op = res_1_c}},      // 0x89
    {"RES 1, D", 0, NO_OP, {.func_no_op = res_1_d}},      // 0x8a
    {"RES 1, E", 0, NO_OP, {.func_no_op = res_1_e}},      // 0x8b
    {"RES 1, H", 0, NO_OP, {.func_no_op = res_1_h}},      // 0x8c
    {"RES 1, L", 0, NO_OP, {.func_no_op = res_1_l}},      // 0x8d
    {"RES 1, (HL)", 0, NO_OP, {.func_no_op = res_1_hlp}}, // 0x8e
    {"RES 1, A", 0, NO_OP, {.func_no_op = res_1_a}},      // 0x8f
    {"RES 2, B", 0, NO_OP, {.func_no_op = res_2_b}},      // 0x90
    {"RES 2, C", 0, NO_OP, {.func_no_op = res_2_c}},      // 0x91
    {"RES 2, D", 0, NO_OP, {.func_no_op = res_2_d}},      // 0x92
    {"RES 2, E", 0, NO_OP, {.func_no_op = res_2_e}},      // 0x93
    {"RES 2, H", 0, NO_OP, {.func_no_op = res_2_h}},      // 0x94
    {"RES 2, L", 0, NO_OP, {.func_no_op = res_2_l}},      // 0x95
    {"RES 2, (HL)", 0, NO_OP, {.func_no_op = res_2_hlp}}, // 0x96
    {"RES 2, A", 0, NO_OP, {.func_no_op = res_2_a}},      // 0x97
    {"RES 3, B", 0, NO_OP, {.func_no_op = res_3_b}},      // 0x98
    {"RES 3, C", 0, NO_OP, {.func_no_op = res_3_c}},      // 0x99
    {"RES 3, D", 0, NO_OP, {.func_no_op = res_3_d}},      // 0x9a
    {"RES 3, E", 0, NO_OP, {.func_no_op = res_3_e}},      // 0x9b
    {"RES 3, H", 0, NO_OP, {.func_no_op = res_3_h}},      // 0x9c
    {"RES 3, L", 0, NO_OP, {.func_no_op = res_3_l}},      // 0x9d
    {"RES 3, (HL)", 0, NO_OP, {.func_no_op = res_3_hlp}}, // 0x9e
    {"RES 3, A", 0, NO_OP, {.func_no_op = res_3_a}},      // 0x9f
    {"RES 4, B", 0, NO_OP, {.func_no_op = res_4_b}},      // 0xa0
    {"RES 4, C", 0, NO_OP, {.func_no_op = res_4_c}},      // 0xa1
    {"RES 4, D", 0, NO_OP, {.func_no_op = res_4_d}},      // 0xa2
    {"RES 4, E", 0, NO_OP, {.func_no_op = res_4_e}},      // 0xa3
    {"RES 4, H", 0, NO_OP, {.func_no_op = res_4_h}},      // 0xa4
    {"RES 4, L", 0, NO_OP, {.func_no_op = res_4_l}},      // 0xa5
    {"RES 4, (HL)", 0, NO_OP, {.func_no_op = res_4_hlp}}, // 0xa6
    {"RES 4, A", 0, NO_OP, {.func_no_op = res_4_a}},      // 0xa7
    {"RES 5, B", 0, NO_OP, {.func_no_op = res_5_b}},      // 0xa8
    {"RES 5, C", 0, NO_OP, {.func_no_op = res_5_c}},      // 0xa9
    {"RES 5, D", 0, NO_OP, {.func_no_op = res_5_d}},      // 0xaa
    {"RES 5, E", 0, NO_OP, {.func_no_op = res_5_e}},      // 0xab
    {"RES 5, H", 0, NO_OP, {.func_no_op = res_5_h}},      // 0xac
    {"RES 5, L", 0, NO_OP, {.func_no_op = res_5_l}},      // 0xad
    {"RES 5, (HL)", 0, NO_OP, {.func_no_op = res_5_hlp}}, // 0xae
    {"RES 5, A", 0, NO_OP, {.func_no_op = res_5_a}},      // 0xaf
    {"RES 6, B", 0, NO_OP, {.func_no_op = res_6_b}},      // 0xb0
    {"RES 6, C", 0, NO_OP, {.func_no_op = res_6_c}},      // 0xb1
    {"RES 6, D", 0, NO_OP, {.func_no_op = res_6_d}},      // 0xb2
    {"RES 6, E", 0, NO_OP, {.func_no_op = res_6_e}},      // 0xb3
    {"RES 6, H", 0, NO_OP, {.func_no_op = res_6_h}},      // 0xb4
    {"RES 6, L", 0, NO_OP, {.func_no_op = res_6_l}},      // 0xb5
    {"RES 6, (HL)", 0, NO_OP, {.func_no_op = res_6_hlp}}, // 0xb6
    {"RES 6, A", 0, NO_OP, {.func_no_op = res_6_a}},      // 0xb7
    {"RES 7, B", 0, NO_OP, {.func_no_op = res_7_b}},      // 0xb8
    {"RES 7, C", 0, NO_OP, {.func_no_op = res_7_c}},      // 0xb9
    {"RES 7, D", 0, NO_OP, {.func_no_op = res_7_d}},      // 0xba
    {"RES 7, E", 0, NO_OP, {.func_no_op = res_7_e}},      // 0xbb
    {"RES 7, H", 0, NO_OP, {.func_no_op = res_7_h}},      // 0xbc
    {"RES 7, L", 0, NO_OP, {.func_no_op = res_7_l}},      // 0xbd
    {"RES 7, (HL)", 0, NO_OP, {.func_no_op = res_7_hlp}}, // 0xbe
    {"RES 7, A", 0, NO_OP, {.func_no_op = res_7_a}},      // 0xbf
    {"SET 0, B", 0, NO_OP, {.func_no_op = set_0_b}},      // 0xc0
    {"SET 0, C", 0, NO_OP, {.func_no_op = set_0_c}},      // 0xc1
    {"SET 0, D", 0, NO_OP, {.func_no_op = set_0_d}},      // 0xc2
    {"SET 0, E", 0, NO_OP, {.func_no_op = set_0_e}},      // 0xc3
    {"SET 0, H", 0, NO_OP, {.func_no_op = set_0_h}},      // 0xc4
    {"SET 0, L", 0, NO_OP, {.func_no_op = set_0_l}},      // 0xc5
    {"SET 0, (HL)", 0, NO_OP, {.func_no_op = set_0_hlp}}, // 0xc6
    {"SET 0, A", 0, NO_OP, {.func_no_op = set_0_a}},      // 0xc7
    {"SET 1, B", 0, NO_OP, {.func_no_op = set_1_b}},      // 0xc8
    {"SET 1, C", 0, NO_OP, {.func_no_op = set_1_c}},      // 0xc9
    {"SET 1, D", 0, NO_OP, {.func_no_op = set_1_d}},      // 0xca
    {"SET 1, E", 0, NO_OP, {.func_no_op = set_1_e}},      // 0xcb
    {"SET 1, H", 0, NO_OP, {.func_no_op = set_1_h}},      // 0xcc
    {"SET 1, L", 0, NO_OP, {.func_no_op = set_1_l}},      // 0xcd
    {"SET 1, (HL)", 0, NO_OP, {.func_no_op = set_1_hlp}}, // 0xce
    {"SET 1, A", 0, NO_OP, {.func_no_op = set_1_a}},      // 0xcf
    {"SET 2, B", 0, NO_OP, {.func_no_op = set_2_b}},      // 0xd0
    {"SET 2, C", 0, NO_OP, {.func_no_op = set_2_c}},      // 0xd1
    {"SET 2, D", 0, NO_OP, {.func_no_op = set_2_d}},      // 0xd2
    {"SET 2, E", 0, NO_OP, {.func_no_op = set_2_e}},      // 0xd3
    {"SET 2, H", 0, NO_OP, {.func_no_op = set_2_h}},      // 0xd4
    {"SET 2, L", 0, NO_OP, {.func_no_op = set_2_l}},      // 0xd5
    {"SET 2, (HL)", 0, NO_OP, {.func_no_op = set_2_hlp}}, // 0xd6
    {"SET 2, A", 0, NO_OP, {.func_no_op = set_2_a}},      // 0xd7
    {"SET 3, B", 0, NO_OP, {.func_no_op = set_3_b}},      // 0xd8
    {"SET 3, C", 0, NO_OP, {.func_no_op = set_3_c}},      // 0xd9
    {"SET 3, D", 0, NO_OP, {.func_no_op = set_3_d}},      // 0xda
    {"SET 3, E", 0, NO_OP, {.func_no_op = set_3_e}},      // 0xdb
    {"SET 3, H", 0, NO_OP, {.func_no_op = set_3_h}},      // 0xdc
    {"SET 3, L", 0, NO_OP, {.func_no_op = set_3_l}},      // 0xdd
    {"SET 3, (HL)", 0, NO_OP, {.func_no_op = set_3_hlp}}, // 0xde
    {"SET 3, A", 0, NO_OP, {.func_no_op = set_3_a}},      // 0xdf
    {"SET 4, B", 0, NO_OP, {.func_no_op = set_4_b}},      // 0xe0
    {"SET 4, C", 0, NO_OP, {.func_no_op = set_4_c}},      // 0xe1
    {"SET 4, D", 0, NO_OP, {.func_no_op = set_4_d}},      // 0xe2
    {"SET 4, E", 0, NO_OP, {.func_no_op = set_4_e}},      // 0xe3
    {"SET 4, H", 0, NO_OP, {.func_no_op = set_4_h}},      // 0xe4
    {"SET 4, L", 0, NO_OP, {.func_no_op = set_4_l}},      // 0xe5
    {"SET 4, (HL)", 0, NO_OP, {.func_no_op = set_4_hlp}}, // 0xe6
    {"SET 4, A", 0, NO_OP, {.func_no_op = set_4_a}},      // 0xe7
    {"SET 5, B", 0, NO_OP, {.func_no_op = set_5_b}},      // 0xe8
    {"SET 5, C", 0, NO_OP, {.func_no_op = set_5_c}},      // 0xe9
    {"SET 5, D", 0, NO_OP, {.func_no_op = set_5_d}},      // 0xea
    {"SET 5, E", 0, NO_OP, {.func_no_op = set_5_e}},      // 0xeb
    {"SET 5, H", 0, NO_OP, {.func_no_op = set_5_h}},      // 0xec
    {"SET 5, L", 0, NO_OP, {.func_no_op = set_5_l}},      // 0xed
    {"SET 5, (HL)", 0, NO_OP, {.func_no_op = set_5_hlp}}, // 0xee
    {"SET 5, A", 0, NO_OP, {.func_no_op = set_5_a}},      // 0xef
    {"SET 6, B", 0, NO_OP, {.func_no_op = set_6_b}},      // 0xf0
    {"SET 6, C", 0, NO_OP, {.func_no_op = set_6_c}},      // 0xf1
    {"SET 6, D", 0, NO_OP, {.func_no_op = set_6_d}},      // 0xf2
    {"SET 6, E", 0, NO_OP, {.func_no_op = set_6_e}},      // 0xf3
    {"SET 6, H", 0, NO_OP, {.func_no_op = set_6_h}},      // 0xf4
    {"SET 6, L", 0, NO_OP, {.func_no_op = set_6_l}},      // 0xf5
    {"SET 6, (HL)", 0, NO_OP, {.func_no_op = set_6_hlp}}, // 0xf6
    {"SET 6, A", 0, NO_OP, {.func_no_op = set_6_a}},      // 0xf7
    {"SET 7, B", 0, NO_OP, {.func_no_op = set_7_b}},      // 0xf8
    {"SET 7, C", 0, NO_OP, {.func_no_op = set_7_c}},      // 0xf9
    {"SET 7, D", 0, NO_OP, {.func_no_op = set_7_d}},      // 0xfa
    {"SET 7, E", 0, NO_OP, {.func_no_op = set_7_e}},      // 0xfb
    {"SET 7, H", 0, NO_OP, {.func_no_op = set_7_h}},      // 0xfc
    {"SET 7, L", 0, NO_OP, {.func_no_op = set_7_l}},      // 0xfd
    {"SET 7, (HL)", 0, NO_OP, {.func_no_op = set_7_hlp}}, // 0xfe
    {"SET 7, A", 0, NO_OP, {.func_no_op = set_7_a}},      // 0xff
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

const char *cartridgeTypeStrings[] = {
    [ROM_ONLY] = "ROM ONLY",
    [ROM_MBC1] = "ROM MBC1",
    [ROM_MBC1_RAM] = "ROM MBC1 + RAM",
    [ROM_MBC1_RAM_BATT] = "ROM MBC1 + RAM + Battery",
    [ROM_MBC2] = "ROM MBC2",
    [ROM_MBC2_BATT] = "ROM MBC2 + Battery",
    [ROM_RAM] = "ROM + RAM",
    [ROM_RAM_BATT] = "ROM + RAM + Battery",
    [ROM_MMM01] = "ROM MMM01",
    [ROM_MMM01_SRAM] = "ROM MMM01 + SRAM",
    [ROM_MMM01_SRAM_BATT] = "ROM MMM01 + SRAM + Battery",
    [ROM_MBC3_TIMER_BATT] = "ROM MBC3 + Timer + Battery",
    [ROM_MBC3_TIMER_RAM_BATT] = "ROM MBC3 + Timer + RAM + Battery",
    [ROM_MBC3] = "ROM MBC3",
    [ROM_MBC3_RAM] = "ROM MBC3 + RAM",
    [ROM_MBC3_RAM_BATT] = "ROM MBC3 + RAM + Battery",
    [ROM_MBC5] = "ROM MBC5",
    [ROM_MBC5_RAM] = "ROM MBC5 + RAM",
    [ROM_MBC5_RAM_BATT] = "ROM MBC5 + RAM + Battery",
    [ROM_MBC5_RUMBLE] = "ROM MBC5 + Rumble",
    [ROM_MBC5_RUMBLE_SRAM] = "ROM MBC5 + Rumble + SRAM",
    [ROM_MBC5_RUMBLE_SRAM_BATT] = "ROM MBC5 + Rumble + SRAM + Battery",
    [POCKET_CAMERA] = "Pocket Camera",
    [BANDAI_TAMA5] = "Bandai TAMA5",
    [HUDSON_HUC3] = "Hudson HuC-3",
    [HUDSON_HUC1] = "Hudson HuC-1"};
