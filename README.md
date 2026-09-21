# Gameboy-Emu

<img align="right" src="https://visitor-badge.laobi.icu/badge?page_id=Trypezz.Gameboy-Emu&"  />

A Game Boy (DMG) emulator in C++23. This is a learning project, not a
finished emulator.

It loads a ROM, prints the cartridge header, and runs a subset of the
CPU. There is no video, audio, or input yet, so it will not play games.

## Status

| Piece      | State                                  |
| ---------- | -------------------------------------- |
| Cartridge  | Load and header parse                  |
| CPU        | Partial (loads, inc/dec, some ALU, JP) |
| CB prefix  | Not started                            |
| Memory bus | ROM reads only                         |
| PPU / LCD  | Not started                            |
| Interrupts | Not started                            |
| Timer      | Not started                            |
| Joypad     | Not started                            |
| APU        | Not started                            |
| MBC        | Not started                            |

Unimplemented opcodes print a message and stop the run loop.

## Requirements

- CMake 3.25+
- A C++23 compiler (GCC or Clang)
- Ninja (used by the default preset)
- SDL3

Arch:

```bash
sudo pacman -S cmake ninja sdl3
```

## Build

```bash
cmake --preset default
cmake --build --preset default
```

The binary lands at build/default/bin/Gameboy-Emu.

## Run

This repo does not ship ROMs. Use a dump you are allowed to have, or
homebrew.

```bash
./build/default/bin/Gameboy-Emu /path/to/rom.gb
```

There is a debug build too in:

```bash
./out/Debug/bin/Gameboy-Emu
```

With no argument it looks for a ROM under resources/ (that path is
set at configure time and is not in git).

## Layout

| Directory         | Purpose                       |
| ----------------- | ----------------------------- |
| src/CPU/          | LR35902 core and opcode table |
| src/Cartridge     | ROM load and header           |
| src/MemoryBus     | 16-bit address space (stub)   |
| src/emulator_core | load, run loop, clock counter |
| includes/         | headers, same tree as src     |

## Legal

No Game Boy ROMs and no Nintendo boot ROM are included. Game Boy is a
trademark of Nintendo.
