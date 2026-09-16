#include "CPU/cpu.hpp"
#include "Cartridge/cart.hpp"
#include "emualtor_core.hpp"

uint32_t core_clock_counter;
bool core_quit_request = false;

int core_init(int argc, char *argv[]) {
  const char *rom_path = (argc >= 2) ? argv[1] : DEFAULT_ROM_PATH;
  if (!cart_load(rom_path)) {
    return -1;
  }

  cart_print_info();

  return 0;
}

int core_run() {
  cpu_reset();

  while (!core_quit_request) {
    cpu_fetch();
    if (!cpu_execute()) {
      core_quit_request = true;
    }
  }

  return 0;
}

int core_shutdown() { return 0; }

// Syncing between different parts of the 'hardware' for more accuracy
void core_advance_cpu_clocks(uint8_t clocks) { core_clock_counter += clocks; }
