#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>

#include "CPU/cpu.hpp"
#include "Cartridge/cart.hpp"

int SDLCALL gameboy_emu_runapp_callback(int argc, char *argv[]);

int main(int argc, char *argv[]) {
  return SDL_RunApp(argc, argv, gameboy_emu_runapp_callback, NULL);
}

int SDLCALL gameboy_emu_runapp_callback(int argc, char *argv[]) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Quit();
    return -1;
  }

  const char *rom_path = (argc >= 2) ? argv[1] : DEFAULT_ROM_PATH;
  if (!cart_load(rom_path)) {
    SDL_Quit();
    return 1;
  }

  cart_print_info();

  cpu_reset();
  cpu_fetch();
  cpu_execute();

  SDL_Quit();
  return 0;
}
