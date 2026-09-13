#include "Cartridge/cart.hpp"

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <ios>
#include <iosfwd>
#include <linux/limits.h>
#include <unistd.h>

char runtime_path_buffer[PATH_MAX];
uint8_t cartridge_data[MAX_CART_SIZE];
bool cartridge_loaded = false;
cart_header_struct *cartridge_header =
    (cart_header_struct *)(cartridge_data + 0x100);

bool get_runtime_path() {
  return getcwd(runtime_path_buffer, PATH_MAX) != nullptr;
}

void cart_print_info() {
  printf("Entry point: %.2X%.2X%.2X%.2X\n", cartridge_header->entry_point[0],
         cartridge_header->entry_point[1], cartridge_header->entry_point[2],
         cartridge_header->entry_point[3]);
  printf("Title: %s\n", cartridge_header->title);
  printf("CBG Flag: %2X\n", cartridge_header->cgb_flag);
  printf("New Licensee code: %.2X%.2X\n",
         cartridge_header->new_licensee_code[0],
         cartridge_header->new_licensee_code[1]);
  printf("SGB Flag: %2X\n", cartridge_header->sgb_flag);
  printf("Type: %2X\n", cartridge_header->cartridge_type);
  printf("ROM Size: %2X\n", cartridge_header->rom_size);
  printf("RAM Size: %2X\n", cartridge_header->ram_size);
  printf("Destination Code: %2X\n", cartridge_header->destination_code);
  printf("Old Licensee Code: %2X\n", cartridge_header->old_licensee_code);
  printf("Version: %2X\n", cartridge_header->mask_rom_version_number);
  printf("Header Checksum: %2X\n", cartridge_header->header_checksum);
  printf("Global Checksum: %2X%2X\n", cartridge_header->global_checksum_hi,
         cartridge_header->global_checksum_lo);
}

bool cart_load(const char *filename) {
  std::streampos size;
  std::ifstream file(filename, std::ios::in | std::ios::binary | std::ios::ate);
  if (file.is_open()) {
    size = file.tellg();
    file.seekg(0, std::ios::beg);
    file.read((char *)cartridge_data, MAX_CART_SIZE);
    file.close();
    printf("ROM %s loaded, size: %lld bytes\n", filename,
           static_cast<long long>(std::streamoff(size)));
    return true;
  }
  printf("Failed to load file %s\n", filename);
  return false;
}
