#pragma once

#include <cstdint>

extern uint32_t core_clock_counter;

int core_init(int argc, char *argv[]);
int core_run();
int core_shutdown();

// Syncing between different parts of the 'hardware' for more accuracy
void core_advance_cpu_clocks(uint8_t clocks);
