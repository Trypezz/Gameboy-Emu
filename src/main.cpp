#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>

#include "emulator_core.hpp"

int SDLCALL gameboy_emu_runapp_callback(int argc, char *argv[]);

int main(int argc, char *argv[]) {
  return SDL_RunApp(argc, argv, gameboy_emu_runapp_callback, NULL);
}

int SDLCALL gameboy_emu_runapp_callback(int argc, char *argv[]) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Quit();
    return -1;
  }

  int error = core_init(argc, argv);
  if (error != 0) {
    return error;
  }

  core_run();

  core_shutdown();

  SDL_Quit();
  return 0;
}
