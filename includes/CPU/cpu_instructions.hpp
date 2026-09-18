#pragma once

#include "CPU/cpu.hpp"

struct gb_cpu_instruction {
  const char *disassembly;
  uint8_t operand_length;
  cpu_execute_op execute;
};

extern const gb_cpu_instruction instructions[256];
