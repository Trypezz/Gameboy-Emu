#pragma once
#include <cstdint>

//
// Flags:
//   Z = Zero Flag -> Bit 7
//   N = Substract Flag -> Bit 6
//   H = Half Carry Flag -> Bit 5
//   C = Carry Flag -> Bit 4
//

#define SET_FLAG_ZERO(value)                                                   \
  cpu_registers.f = (cpu_registers.f & ~(1UL << 7)) | ((value) << 7)
#define GET_ZERO_FLAG ((cpu_registers.f & ~(1UL << 7)) << 7)

#define SET_FLAG_SUBTRACT(value)                                               \
  cpu_registers.f = (cpu_registers.f & ~(1UL << 6)) | ((value) << 6)
#define GET_FLAG_SUBTRACT ((cpu_registers.f & ~(1UL << 6)) << 6)

#define SET_FLAG_HALF_CARRY(value)                                             \
  cpu_registers.f = (cpu_registers.f & ~(1UL << 5)) | ((value) << 5)
#define GET_FLAG_HALF_CARRY ((cpu_registers.f & ~(1UL << 5)) << 5)

#define SET_FLAG_CARRY(value)                                                  \
  cpu_registers.f = (cpu_registers.f & ~(1UL << 4)) | ((value) << 4)
#define GET_FLAG_CARRY ((cpu_registers.f & ~(1UL << 4)) << 4)

struct gb_cpu_registers {

  union {
    uint16_t af;
    struct {
      uint8_t f; // (Low half) flags
      uint8_t a; // (High half) accumulator
    };
  };

  union {
    uint16_t bc;
    struct {
      uint8_t c;
      uint8_t b;
    };
  };

  union {
    uint16_t de;
    struct {
      uint8_t e;
      uint8_t d;
    };
  };

  union {
    uint16_t hl;
    struct {
      uint8_t l;
      uint8_t h;
    };
  };

  union {
    uint16_t sp; // Stack pointer
    struct {
      uint8_t p;
      uint8_t s;
    };
  };

  uint16_t pc; // Program counter (holds the adress of the next instruction)
};

typedef void (*cpu_execute_op)();

extern gb_cpu_registers cpu_registers;

void cpu_reset();
void cpu_fetch();
bool cpu_execute();

// Instructions implementation
void cpu_nop(); // 0x00

// LD (HL-), A = LDD (HL), A
void cpu_ldd_hl_a(); // 0x32
// JP a16
void cpu_jp_a16(); // 0xC3
// XOR A, A
void cpu_xor_a_a(); // 0xAF

/*
 * 8-Bit Loads
 */

// LD A, n8
void cpu_ld_a_n8(); // 0x3E
// LD B, n8
void cpu_ld_b_n8(); // 0x06
// LD C, n8
void cpu_ld_c_n8(); // 0x0E
// LD D, n8
void cpu_ld_d_n8(); // 0x16
// LD E, n8
void cpu_ld_e_n8(); // 0x1E
// LD H, n8
void cpu_ld_h_n8(); // 0x26
// LD L, n8
void cpu_ld_l_n8(); // 0x2E

/*
 * 8-Bit Increments
 */

// INC A
void cpu_inc_a(); // 0x3C
// INC B
void cpu_inc_b(); // 0x04
// INC C
void cpu_inc_c(); // 0x0C
// INC D
void cpu_inc_d(); // 0x14
// INC E
void cpu_inc_e(); // 0x1C
// INC H
void cpu_inc_h(); // 0x24
// INC L
void cpu_inc_l(); // 0x2C

/*
 * 8-Bit Decrements
 */

// DEC A
void cpu_dec_a(); // 0x3D
// DEC B
void cpu_dec_b(); // 0x05
// DEC C
void cpu_dec_c(); // 0x0D
// DEC D
void cpu_dec_d(); // 0x15
// DEC E
void cpu_dec_e(); // 0x1D
// DEC H
void cpu_dec_h(); // 0x25
// DEC L
void cpu_dec_l(); // 0x2D

/*
 * 8-Bit indirect loads from register
 */

// LD (BC), A
void cpu_ld_bc_a(); // 0x02
// LD (DE), A
void cpu_ld_de_a(); // 0x12
// LD (HL), A
void cpu_ld_hl_a(); // 0x77
// LD (HL), B
void cpu_ld_hl_b(); // 0x70
// LD (HL), C
void cpu_ld_hl_c(); // 0x71
// LD (HL), D
void cpu_ld_hl_d(); // 0x72
// LD (HL), E
void cpu_ld_hl_e(); // 0x73
// LD (HL), H
void cpu_ld_hl_h(); // 0x74
// LD (HL), L
void cpu_ld_hl_l(); // 0x75

/*
 * 8-Bit indirect loads to register
 */

// LD A, (BC)
void cpu_ld_a_bc(); // 0x0A
// LD A, (DE)
void cpu_ld_a_de(); // 0x1A
// LD A, (HL)
void cpu_ld_a_hl(); // 0x7E
// LD B, (HL)
void cpu_ld_b_hl(); // 0x46
// LD C, (HL)
void cpu_ld_c_hl(); // 0x4E
// LD D, (HL)
void cpu_ld_d_hl(); // 0x56
// LD E, (HL)
void cpu_ld_e_hl(); // 0x5E
// LD H, (HL)
void cpu_ld_h_hl(); // 0x66
// LD L, (HL)
void cpu_ld_l_hl(); // 0x6E

/*
 * 8-Bit ADD to Accumulator
 */

// ADD A, B
void cpu_add_b_to_a(); // 0x80
// ADD A, C
void cpu_add_c_to_a(); // 0x81
// ADD A, D
void cpu_add_d_to_a(); // 0x82
// ADD A, E
void cpu_add_e_to_a(); // 0x83
// ADD A, H
void cpu_add_h_to_a(); // 0x84
// ADD A, L
void cpu_add_l_to_a(); // 0x85

/*
 * 16-Bit Loads
 */

// LD HL, n16
void cpu_ld_hl_n16(); // 0x21
// LD BC, n16
void cpu_ld_bc_n16(); // 0x01
// LD DE, n16
void cpu_ld_de_n16(); // 0x11
// LD SP, n16
void cpu_ld_sp_n16(); // 0x31

/*
 * 16-Bit Increments
 */

// INC BC
void cpu_inc_bc(); // 0x03
// INC DE
void cpu_inc_de(); // 0x13
// INC HL
void cpu_inc_hl(); // 0x23
// INC SP
void cpu_inc_sp(); // 0x33

/*
 * 16-Bit Decrements
 */

// DEC BC
void cpu_dec_bc(); // 0x0B
// DEC DE
void cpu_dec_de(); // 0x1B
// DEC HL
void cpu_dec_hl(); // 0x2B
// DEC SP
void cpu_dec_sp(); // 0x3B

/*
 * 16-Bit ADD to HL
 */

// ADD HL, BC
void cpu_add_bc_to_hl(); // 0x09
// ADD HL, DE
void cpu_add_de_to_hl(); // 0x19
// ADD HL, SP
void cpu_add_sp_to_hl(); // 0x39
