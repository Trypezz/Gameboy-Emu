#include "CPU/cpu.hpp"
#include "CPU/cpu_instructions.hpp"
#include "CPU/cpu_routines.hpp"
#include "MemoryBus/memory_bus.hpp"
#include "emualtor_core.hpp"
#include <cstdint>
#include <cstdio>

gb_cpu_registers cpu_registers;
uint8_t cpu_current_op_code = 0;
uint32_t cpu_instruction_counter = 0;
cpu_execute_op cpu_current_instruction_execute = nullptr;

void cpu_reset() {
  // After executing the boot rom, the registers should have these values
  cpu_registers.af = 0x01B0;
  cpu_registers.bc = 0x0013;
  cpu_registers.de = 0x00D8;
  cpu_registers.hl = 0x014D;
  cpu_registers.sp = 0xFFFE;
  cpu_registers.pc = 0x0100;
}

void cpu_fetch() {
  cpu_current_op_code = memory_bus_read(cpu_registers.pc++);
  const gb_cpu_instruction &instruction = instructions[cpu_current_op_code];
  cpu_current_instruction_execute = instruction.execute;
}

bool cpu_execute() {
  if (!cpu_current_instruction_execute) {
    const gb_cpu_instruction &instruction = instructions[cpu_current_op_code];
    const uint8_t pchi = ((cpu_registers.pc - 1) & 0xFF00) >> 8;
    const uint8_t pclo = ((cpu_registers.pc - 1) & 0xFF);

    printf("Unknown instruction 0x%.2X at: PC:%.2X%.2X (%s), count %i\n",
           cpu_current_op_code, pchi, pclo, instruction.disassembly,
           cpu_instruction_counter);

    return false;
  }
  // This actually executes the instruction
  ((cpu_execute_op)cpu_current_instruction_execute)();
  return true;
}

// Istructions implementation
// NOP = No operation
void cpu_nop() { core_advance_cpu_clocks(4); }
// JP a16
void cpu_jp_a16() {
  core_advance_cpu_clocks(4);
  uint32_t temp = memory_bus_read(cpu_registers.pc++);
  cpu_registers.pc &= 0xFFFF;
  core_advance_cpu_clocks(4);
  temp |= ((uint32_t)memory_bus_read(cpu_registers.pc++)) << 8;
  cpu_registers.pc = temp;
  core_advance_cpu_clocks(4);
}
// XOR A, A
void cpu_xor_a_a() {
  cpu_registers.a = 0;
  SET_FLAG_SUBTRACT(0);
  SET_FLAG_CARRY(0);
  SET_FLAG_HALF_CARRY(0);
  SET_FLAG_ZERO(1);
  core_advance_cpu_clocks(4);
}

// LD (HL-), A = LDD (HL), A
void cpu_ldd_hl_a() {
  core_advance_cpu_clocks(4);
  memory_bus_write(cpu_registers.hl, cpu_registers.a);
  cpu_registers.hl = (cpu_registers.hl - 1) & 0xFFFF;
  core_advance_cpu_clocks(4);
}

/*
 * 8-Bit Loads
 */

// LD A, n8
void cpu_ld_a_n8() { cpu_routine_ld_8(cpu_registers.a); }
// LD B, n8
void cpu_ld_b_n8() { cpu_routine_ld_8(cpu_registers.b); }
// LD C, n8
void cpu_ld_c_n8() { cpu_routine_ld_8(cpu_registers.c); }
// LD D, n8
void cpu_ld_d_n8() { cpu_routine_ld_8(cpu_registers.d); }
// LD E, n8
void cpu_ld_e_n8() { cpu_routine_ld_8(cpu_registers.e); }
// LD H, n8
void cpu_ld_h_n8() { cpu_routine_ld_8(cpu_registers.h); }
// LD L, n8
void cpu_ld_l_n8() { cpu_routine_ld_8(cpu_registers.l); }

/*
 * 8-Bit Increments
 */

// INC A
void cpu_inc_a() { cpu_routine_inc_8(cpu_registers.a); }
// INC B
void cpu_inc_b() { cpu_routine_inc_8(cpu_registers.b); }
// INC C
void cpu_inc_c() { cpu_routine_inc_8(cpu_registers.c); }
// INC D
void cpu_inc_d() { cpu_routine_inc_8(cpu_registers.d); }
// INC E
void cpu_inc_e() { cpu_routine_inc_8(cpu_registers.e); }
// INC H
void cpu_inc_h() { cpu_routine_inc_8(cpu_registers.h); }
// INC L
void cpu_inc_l() { cpu_routine_inc_8(cpu_registers.l); }

/*
 * 8-Bit Decrements
 */

// DEC A
void cpu_dec_a() { cpu_routine_dec_8(cpu_registers.a); }
// DEC B
void cpu_dec_b() { cpu_routine_dec_8(cpu_registers.b); }
// DEC C
void cpu_dec_c() { cpu_routine_dec_8(cpu_registers.c); }
// DEC D
void cpu_dec_d() { cpu_routine_dec_8(cpu_registers.d); }
// DEC E
void cpu_dec_e() { cpu_routine_dec_8(cpu_registers.e); }
// DEC H
void cpu_dec_h() { cpu_routine_dec_8(cpu_registers.h); }
// DEC L
void cpu_dec_l() { cpu_routine_dec_8(cpu_registers.l); }

/*
 * 8-Bit indirect loads from register
 */

// LD (BC), A
void cpu_ld_bc_a() { cpu_routine_ld_ptr8(cpu_registers.bc, cpu_registers.a); }
// LD (DE), A
void cpu_ld_de_a() { cpu_routine_ld_ptr8(cpu_registers.de, cpu_registers.a); }
// LD (HL), A
void cpu_ld_hl_a() { cpu_routine_ld_ptr8(cpu_registers.hl, cpu_registers.a); }
// LD (HL), B
void cpu_ld_hl_b() { cpu_routine_ld_ptr8(cpu_registers.hl, cpu_registers.b); }
// LD (HL), C
void cpu_ld_hl_c() { cpu_routine_ld_ptr8(cpu_registers.hl, cpu_registers.c); }
// LD (HL), D
void cpu_ld_hl_d() { cpu_routine_ld_ptr8(cpu_registers.hl, cpu_registers.d); }
// LD (HL), E
void cpu_ld_hl_e() { cpu_routine_ld_ptr8(cpu_registers.hl, cpu_registers.e); }
// LD (HL), H
void cpu_ld_hl_h() { cpu_routine_ld_ptr8(cpu_registers.hl, cpu_registers.h); }
// LD (HL), L
void cpu_ld_hl_l() { cpu_routine_ld_ptr8(cpu_registers.hl, cpu_registers.l); }

/*
 * 8-Bit indirect loads to register
 */

// LD A, (BC)
void cpu_ld_a_bc() { cpu_routine_ld_ptr_16(cpu_registers.a, cpu_registers.bc); }
// LD A, (DE)
void cpu_ld_a_de() { cpu_routine_ld_ptr_16(cpu_registers.a, cpu_registers.de); }
// LD A, (HL)
void cpu_ld_a_hl() { cpu_routine_ld_ptr_16(cpu_registers.a, cpu_registers.hl); }
// LD B, (HL)
void cpu_ld_b_hl() { cpu_routine_ld_ptr_16(cpu_registers.b, cpu_registers.hl); }
// LD C, (HL)
void cpu_ld_c_hl() { cpu_routine_ld_ptr_16(cpu_registers.c, cpu_registers.hl); }
// LD D, (HL)
void cpu_ld_d_hl() { cpu_routine_ld_ptr_16(cpu_registers.d, cpu_registers.hl); }
// LD E, (HL)
void cpu_ld_e_hl() { cpu_routine_ld_ptr_16(cpu_registers.e, cpu_registers.hl); }
// LD H, (HL)
void cpu_ld_h_hl() { cpu_routine_ld_ptr_16(cpu_registers.h, cpu_registers.hl); }
// LD L, (HL)
void cpu_ld_l_hl() { cpu_routine_ld_ptr_16(cpu_registers.l, cpu_registers.hl); }

/*
 * 8-Bit ADD to Accumulator
 */

// ADD A, B
void cpu_add_b_to_a() { cpu_routine_add_to_a_8(cpu_registers.b); }
// ADD A, C
void cpu_add_c_to_a() { cpu_routine_add_to_a_8(cpu_registers.c); }
// ADD A, D
void cpu_add_d_to_a() { cpu_routine_add_to_a_8(cpu_registers.d); }
// ADD A, E
void cpu_add_e_to_a() { cpu_routine_add_to_a_8(cpu_registers.e); }
// ADD A, H
void cpu_add_h_to_a() { cpu_routine_add_to_a_8(cpu_registers.h); }
// ADD A, L
void cpu_add_l_to_a() { cpu_routine_add_to_a_8(cpu_registers.l); }

/*
 * 16-Bit Loads
 */

// LD HL, n16
void cpu_ld_hl_n16() { cpu_routine_ld_16(cpu_registers.h, cpu_registers.l) }
// LD BC, n16
void cpu_ld_bc_n16() { cpu_routine_ld_16(cpu_registers.b, cpu_registers.c); }
// LD BC, n16
void cpu_ld_de_n16() { cpu_routine_ld_16(cpu_registers.d, cpu_registers.e) }
// LD SP, n16
void cpu_ld_sp_n16() { cpu_routine_ld_16(cpu_registers.s, cpu_registers.p); }

/*
 * 16-Bit Increments
 */

// INC BC
void cpu_inc_bc() { cpu_routine_inc_16(cpu_registers.bc); }
// INC DE
void cpu_inc_de() { cpu_routine_inc_16(cpu_registers.de); }
// INC HL
void cpu_inc_hl() { cpu_routine_inc_16(cpu_registers.hl); }
// INC SP
void cpu_inc_sp() { cpu_routine_inc_16(cpu_registers.sp); }

/*
 * 16-Bit Decrements
 */

// DEC BC
void cpu_dec_bc() { cpu_routine_dec_16(cpu_registers.bc); }
// DEC DE
void cpu_dec_de() { cpu_routine_dec_16(cpu_registers.de); }
// DEC HL
void cpu_dec_hl() { cpu_routine_dec_16(cpu_registers.hl); }
// DEC SP
void cpu_dec_sp() { cpu_routine_dec_16(cpu_registers.sp); }

/*
 * 16-Bit ADD to HL
 */

// ADD HL, BC
void cpu_add_bc_to_hl() { cpu_routine_add_hl_16(cpu_registers.bc); }
// ADD HL, DE
void cpu_add_de_to_hl() { cpu_routine_add_hl_16(cpu_registers.de); }
// ADD HL, SP
void cpu_add_sp_to_hl() { cpu_routine_add_hl_16(cpu_registers.sp); }
