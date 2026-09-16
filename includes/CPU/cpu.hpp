#pragma once
#include <cstdint>
#include <stdint.h>

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
#define GET_ZERO_SUBTRACT ((cpu_registers.f & ~(1UL << 6)) << 6)

#define SET_FLAG_HALF_CARRY(value)                                             \
  cpu_registers.f = (cpu_registers.f & ~(1UL << 5)) | ((value) << 5)
#define GET_ZERO_HALF_CARRY ((cpu_registers.f & ~(1UL << 5)) << 5)

#define SET_FLAG_CARRY(value)                                                  \
  cpu_registers.f = (cpu_registers.f & ~(1UL << 4)) | ((value) << 4)
#define GET_ZERO_CARRY ((cpu_registers.f & ~(1UL << 4)) << 4)

struct gb_cpu_registers {

  union {
    uint16_t af;
    struct {
      uint8_t f; // (High half) flags
      uint8_t a; // (Low half) accumulator
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

  uint16_t sp; // Stack pointer
  uint16_t pc; // Program counter (holds the adress of the next instruction)
};

typedef void (*cpu_execute_op)();

extern gb_cpu_registers cpu_registers;

void cpu_reset();
void cpu_fetch();
bool cpu_execute();

// Instructions implementation
void cpu_nop(); // 0x00
// JP a16
void cpu_jp_a16(); // 0xC3
// XOR A, A
void cpu_xor_a_a(); // 0xAF
// LD HL, a16
void cpu_ld_hl_a16(); // 0x21
// LD C, a8
void cpu_ld_c_a8(); // 0x0E
// LD B, a8
void cpu_ld_b_a8(); // 0x06
// LD (HL-), A = LDD (HL), A
void cpu_ldd_hl_a(); // 0x32
// DEC B
void cpu_dec_b(); // 0x05
