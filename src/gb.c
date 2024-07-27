#include "gb.h"

/// Uses a macro in gb.h to map strings to hex
const char *cartridgeTypeStrings[] = {
#define X(name, value, string) [value] = string,
    CARTRIDGE_TYPE_LIST
#undef X
};

const OPS ops[256] = {
    {"NOP", 0, NO_OP, {.func_no_op = nop}},
    {"LD BC, 0x%04X", 2, NO_OP, {.func_no_op = nop}},
    {"LD (BC), A", 0, NO_OP, {.func_no_op = nop}},
    {"INC BC", 0, NO_OP, {.func_no_op = nop}},                  // 0x03
    {"INC B", 1, U8_OP, {.func_u8_op = inc_b}},                 // 0x2c
    {"DEC B", 0, NO_OP, {.func_no_op = nop}},                   // 0x05
    {"LD B, 0x%02X", 1, NO_OP, {.func_no_op = nop}},            // 0x06
    {"RLCA", 0, NO_OP, {.func_no_op = nop}},                    // 0x07
    {"LD (0x%04X), SP", 2, NO_OP, {.func_no_op = nop}},         // 0x08
    {"ADD HL, BC", 0, NO_OP, {.func_no_op = nop}},              // 0x09
    {"LD A, (BC)", 0, NO_OP, {.func_no_op = nop}},              // 0x0a
    {"DEC BC", 0, NO_OP, {.func_no_op = nop}},                  // 0x0b
    {"INC C", 1, U8_OP, {.func_u8_op = inc_c}},                 // 0x2c
    {"DEC C", 0, NO_OP, {.func_no_op = nop}},                   // 0x0d
    {"LD C, 0x%02X", 1, NO_OP, {.func_no_op = nop}},            // 0x0e
    {"RRCA", 0, NO_OP, {.func_no_op = nop}},                    // 0x0f
    {"STOP", 1, NO_OP, {.func_no_op = nop}},                    // 0x10
    {"LD DE, 0x%04X", 2, NO_OP, {.func_no_op = nop}},           // 0x11
    {"LD (DE), A", 0, NO_OP, {.func_no_op = nop}},              // 0x12
    {"INC DE", 0, NO_OP, {.func_no_op = nop}},                  // 0x13
    {"INC D", 1, U8_OP, {.func_u8_op = inc_d}},                 // 0x2c
    {"DEC D", 0, NO_OP, {.func_no_op = nop}},                   // 0x15
    {"LD D, 0x%02X", 1, NO_OP, {.func_no_op = nop}},            // 0x16
    {"RLA", 0, NO_OP, {.func_no_op = nop}},                     // 0x17
    {"JR 0x%02X", 1, NO_OP, {.func_no_op = nop}},               // 0x18
    {"ADD HL, DE", 0, NO_OP, {.func_no_op = nop}},              // 0x19
    {"LD A, (DE)", 0, NO_OP, {.func_no_op = nop}},              // 0x1a
    {"DEC DE", 0, NO_OP, {.func_no_op = nop}},                  // 0x1b
    {"INC E", 1, U8_OP, {.func_u8_op = inc_e}},                 // 0x2c
    {"DEC E", 0, NO_OP, {.func_no_op = nop}},                   // 0x1d
    {"LD E, 0x%02X", 1, NO_OP, {.func_no_op = nop}},            // 0x1e
    {"RRA", 0, NO_OP, {.func_no_op = nop}},                     // 0x1f
    {"JR NZ, 0x%02X", 1, NO_OP, {.func_no_op = nop}},           // 0x20
    {"LD HL, 0x%04X", 2, NO_OP, {.func_no_op = nop}},           // 0x21
    {"LDI (HL), A", 0, NO_OP, {.func_no_op = nop}},             // 0x22
    {"INC HL", 0, NO_OP, {.func_no_op = nop}},                  // 0x23
    {"INC H", 1, U8_OP, {.func_u8_op = inc_h}},                 // 0x2c
    {"DEC H", 0, NO_OP, {.func_no_op = nop}},                   // 0x25
    {"LD H, 0x%02X", 1, NO_OP, {.func_no_op = nop}},            // 0x26
    {"DAA", 0, NO_OP, {.func_no_op = nop}},                     // 0x27
    {"JR Z, 0x%02X", 1, NO_OP, {.func_no_op = nop}},            // 0x28
    {"ADD HL, HL", 0, NO_OP, {.func_no_op = nop}},              // 0x29
    {"LDI A, (HL)", 0, NO_OP, {.func_no_op = nop}},             // 0x2a
    {"DEC HL", 0, NO_OP, {.func_no_op = nop}},                  // 0x2b
    {"INC L", 1, U8_OP, {.func_u8_op = inc_l}},                 // 0x2c
    {"DEC L", 0, NO_OP, {.func_no_op = nop}},                   // 0x2d
    {"LD L, 0x%02X", 1, NO_OP, {.func_no_op = nop}},            // 0x2e
    {"CPL", 0, NO_OP, {.func_no_op = nop}},                     // 0x2f
    {"JR NC, 0x%02X", 1, NO_OP, {.func_no_op = nop}},           // 0x30
    {"LD SP, 0x%04X", 2, NO_OP, {.func_no_op = nop}},           // 0x31
    {"LDD (HL), A", 0, NO_OP, {.func_no_op = nop}},             // 0x32
    {"INC SP", 0, NO_OP, {.func_no_op = nop}},                  // 0x33
    {"INC (HL)", 1, U8_OP, {.func_u8_op = inc_hl}},             // 0x2c
    {"DEC (HL)", 0, NO_OP, {.func_no_op = nop}},                // 0x35
    {"LD (HL), 0x%02X", 1, NO_OP, {.func_no_op = nop}},         // 0x36
    {"SCF", 0, NO_OP, {.func_no_op = nop}},                     // 0x37
    {"JR C, 0x%02X", 1, NO_OP, {.func_no_op = nop}},            // 0x38
    {"ADD HL, SP", 0, NO_OP, {.func_no_op = nop}},              // 0x39
    {"LDD A, (HL)", 0, NO_OP, {.func_no_op = nop}},             // 0x3a
    {"DEC SP", 0, NO_OP, {.func_no_op = nop}},                  // 0x3b
    {"INC A", 1, U8_OP, {.func_u8_op = inc_a}},                 // 0x2c
    {"DEC A", 0, NO_OP, {.func_no_op = nop}},                   // 0x3d
    {"LD A, 0x%02X", 1, NO_OP, {.func_no_op = nop}},            // 0x3e
    {"CCF", 0, NO_OP, {.func_no_op = nop}},                     // 0x3f
    {"LD B, B", 0, NO_OP, {.func_no_op = nop}},                 // 0x40
    {"LD B, C", 0, NO_OP, {.func_no_op = nop}},                 // 0x41
    {"LD B, D", 0, NO_OP, {.func_no_op = nop}},                 // 0x42
    {"LD B, E", 0, NO_OP, {.func_no_op = nop}},                 // 0x43
    {"LD B, H", 0, NO_OP, {.func_no_op = nop}},                 // 0x44
    {"LD B, L", 0, NO_OP, {.func_no_op = nop}},                 // 0x45
    {"LD B, (HL)", 0, NO_OP, {.func_no_op = nop}},              // 0x46
    {"LD B, A", 0, NO_OP, {.func_no_op = nop}},                 // 0x47
    {"LD C, B", 0, NO_OP, {.func_no_op = nop}},                 // 0x48
    {"LD C, C", 0, NO_OP, {.func_no_op = nop}},                 // 0x49
    {"LD C, D", 0, NO_OP, {.func_no_op = nop}},                 // 0x4a
    {"LD C, E", 0, NO_OP, {.func_no_op = nop}},                 // 0x4b
    {"LD C, H", 0, NO_OP, {.func_no_op = nop}},                 // 0x4c
    {"LD C, L", 0, NO_OP, {.func_no_op = nop}},                 // 0x4d
    {"LD C, (HL)", 0, NO_OP, {.func_no_op = nop}},              // 0x4e
    {"LD C, A", 0, NO_OP, {.func_no_op = nop}},                 // 0x4f
    {"LD D, B", 0, NO_OP, {.func_no_op = nop}},                 // 0x50
    {"LD D, C", 0, NO_OP, {.func_no_op = nop}},                 // 0x51
    {"LD D, D", 0, NO_OP, {.func_no_op = nop}},                 // 0x52
    {"LD D, E", 0, NO_OP, {.func_no_op = nop}},                 // 0x53
    {"LD D, H", 0, NO_OP, {.func_no_op = nop}},                 // 0x54
    {"LD D, L", 0, NO_OP, {.func_no_op = nop}},                 // 0x55
    {"LD D, (HL)", 0, NO_OP, {.func_no_op = nop}},              // 0x56
    {"LD D, A", 0, NO_OP, {.func_no_op = nop}},                 // 0x57
    {"LD E, B", 0, NO_OP, {.func_no_op = nop}},                 // 0x58
    {"LD E, C", 0, NO_OP, {.func_no_op = nop}},                 // 0x59
    {"LD E, D", 0, NO_OP, {.func_no_op = nop}},                 // 0x5a
    {"LD E, E", 0, NO_OP, {.func_no_op = nop}},                 // 0x5b
    {"LD E, H", 0, NO_OP, {.func_no_op = nop}},                 // 0x5c
    {"LD E, L", 0, NO_OP, {.func_no_op = nop}},                 // 0x5d
    {"LD E, (HL)", 0, NO_OP, {.func_no_op = nop}},              // 0x5e
    {"LD E, A", 0, NO_OP, {.func_no_op = nop}},                 // 0x5f
    {"LD H, B", 0, NO_OP, {.func_no_op = nop}},                 // 0x60
    {"LD H, C", 0, NO_OP, {.func_no_op = nop}},                 // 0x61
    {"LD H, D", 0, NO_OP, {.func_no_op = nop}},                 // 0x62
    {"LD H, E", 0, NO_OP, {.func_no_op = nop}},                 // 0x63
    {"LD H, H", 0, NO_OP, {.func_no_op = nop}},                 // 0x64
    {"LD H, L", 0, NO_OP, {.func_no_op = nop}},                 // 0x65
    {"LD H, (HL)", 0, NO_OP, {.func_no_op = nop}},              // 0x66
    {"LD H, A", 0, NO_OP, {.func_no_op = nop}},                 // 0x67
    {"LD L, B", 0, NO_OP, {.func_no_op = nop}},                 // 0x68
    {"LD L, C", 0, NO_OP, {.func_no_op = nop}},                 // 0x69
    {"LD L, D", 0, NO_OP, {.func_no_op = nop}},                 // 0x6a
    {"LD L, E", 0, NO_OP, {.func_no_op = nop}},                 // 0x6b
    {"LD L, H", 0, NO_OP, {.func_no_op = nop}},                 // 0x6c
    {"LD L, L", 0, NO_OP, {.func_no_op = nop}},                 // 0x6d
    {"LD L, (HL)", 0, NO_OP, {.func_no_op = nop}},              // 0x6e
    {"LD L, A", 0, NO_OP, {.func_no_op = nop}},                 // 0x6f
    {"LD (HL), B", 0, NO_OP, {.func_no_op = nop}},              // 0x70
    {"LD (HL), C", 0, NO_OP, {.func_no_op = nop}},              // 0x71
    {"LD (HL), D", 0, NO_OP, {.func_no_op = nop}},              // 0x72
    {"LD (HL), E", 0, NO_OP, {.func_no_op = nop}},              // 0x73
    {"LD (HL), H", 0, NO_OP, {.func_no_op = nop}},              // 0x74
    {"LD (HL), L", 0, NO_OP, {.func_no_op = nop}},              // 0x75
    {"HALT", 0, NO_OP, {.func_no_op = nop}},                    // 0x76
    {"LD (HL), A", 0, NO_OP, {.func_no_op = nop}},              // 0x77
    {"LD A, B", 0, NO_OP, {.func_no_op = nop}},                 // 0x78
    {"LD A, C", 0, NO_OP, {.func_no_op = nop}},                 // 0x79
    {"LD A, D", 0, NO_OP, {.func_no_op = nop}},                 // 0x7a
    {"LD A, E", 0, NO_OP, {.func_no_op = nop}},                 // 0x7b
    {"LD A, H", 0, NO_OP, {.func_no_op = nop}},                 // 0x7c
    {"LD A, L", 0, NO_OP, {.func_no_op = nop}},                 // 0x7d
    {"LD A, (HL)", 0, NO_OP, {.func_no_op = nop}},              // 0x7e
    {"LD A, A", 0, NO_OP, {.func_no_op = nop}},                 // 0x7f
    {"ADD A, B", 0, NO_OP, {.func_no_op = nop}},                // 0x80
    {"ADD A, C", 0, NO_OP, {.func_no_op = nop}},                // 0x81
    {"ADD A, D", 0, NO_OP, {.func_no_op = nop}},                // 0x82
    {"ADD A, E", 0, NO_OP, {.func_no_op = nop}},                // 0x83
    {"ADD A, H", 0, NO_OP, {.func_no_op = nop}},                // 0x84
    {"ADD A, L", 0, NO_OP, {.func_no_op = nop}},                // 0x85
    {"ADD A, (HL)", 0, NO_OP, {.func_no_op = nop}},             // 0x86
    {"ADD A", 0, NO_OP, {.func_no_op = nop}},                   // 0x87
    {"ADC B", 0, NO_OP, {.func_no_op = nop}},                   // 0x88
    {"ADC C", 0, NO_OP, {.func_no_op = nop}},                   // 0x89
    {"ADC D", 0, NO_OP, {.func_no_op = nop}},                   // 0x8a
    {"ADC E", 0, NO_OP, {.func_no_op = nop}},                   // 0x8b
    {"ADC H", 0, NO_OP, {.func_no_op = nop}},                   // 0x8c
    {"ADC L", 0, NO_OP, {.func_no_op = nop}},                   // 0x8d
    {"ADC (HL)", 0, NO_OP, {.func_no_op = nop}},                // 0x8e
    {"ADC A", 0, NO_OP, {.func_no_op = nop}},                   // 0x8f
    {"SUB B", 0, NO_OP, {.func_no_op = nop}},                   // 0x90
    {"SUB C", 0, NO_OP, {.func_no_op = nop}},                   // 0x91
    {"SUB D", 0, NO_OP, {.func_no_op = nop}},                   // 0x92
    {"SUB E", 0, NO_OP, {.func_no_op = nop}},                   // 0x93
    {"SUB H", 0, NO_OP, {.func_no_op = nop}},                   // 0x94
    {"SUB L", 0, NO_OP, {.func_no_op = nop}},                   // 0x95
    {"SUB (HL)", 0, NO_OP, {.func_no_op = nop}},                // 0x96
    {"SUB A", 0, NO_OP, {.func_no_op = nop}},                   // 0x97
    {"SBC B", 0, NO_OP, {.func_no_op = nop}},                   // 0x98
    {"SBC C", 0, NO_OP, {.func_no_op = nop}},                   // 0x99
    {"SBC D", 0, NO_OP, {.func_no_op = nop}},                   // 0x9a
    {"SBC E", 0, NO_OP, {.func_no_op = nop}},                   // 0x9b
    {"SBC H", 0, NO_OP, {.func_no_op = nop}},                   // 0x9c
    {"SBC L", 0, NO_OP, {.func_no_op = nop}},                   // 0x9d
    {"SBC (HL)", 0, NO_OP, {.func_no_op = nop}},                // 0x9e
    {"SBC A", 0, NO_OP, {.func_no_op = nop}},                   // 0x9f
    {"AND B", 0, NO_OP, {.func_no_op = nop}},                   // 0xa0
    {"AND C", 0, NO_OP, {.func_no_op = nop}},                   // 0xa1
    {"AND D", 0, NO_OP, {.func_no_op = nop}},                   // 0xa2
    {"AND E", 0, NO_OP, {.func_no_op = nop}},                   // 0xa3
    {"AND H", 0, NO_OP, {.func_no_op = nop}},                   // 0xa4
    {"AND L", 0, NO_OP, {.func_no_op = nop}},                   // 0xa5
    {"AND (HL)", 0, NO_OP, {.func_no_op = nop}},                // 0xa6
    {"AND A", 0, NO_OP, {.func_no_op = nop}},                   // 0xa7
    {"XOR B", 0, NO_OP, {.func_no_op = nop}},                   // 0xa8
    {"XOR C", 0, NO_OP, {.func_no_op = nop}},                   // 0xa9
    {"XOR D", 0, NO_OP, {.func_no_op = nop}},                   // 0xaa
    {"XOR E", 0, NO_OP, {.func_no_op = nop}},                   // 0xab
    {"XOR H", 0, NO_OP, {.func_no_op = nop}},                   // 0xac
    {"XOR L", 0, NO_OP, {.func_no_op = nop}},                   // 0xad
    {"XOR (HL)", 0, NO_OP, {.func_no_op = nop}},                // 0xae
    {"XOR A", 0, NO_OP, {.func_no_op = nop}},                   // 0xaf
    {"OR B", 0, NO_OP, {.func_no_op = nop}},                    // 0xb0
    {"OR C", 0, NO_OP, {.func_no_op = nop}},                    // 0xb1
    {"OR D", 0, NO_OP, {.func_no_op = nop}},                    // 0xb2
    {"OR E", 0, NO_OP, {.func_no_op = nop}},                    // 0xb3
    {"OR H", 0, NO_OP, {.func_no_op = nop}},                    // 0xb4
    {"OR L", 0, NO_OP, {.func_no_op = nop}},                    // 0xb5
    {"OR (HL)", 0, NO_OP, {.func_no_op = nop}},                 // 0xb6
    {"OR A", 0, NO_OP, {.func_no_op = nop}},                    // 0xb7
    {"CP B", 0, NO_OP, {.func_no_op = nop}},                    // 0xb8
    {"CP C", 0, NO_OP, {.func_no_op = nop}},                    // 0xb9
    {"CP D", 0, NO_OP, {.func_no_op = nop}},                    // 0xba
    {"CP E", 0, NO_OP, {.func_no_op = nop}},                    // 0xbb
    {"CP H", 0, NO_OP, {.func_no_op = nop}},                    // 0xbc
    {"CP L", 0, NO_OP, {.func_no_op = nop}},                    // 0xbd
    {"CP (HL)", 0, NO_OP, {.func_no_op = nop}},                 // 0xbe
    {"CP A", 0, NO_OP, {.func_no_op = nop}},                    // 0xbf
    {"RET NZ", 0, NO_OP, {.func_no_op = nop}},                  // 0xc0
    {"POP BC", 0, NO_OP, {.func_no_op = nop}},                  // 0xc1
    {"JP NZ, 0x%04X", 2, NO_OP, {.func_no_op = nop}},           // 0xc2
    {"JP 0x%04X", 2, U16_OP, {.func_u16_op = jp_nn}},           // 0xc3
    {"CALL NZ, 0x%04X", 2, NO_OP, {.func_no_op = nop}},         // 0xc4
    {"PUSH BC", 0, NO_OP, {.func_no_op = nop}},                 // 0xc5
    {"ADD A, 0x%02X", 1, NO_OP, {.func_no_op = nop}},           // 0xc6
    {"RST 0x00", 0, NO_OP, {.func_no_op = nop}},                // 0xc7
    {"RET Z", 0, NO_OP, {.func_no_op = nop}},                   // 0xc8
    {"RET", 0, NO_OP, {.func_no_op = nop}},                     // 0xc9
    {"JP Z, 0x%04X", 2, NO_OP, {.func_no_op = nop}},            // 0xca
    {"CB %02X", 1, NO_OP, {.func_no_op = nop}},                 // 0xcb
    {"CALL Z, 0x%04X", 2, NO_OP, {.func_no_op = nop}},          // 0xcc
    {"CALL 0x%04X", 2, NO_OP, {.func_no_op = nop}},             // 0xcd
    {"ADC 0x%02X", 1, NO_OP, {.func_no_op = nop}},              // 0xce
    {"RST 0x08", 0, NO_OP, {.func_no_op = nop}},                // 0xcf
    {"RET NC", 0, NO_OP, {.func_no_op = nop}},                  // 0xd0
    {"POP DE", 0, NO_OP, {.func_no_op = nop}},                  // 0xd1
    {"JP NC, 0x%04X", 2, NO_OP, {.func_no_op = nop}},           // 0xd2
    {"UNKNOWN", 0, NO_OP, {.func_no_op = nop}},                 // 0xd3
    {"CALL NC, 0x%04X", 2, NO_OP, {.func_no_op = nop}},         // 0xd4
    {"PUSH DE", 0, NO_OP, {.func_no_op = nop}},                 // 0xd5
    {"SUB 0x%02X", 1, NO_OP, {.func_no_op = nop}},              // 0xd6
    {"RST 0x10", 0, NO_OP, {.func_no_op = nop}},                // 0xd7
    {"RET C", 0, NO_OP, {.func_no_op = nop}},                   // 0xd8
    {"RETI", 0, NO_OP, {.func_no_op = nop}},                    // 0xd9
    {"JP C, 0x%04X", 2, NO_OP, {.func_no_op = nop}},            // 0xda
    {"UNKNOWN", 0, NO_OP, {.func_no_op = nop}},                 // 0xdb
    {"CALL C, 0x%04X", 2, NO_OP, {.func_no_op = nop}},          // 0xdc
    {"UNKNOWN", 0, NO_OP, {.func_no_op = nop}},                 // 0xdd
    {"SBC 0x%02X", 1, NO_OP, {.func_no_op = nop}},              // 0xde
    {"RST 0x18", 0, NO_OP, {.func_no_op = nop}},                // 0xdf
    {"LD (0xFF00 + 0x%02X), A", 1, NO_OP, {.func_no_op = nop}}, // 0xe0
    {"POP HL", 0, NO_OP, {.func_no_op = nop}},                  // 0xe1
    {"LD (0xFF00 + C), A", 0, NO_OP, {.func_no_op = nop}},      // 0xe2
    {"UNKNOWN", 0, NO_OP, {.func_no_op = nop}},                 // 0xe3
    {"UNKNOWN", 0, NO_OP, {.func_no_op = nop}},                 // 0xe4
    {"PUSH HL", 0, NO_OP, {.func_no_op = nop}},                 // 0xe5
    {"AND 0x%02X", 1, NO_OP, {.func_no_op = nop}},              // 0xe6
    {"RST 0x20", 0, NO_OP, {.func_no_op = nop}},                // 0xe7
    {"ADD SP,0x%02X", 1, NO_OP, {.func_no_op = nop}},           // 0xe8
    {"JP HL", 0, NO_OP, {.func_no_op = nop}},                   // 0xe9
    {"LD (0x%04X), A", 2, NO_OP, {.func_no_op = nop}},          // 0xea
    {"UNKNOWN", 0, NO_OP, {.func_no_op = nop}},                 // 0xeb
    {"UNKNOWN", 0, NO_OP, {.func_no_op = nop}},                 // 0xec
    {"UNKNOWN", 0, NO_OP, {.func_no_op = nop}},                 // 0xed
    {"XOR 0x%02X", 1, NO_OP, {.func_no_op = nop}},              // 0xee
    {"RST 0x28", 0, NO_OP, {.func_no_op = nop}},                // 0xef
    {"LD A, (0xFF00 + 0x%02X)", 1, NO_OP, {.func_no_op = nop}}, // 0xf0
    {"POP AF", 0, NO_OP, {.func_no_op = nop}},                  // 0xf1
    {"LD A, (0xFF00 + C)", 0, NO_OP, {.func_no_op = nop}},      // 0xf2
    {"DI", 0, NO_OP, {.func_no_op = nop}},                      // 0xf3
    {"UNKNOWN", 0, NO_OP, {.func_no_op = nop}},                 // 0xf4
    {"PUSH AF", 0, NO_OP, {.func_no_op = nop}},                 // 0xf5
    {"OR 0x%02X", 1, NO_OP, {.func_no_op = nop}},               // 0xf6
    {"RST 0x30", 0, NO_OP, {.func_no_op = nop}},                // 0xf7
    {"LD HL, SP+0x%02X", 1, NO_OP, {.func_no_op = nop}},        // 0xf8
    {"LD SP, HL", 0, NO_OP, {.func_no_op = nop}},               // 0xf9
    {"LD A, (0x%04X)", 2, NO_OP, {.func_no_op = nop}},          // 0xfa
    {"EI", 0, NO_OP, {.func_no_op = nop}},                      // 0xfb
    {"UNKNOWN", 0, NO_OP, {.func_no_op = nop}},                 // 0xfc
    {"UNKNOWN", 0, NO_OP, {.func_no_op = nop}},                 // 0xfd
    {"CP 0x%02X", 1, NO_OP, {.func_no_op = nop}},               // 0xfe
    {"RST 0x38", 0, NO_OP, {.func_no_op = nop}},                // 0xff
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
}

u8 read_u8(GB *vm, u16 addr) { return 0; }

u16 read_u16(GB *vm, u16 addr) { return 0; }

void _reg_set_flag(GB *vm, u8 z, u8 n, u8 h, u8 c) {
  // 0 remains 0, other values become 1
  z = !!z;
  n = !!n;
  h = !!h;
  c = !!c;

  vm->r.f &= 0x0F; // Clear flags

  vm->r.f |= (z << 7) | (n << 6) | (h << 5) | (c << 4);
}
