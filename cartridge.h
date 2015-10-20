#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <gio/gio.h>

#include "util.h"

typedef enum Mirror {
  MIRROR_HORIZONTAL,
  MIRROR_VERTICAL,
  MIRROR_QUAD
} Mirror;

typedef struct Cartridge {
  size_t prg_rom_size;
  byte * prg_rom;

  size_t chr_rom_size;
  byte * chr_rom;
  
  int mapper;
  Mirror mirror;
  
  bool prg_ram;
} Cartridge;

Cartridge * cartridge_new(GFile * rom_file);
byte cartridge_read(Cartridge * cartridge, uint16_t addr);

#endif
