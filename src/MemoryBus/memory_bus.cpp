#include <stdint.h>

#include "Cartridge/cart.hpp"
#include "MemoryBus/memory_bus.hpp"

uint8_t memory_bus_read(const uint16_t addr) { return cartridge_data[addr]; }

void memory_bus_write(const uint16_t addr, const uint8_t value) {
  // TODO: implement writes
}

uint8_t memory_bus_write(const uint16_t addr) {
  // TODO: implement writes
  return 0;
}
