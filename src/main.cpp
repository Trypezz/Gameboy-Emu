#include <iostream>

#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>

#include "Cartridge/cart.hpp"

int SDLCALL gameboy_emu_runapp_callback(int argc, char *argv[]);

int main(int argc, char *argv[]) {
  return SDL_RunApp(argc, argv, gameboy_emu_runapp_callback, NULL);
}

int SDLCALL gameboy_emu_runapp_callback(int argc, char *argv[]) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    return -1;
  }

  if (argc < 2) {
    printf("Usage: %s <rom.gb>\n", argv[0]);
    SDL_Quit();
    return 1;
  }

  if (cart_load(argv[1])) {
    cart_print_info();
  }

  SDL_Quit();
  return 0;
}
