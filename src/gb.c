#include "gb.h"

/// Uses a macro in gb.h to map strings to hex
const char *cartridgeTypeStrings[] = {
#define X(name, value, string) [value] = string,
    CARTRIDGE_TYPE_LIST
#undef X
};

const OPS ops[256] = {
    {"NOP", 0, NO_OP, {.func_no_op = nop}},                    // 0x00
    {"LD BC, 0x%04X", 2, U16_OP, {.func_u16_op = ld_bc_nn}},   // 0x01
    {"LD (BC), A", 1, U8_OP, {.func_u8_op = ld_bc_a}},         // 0x02
    {"INC BC", 0, NO_OP, {.func_no_op = inc_bc}},              // 0x03
    {"INC B", 0, NO_OP, {.func_no_op = inc_b}},                // 0x04
    {"DEC B", 0, NO_OP, {.func_no_op = dec_b}},                // 0x05
    {"LD B, 0x%02X", 1, U8_OP, {.func_u8_op = ln_b_n}},        // 0x06
    {"RLCA", 0, NO_OP, {.func_no_op = rlca}},                  // 0x07
    {"LD (0x%04X), SP", 2, U16_OP, {.func_u16_op = ld_nn_sp}}, // 0x08
    {"ADD HL, BC", 0, NO_OP, {.func_no_op = add_hl_bc}},       // 0x09
    {"LD A, (BC)", 1, U8_OP, {.func_u8_op = ld_a_bc}},         // 0x0a
    {"DEC BC", 0, NO_OP, {.func_no_op = dec_bc}},              // 0x0b
    {"INC C", 0, NO_OP, {.func_no_op = inc_c}},                // 0x0c
    {"DEC C", 0, NO_OP, {.func_no_op = dec_c}},                // 0x0d
    {"LD C, 0x%02X", 1, U8_OP, {.func_u8_op = ln_c_n}},        // 0x0e
    {"RRCA", 0, NO_OP, {.func_no_op = rrca}},                  // 0x0f
    {"STOP", 0, NO_OP, {.func_no_op = stop}},                  // 0x10
    {"LD DE, 0x%04X", 2, U16_OP, {.func_u16_op = ld_de_nn}},   // 0x11
    {"LD (DE), A", 1, U8_OP, {.func_u8_op = ld_de_a}},         // 0x12
    {"INC DE", 0, NO_OP, {.func_no_op = inc_de}},              // 0x13
    {"INC D", 0, NO_OP, {.func_no_op = inc_d}},                // 0x14
    {"DEC D", 0, NO_OP, {.func_no_op = dec_d}},                // 0x15
    {"LD D, 0x%02X", 1, U8_OP, {.func_u8_op = ln_d_n}},        // 0x16
    {"RLA", 0, NO_OP, {.func_no_op = rla}},                    // 0x17
    {"JR 0x%02X", 1, U8_OP, {.func_u8_op = jr_n}},             // 0x18
    {"ADD HL, DE", 0, NO_OP, {.func_no_op = add_hl_de}},       // 0x19
    {"LD A, (DE)", 1, U8_OP, {.func_u8_op = ld_a_de}},         // 0x1a
    {"DEC DE", 0, NO_OP, {.func_no_op = dec_de}},              // 0x1b
    {"INC E", 0, NO_OP, {.func_no_op = inc_e}},                // 0x1c
    {"DEC E", 0, NO_OP, {.func_no_op = dec_e}},                // 0x1d
    {"LD E, 0x%02X", 1, U8_OP, {.func_u8_op = ln_e_n}},        // 0x1e
    {"RRA", 0, NO_OP, {.func_no_op = rra}},                    // 0x1f
    {"JR NZ, 0x%02X", 1, U8_OP, {.func_u8_op = jr_nz_n}},      // 0x20
    {"LD HL, 0x%04X", 2, U16_OP, {.func_u16_op = ld_hl_nn}},   // 0x21
    {"LDI (HL), A", 0, NO_OP, {.func_no_op = ld_hli_a}},       // 0x22
    {"INC HL", 0, NO_OP, {.func_no_op = inc_hl_16}},           // 0x23
    {"INC H", 0, NO_OP, {.func_no_op = inc_h}},                // 0x24
    {"DEC H", 0, NO_OP, {.func_no_op = dec_h}},                // 0x25
    {"LD H, 0x%02X", 1, U8_OP, {.func_u8_op = ln_h_n}},        // 0x26
    {"DAA", 0, NO_OP, {.func_no_op = daa}},                    // 0x27
    {"JR Z, 0x%02X", 1, U8_OP, {.func_u8_op = jr_z_n}},        // 0x28
    {"ADD HL, HL", 0, NO_OP, {.func_no_op = add_hl_hl}},       // 0x29
    {"LDI A, (HL)", 0, NO_OP, {.func_no_op = ld_a_hli}},       // 0x2a
    {"DEC HL", 0, NO_OP, {.func_no_op = dec_hl_16}},           // 0x2b
    {"INC L", 0, NO_OP, {.func_no_op = inc_l}},                // 0x2c
    {"DEC L", 0, NO_OP, {.func_no_op = dec_l}},                // 0x2d
    {"LD L, 0x%02X", 1, U8_OP, {.func_u8_op = ln_l_n}},        // 0x2e
    {"CPL", 0, NO_OP, {.func_no_op = cpl}},                    // 0x2f
    {"JR NC, 0x%02X", 1, U8_OP, {.func_u8_op = jr_nc_n}},      // 0x30
    {"LD SP, 0x%04X", 2, U16_OP, {.func_u16_op = ld_sp_nn}},   // 0x31
    {"LDD (HL), A", 0, NO_OP, {.func_no_op = ld_hld_a}},       // 0x32
    {"INC SP", 0, NO_OP, {.func_no_op = inc_sp}},              // 0x33
    {"INC (HL)", 0, NO_OP, {.func_no_op = inc_hl}},            // 0x34
    {"DEC (HL)", 0, NO_OP, {.func_no_op = dec_hl}},            // 0x35
    {"LD (HL), 0x%02X", 1, U8_OP, {.func_u8_op = ld_hl_n}},    // 0x36
    {"SCF", 0, NO_OP, {.func_no_op = scf}},                    // 0x37
    {"JR C, 0x%02X", 1, U8_OP, {.func_u8_op = jr_c_n}},        // 0x38
    {"ADD HL, SP", 0, NO_OP, {.func_no_op = add_hl_sp}},       // 0x39
    {"LDD A, (HL)", 0, NO_OP, {.func_no_op = ld_a_hld}},       // 0x3a
    {"DEC SP", 0, NO_OP, {.func_no_op = dec_sp}},              // 0x3b
    {"INC A", 0, NO_OP, {.func_no_op = inc_a}},                // 0x3c
    {"DEC A", 0, NO_OP, {.func_no_op = dec_a}},                // 0x3d
    {"LD A, 0x%02X", 1, U8_OP, {.func_u8_op = ld_a_n}},        // 0x3e
    {"CCF", 0, NO_OP, {.func_no_op = ccf}},                    // 0x3f
    {"LD B, B", 1, U8_OP, {.func_u8_op = ld_b_b}},             // 0x40
    {"LD B, C", 1, U8_OP, {.func_u8_op = ld_b_c}},             // 0x41
    {"LD B, D", 1, U8_OP, {.func_u8_op = ld_b_d}},             // 0x42
    {"LD B, E", 1, U8_OP, {.func_u8_op = ld_b_e}},             // 0x43
    {"LD B, H", 1, U8_OP, {.func_u8_op = ld_b_h}},             // 0x44
    {"LD B, L", 1, U8_OP, {.func_u8_op = ld_b_l}},             // 0x45
    {"LD B, (HL)", 1, U8_OP, {.func_u8_op = ld_b_hl}},         // 0x46
    {"LD B, A", 1, U8_OP, {.func_u8_op = ld_b_a}},             // 0x47
    {"LD C, B", 1, U8_OP, {.func_u8_op = ld_c_b}},             // 0x48
    {"LD C, C", 1, U8_OP, {.func_u8_op = ld_c_c}},             // 0x49
    {"LD C, D", 1, U8_OP, {.func_u8_op = ld_c_d}},             // 0x4a
    {"LD C, E", 1, U8_OP, {.func_u8_op = ld_c_e}},             // 0x4b
    {"LD C, H", 1, U8_OP, {.func_u8_op = ld_c_h}},             // 0x4c
    {"LD C, L", 1, U8_OP, {.func_u8_op = ld_c_l}},             // 0x4d
    {"LD C, (HL)", 1, U8_OP, {.func_u8_op = ld_c_hl}},         // 0x4e
    {"LD C, A", 1, U8_OP, {.func_u8_op = ld_c_a}},             // 0x4f
    {"LD D, B", 1, U8_OP, {.func_u8_op = ld_d_b}},             // 0x50
    {"LD D, C", 1, U8_OP, {.func_u8_op = ld_d_c}},             // 0x51
    {"LD D, D", 1, U8_OP, {.func_u8_op = ld_d_d}},             // 0x52
    {"LD D, E", 1, U8_OP, {.func_u8_op = ld_d_e}},             // 0x53
    {"LD D, H", 1, U8_OP, {.func_u8_op = ld_d_h}},             // 0x54
    {"LD D, L", 1, U8_OP, {.func_u8_op = ld_d_l}},             // 0x55
    {"LD D, (HL)", 1, U8_OP, {.func_u8_op = ld_d_hl}},         // 0x56
    {"LD D, A", 1, U8_OP, {.func_u8_op = ld_d_a}},             // 0x57
    {"LD E, B", 1, U8_OP, {.func_u8_op = ld_e_b}},             // 0x58
    {"LD E, C", 1, U8_OP, {.func_u8_op = ld_e_c}},             // 0x59
    {"LD E, D", 1, U8_OP, {.func_u8_op = ld_e_d}},             // 0x5a
    {"LD E, E", 1, U8_OP, {.func_u8_op = ld_e_e}},             // 0x5b
    {"LD E, H", 1, U8_OP, {.func_u8_op = ld_e_h}},             // 0x5c
    {"LD E, L", 1, U8_OP, {.func_u8_op = ld_e_l}},             // 0x5d
    {"LD E, (HL)", 1, U8_OP, {.func_u8_op = ld_e_hl}},         // 0x5e
    {"LD E, A", 1, U8_OP, {.func_u8_op = ld_e_a}},             // 0x5f
    {"LD H, B", 1, U8_OP, {.func_u8_op = ld_h_b}},             // 0x60
    {"LD H, C", 1, U8_OP, {.func_u8_op = ld_h_c}},             // 0x61
    {"LD H, D", 1, U8_OP, {.func_u8_op = ld_h_d}},             // 0x62
    {"LD H, E", 1, U8_OP, {.func_u8_op = ld_h_e}},             // 0x63
    {"LD H, H", 1, U8_OP, {.func_u8_op = ld_h_h}},             // 0x64
    {"LD H, L", 1, U8_OP, {.func_u8_op = ld_h_l}},             // 0x65
    {"LD H, (HL)", 1, U8_OP, {.func_u8_op = ld_h_hl}},         // 0x66
    {"LD H, A", 1, U8_OP, {.func_u8_op = ld_h_a}},             // 0x67
    {"LD L, B", 1, U8_OP, {.func_u8_op = ld_l_b}},             // 0x68
    {"LD L, C", 1, U8_OP, {.func_u8_op = ld_l_c}},             // 0x69
    {"LD L, D", 1, U8_OP, {.func_u8_op = ld_l_d}},             // 0x6a
    {"LD L, E", 1, U8_OP, {.func_u8_op = ld_l_e}},             // 0x6b
    {"LD L, H", 1, U8_OP, {.func_u8_op = ld_l_h}},             // 0x6c
    {"LD L, L", 1, U8_OP, {.func_u8_op = ld_l_l}},             // 0x6d
    {"LD L, (HL)", 1, U8_OP, {.func_u8_op = ld_l_hl}},         // 0x6e
    {"LD L, A", 1, U8_OP, {.func_u8_op = ld_l_a}},             // 0x6f
    {"LD (HL), B", 1, U8_OP, {.func_u8_op = ld_hl_b}},         // 0x70
    {"LD (HL), C", 1, U8_OP, {.func_u8_op = ld_hl_c}},         // 0x71
    {"LD (HL), D", 1, U8_OP, {.func_u8_op = ld_hl_d}},         // 0x72
    {"LD (HL), E", 1, U8_OP, {.func_u8_op = ld_hl_e}},         // 0x73
    {"LD (HL), H", 1, U8_OP, {.func_u8_op = ld_hl_h}},         // 0x74
    {"LD (HL), L", 1, U8_OP, {.func_u8_op = ld_hl_l}},         // 0x75
    {"HALT", 0, NO_OP, {.func_no_op = halt}},                  // 0x76
    {"LD (HL), A", 1, U8_OP, {.func_u8_op = ld_hl_a}},         // 0x77
    {"LD A, B", 1, U8_OP, {.func_u8_op = ld_a_b}},             // 0x78
    {"LD A, C", 1, U8_OP, {.func_u8_op = ld_a_c}},             // 0x79
    {"LD A, D", 1, U8_OP, {.func_u8_op = ld_a_d}},             // 0x7a
    {"LD A, E", 1, U8_OP, {.func_u8_op = ld_a_e}},             // 0x7b
    {"LD A, H", 1, U8_OP, {.func_u8_op = ld_a_h}},             // 0x7c
    {"LD A, L", 1, U8_OP, {.func_u8_op = ld_a_l}},             // 0x7d
    {"LD A, (HL)", 1, U8_OP, {.func_u8_op = ld_a_hl}},         // 0x7e
    {"LD A, A", 1, U8_OP, {.func_u8_op = ld_a_a}},             // 0x7f
    {"ADD A, B", 0, NO_OP, {.func_no_op = add_a_b}},           // 0x80
    {"ADD A, C", 0, NO_OP, {.func_no_op = add_a_c}},           // 0x81
    {"ADD A, D", 0, NO_OP, {.func_no_op = add_a_d}},           // 0x82
    {"ADD A, E", 0, NO_OP, {.func_no_op = add_a_e}},           // 0x83
    {"ADD A, H", 0, NO_OP, {.func_no_op = add_a_h}},           // 0x84
    {"ADD A, L", 0, NO_OP, {.func_no_op = add_a_l}},           // 0x85
    {"ADD A, (HL)", 0, NO_OP, {.func_no_op = add_a_hl}},       // 0x86
    {"ADD A, A", 0, NO_OP, {.func_no_op = add_a_a}},           // 0x87
    {"ADC A, B", 0, NO_OP, {.func_no_op = adc_a_b}},           // 0x88
    {"ADC A, C", 0, NO_OP, {.func_no_op = adc_a_c}},           // 0x89
    {"ADC A, D", 0, NO_OP, {.func_no_op = adc_a_d}},           // 0x8a
    {"ADC A, E", 0, NO_OP, {.func_no_op = adc_a_e}},           // 0x8b
    {"ADC A, H", 0, NO_OP, {.func_no_op = adc_a_h}},           // 0x8c
    {"ADC A, L", 0, NO_OP, {.func_no_op = adc_a_l}},           // 0x8d
    {"ADC A, (HL)", 0, NO_OP, {.func_no_op = adc_a_hl}},       // 0x8e
    {"ADC A, A", 0, NO_OP, {.func_no_op = adc_a_a}},           // 0x8f
    {"SUB B", 0, NO_OP, {.func_no_op = sub_b}},                // 0x90
    {"SUB C", 0, NO_OP, {.func_no_op = sub_c}},                // 0x91
    {"SUB D", 0, NO_OP, {.func_no_op = sub_d}},                // 0x92
    {"SUB E", 0, NO_OP, {.func_no_op = sub_e}},                // 0x93
    {"SUB H", 0, NO_OP, {.func_no_op = sub_h}},                // 0x94
    {"SUB L", 0, NO_OP, {.func_no_op = sub_l}},                // 0x95
    {"SUB (HL)", 0, NO_OP, {.func_no_op = sub_hl}},            // 0x96
    {"SUB A", 0, NO_OP, {.func_no_op = sub_a}},                // 0x97
    {"SBC A, B", 1, U8_OP, {.func_u8_op = sbc_a_b}},           // 0x98
    {"SBC A, C", 1, U8_OP, {.func_u8_op = sbc_a_c}},           // 0x99
    {"SBC A, D", 1, U8_OP, {.func_u8_op = sbc_a_d}},           // 0x9a
    {"SBC A, E", 1, U8_OP, {.func_u8_op = sbc_a_e}},           // 0x9b
    {"SBC A, H", 1, U8_OP, {.func_u8_op = sbc_a_h}},           // 0x9c
    {"SBC A, L", 1, U8_OP, {.func_u8_op = sbc_a_l}},           // 0x9d
    {"SBC A, (HL)", 1, U8_OP, {.func_u8_op = sbc_a_hl}},       // 0x9e
    {"SBC A, A", 1, U8_OP, {.func_u8_op = sbc_a_a}},           // 0x9f
    {"AND B", 0, NO_OP, {.func_no_op = and_b}},                // 0xa0
    {"AND C", 0, NO_OP, {.func_no_op = and_c}},                // 0xa1
    {"AND D", 0, NO_OP, {.func_no_op = and_d}},                // 0xa2
    {"AND E", 0, NO_OP, {.func_no_op = and_e}},                // 0xa3
    {"AND H", 0, NO_OP, {.func_no_op = and_h}},                // 0xa4
    {"AND L", 0, NO_OP, {.func_no_op = and_l}},                // 0xa5
    {"AND (HL)", 0, NO_OP, {.func_no_op = and_hl}},            // 0xa6
    {"AND A", 0, NO_OP, {.func_no_op = and_a}},                // 0xa7
    {"XOR B", 0, NO_OP, {.func_no_op = xor_b}},                // 0xa8
    {"XOR C", 0, NO_OP, {.func_no_op = xor_c}},                // 0xa9
    {"XOR D", 0, NO_OP, {.func_no_op = xor_d}},                // 0xaa
    {"XOR E", 0, NO_OP, {.func_no_op = xor_e}},                // 0xab
    {"XOR H", 0, NO_OP, {.func_no_op = xor_h}},                // 0xac
    {"XOR L", 0, NO_OP, {.func_no_op = xor_l}},                // 0xad
    {"XOR (HL)", 0, NO_OP, {.func_no_op = xor_hl}},            // 0xae
    {"XOR A", 0, NO_OP, {.func_no_op = xor_a}},                // 0xaf
    {"OR B", 0, NO_OP, {.func_no_op = or_b}},                  // 0xb0
    {"OR C", 0, NO_OP, {.func_no_op = or_c}},                  // 0xb1
    {"OR D", 0, NO_OP, {.func_no_op = or_d}},                  // 0xb2
    {"OR E", 0, NO_OP, {.func_no_op = or_e}},                  // 0xb3
    {"OR H", 0, NO_OP, {.func_no_op = or_h}},                  // 0xb4
    {"OR L", 0, NO_OP, {.func_no_op = or_l}},                  // 0xb5
    {"OR (HL)", 0, NO_OP, {.func_no_op = or_hl}},              // 0xb6
    {"OR A", 0, NO_OP, {.func_no_op = or_a}},                  // 0xb7
    {"CP B", 0, NO_OP, {.func_no_op = cp_b}},                  // 0xb8
    {"CP C", 0, NO_OP, {.func_no_op = cp_c}},                  // 0xb9
    {"CP D", 0, NO_OP, {.func_no_op = cp_d}},                  // 0xba
    {"CP E", 0, NO_OP, {.func_no_op = cp_e}},                  // 0xbb
    {"CP H", 0, NO_OP, {.func_no_op = cp_h}},                  // 0xbc
    {"CP L", 0, NO_OP, {.func_no_op = cp_l}},                  // 0xbd
    {"CP (HL)", 0, NO_OP, {.func_no_op = cp_hl}},              // 0xbe
    {"CP A", 0, NO_OP, {.func_no_op = cp_a}},                  // 0xbf
    {"RET NZ", 0, NO_OP, {.func_no_op = ret_nz}},              // 0xc0
    {"POP BC", 0, NO_OP, {.func_no_op = pop_bc}},              // 0xc1
    {"JP NZ, nn", 2, U16_OP, {.func_u16_op = jp_nz_nn}},       // 0xc2
    {"JP nn", 2, U16_OP, {.func_u16_op = jp_nn}},              // 0xc3
    {"CALL NZ, nn", 2, U16_OP, {.func_u16_op = call_nz_nn}},   // 0xc4
    {"PUSH BC", 0, NO_OP, {.func_no_op = push_bc}},            // 0xc5
    {"ADD A, n", 1, U8_OP, {.func_u8_op = add_a_n}},           // 0xc6
    {"RST 0", 0, NO_OP, {.func_no_op = rst_00}},               // 0xc7
    {"RET Z", 0, NO_OP, {.func_no_op = ret_z}},                // 0xc8
    {"RET", 0, NO_OP, {.func_no_op = ret}},                    // 0xc9
    {"JP Z, nn", 2, U16_OP, {.func_u16_op = jp_z_nn}},         // 0xca
    {"CALL Z, nn", 2, U16_OP, {.func_u16_op = call_z_nn}},     // 0xcc
    {"CALL nn", 2, U16_OP, {.func_u16_op = call_nn}},          // 0xcd
    {"ADC A, n", 1, U8_OP, {.func_u8_op = adc_a_n}},           // 0xce
    {"RST 8", 0, NO_OP, {.func_no_op = rst_08}},               // 0xcf
    {"RET NC", 0, NO_OP, {.func_no_op = ret_nc}},              // 0xd0
    {"POP DE", 0, NO_OP, {.func_no_op = pop_de}},              // 0xd1
    {"JP NC, nn", 2, U16_OP, {.func_u16_op = jp_nc_nn}},       // 0xd2
    {"UNKNOWN", 0, NO_OP, {.func_no_op = nop}},                // 0xd3
    {"CALL NC, nn", 2, U16_OP, {.func_u16_op = call_nc_nn}},   // 0xd4
    {"PUSH DE", 0, NO_OP, {.func_no_op = push_de}},            // 0xd5
    {"SUB n", 1, U8_OP, {.func_u8_op = sub_n}},                // 0xd6
    {"RST 10", 0, NO_OP, {.func_no_op = rst_10}},              // 0xd7
    {"RET C", 0, NO_OP, {.func_no_op = ret_c}},                // 0xd8
    {"RETI", 0, NO_OP, {.func_no_op = reti}},                  // 0xd9
    {"JP C, nn", 2, U16_OP, {.func_u16_op = jp_c_nn}},         // 0xda
    {"UNKNOWN", 0, NO_OP, {.func_no_op = nop}},                // 0xdb
    {"CALL C, nn", 2, U16_OP, {.func_u16_op = call_c_nn}},     // 0xdc
    {"SBC A, n", 1, U8_OP, {.func_u8_op = sbc_a_n}},           // 0xde
    {"RST 18", 0, NO_OP, {.func_no_op = rst_18}},              // 0xdf
    {"LD (nn), SP", 2, U16_OP, {.func_u16_op = ld_nn_sp}},     // 0xe8
    {"LD HL, (nn)", 2, U16_OP, {.func_u16_op = ld_hl_nn}},     // 0xea
    {"LD (nn), A", 2, U16_OP, {.func_u16_op = ld_nn_a}},       // 0xed
    {"XOR n", 1, U8_OP, {.func_u8_op = xor_n}},                // 0xee
    {"RST 28", 0, NO_OP, {.func_no_op = rst_28}},              // 0xef
    {"LD SP, HL", 0, NO_OP, {.func_no_op = ld_sp_hl}},         // 0xf9
    {"LD HL, SP+n", 1, U8_OP, {.func_u8_op = ld_hl_sp_n}},     // 0xfa
    {"LD A, (nn)", 2, U16_OP, {.func_u16_op = ld_a_nn}},       // 0xfc
    {"DI", 0, NO_OP, {.func_no_op = di}},                      // 0xfd
    {"CP n", 1, U8_OP, {.func_u8_op = cp_n}},                  // 0xfe
    {"RST 38", 0, NO_OP, {.func_no_op = rst_38}},              // 0xff
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
              .data = malloc(0xFFFF), // TODO: actual size
              .size = 0xFFFF,
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
  vm->mem.io_registers = vm->mem.data + 0xFF00;
  vm->mem.hram = vm->mem.data + 0xFF80;
  vm->mem.interrupt_enable = vm->mem.data + 0xFFFF;

  memset(vm->mem.data, 0xFF, 0xFFFF);

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
  vm->r.af = 0x0001; // Dependent on console type!!
  vm->r.bc = 0x0013;
  vm->r.de = 0x00D8;
  vm->r.hl = 0x014D;
  vm->r.pc = 0x100;
  vm->r.sp = 0xFFFE;

  // DMG
  vm->mem.io_registers[0x00] = 0xCF; // P1
  vm->mem.io_registers[0x01] = 0x00; // SB
  vm->mem.io_registers[0x02] = 0x7E; // SC
  vm->mem.io_registers[0x04] = 0xAB; // DIV
  vm->mem.io_registers[0x05] = 0x00; // TIMA
  vm->mem.io_registers[0x06] = 0x00; // TMA
  vm->mem.io_registers[0x07] = 0x00; // TAC
  vm->mem.io_registers[0x10] = 0x80; // NR10
  vm->mem.io_registers[0x11] = 0xBF; // NR11
  vm->mem.io_registers[0x12] = 0xF3; // NR12
  vm->mem.io_registers[0x14] = 0xBF; // NR14
  vm->mem.io_registers[0x16] = 0x3F; // NR21
  vm->mem.io_registers[0x17] = 0x00; // NR22
  vm->mem.io_registers[0x19] = 0xBF; // NR24
  vm->mem.io_registers[0x1A] = 0x7F; // NR30
  vm->mem.io_registers[0x1B] = 0xFF; // NR31
  vm->mem.io_registers[0x1C] = 0x9F; // NR32
  vm->mem.io_registers[0x1E] = 0xBF; // NR33
  vm->mem.io_registers[0x20] = 0xFF; // NR41
  vm->mem.io_registers[0x21] = 0x00; // NR42
  vm->mem.io_registers[0x22] = 0x00; // NR43
  vm->mem.io_registers[0x23] = 0xBF; // NR30
  vm->mem.io_registers[0x24] = 0x77; // NR50
  vm->mem.io_registers[0x25] = 0xF3; // NR51
  vm->mem.io_registers[0x26] = 0xF1; // NR52
  vm->mem.io_registers[0x40] = 0x91; // LCDC
  vm->mem.io_registers[0x42] = 0x00; // SCY
  vm->mem.io_registers[0x43] = 0x00; // SCX
  vm->mem.io_registers[0x45] = 0x00; // LYC
  vm->mem.io_registers[0x47] = 0xFC; // BGP
  vm->mem.io_registers[0x48] = 0xFF; // OBP0
  vm->mem.io_registers[0x49] = 0xFF; // OBP1
  vm->mem.io_registers[0x4A] = 0x00; // WY
  vm->mem.io_registers[0x4B] = 0x00; // WX
  *vm->mem.interrupt_enable = 0x00;  // IE

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
}

void step(GB *vm) {
  if (vm->flag.halt) {
    return;
  }

  u8 opcode = vm->cart.data[vm->r.pc];
  const OPS *instr = &ops[opcode];

  printf("OP: %s\n", instr->debug_str);

  switch (instr->type) {
  case NO_OP:
    if (instr->func.func_no_op) {
      instr->func.func_no_op(vm);
    }
    break;
  case U8_OP:
    if (instr->func.func_u8_op) {
      u8 operand = read_u8(vm, vm->r.pc + 1);
      instr->func.func_u8_op(vm, operand);
    }
    break;
  case U16_OP:
    if (instr->func.func_u16_op) {
      u16 operand = read_u16(vm, vm->r.pc + 1);
      instr->func.func_u16_op(vm, operand);
    }
    break;
  }

  vm->r.pc += instr->length + 1;

  // DI and EI have a delay.
  if (vm->flag.interrupt_disable_pending) {
    vm->flag.interrupt_master_enable = false;
    vm->flag.interrupt_disable_pending = false;
  }
  if (vm->flag.interrupt_enable_pending) {
    vm->flag.interrupt_master_enable = true;
    vm->flag.interrupt_enable_pending = false;
  }
}

u8 read_u8(GB *vm, u16 addr) { return vm->mem.data[addr]; }

u16 read_u16(GB *vm, u16 addr) {
  return ((u16)vm->mem.data[addr + 1] << 8) | vm->mem.data[addr];
}

void write_u8(GB *vm, u16 addr, u8 value) { vm->mem.data[addr] = value; }

void write_u16(GB *vm, u16 addr, u16 value) {
  vm->mem.data[addr] = value & 0xFF;
  vm->mem.data[addr + 1] = (value >> 8) & 0xFF;
}

void _reg_set_flag(GB *vm, u8 z, u8 n, u8 h, u8 c) {
  // 0 remains 0, other values become 1
  z = !!z;
  n = !!n;
  h = !!h;
  c = !!c;

  vm->r.f &= 0x0F;

  vm->r.f |= (z << 7) | (n << 6) | (h << 5) | (c << 4);
}
