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
// LD HL, a16
void cpu_ld_hl_a16() { cpu_routine_ld_16(cpu_registers.h, cpu_registers.l) }
// LD C, a8
void cpu_ld_c_a8() { cpu_routine_ld_8(cpu_registers.c); }
// LD B, a8
void cpu_ld_b_a8() { cpu_routine_ld_8(cpu_registers.b); }
// LD (HL-), A = LDD (HL), A
void cpu_ldd_hl_a() {
  core_advance_cpu_clocks(4);
  memory_bus_write(cpu_registers.hl, cpu_registers.a);
  cpu_registers.hl = (cpu_registers.hl - 1) & 0xFFFF;
  core_advance_cpu_clocks(4);
}
// DEC B
void cpu_dec_b() { cpu_routine_dec_8(cpu_registers.b); }
