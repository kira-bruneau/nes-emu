#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <gio/gio.h>

#include "../util.h"

typedef enum Mirror {
  MIRROR_HORIZONTAL,
  MIRROR_VERTICAL,
  MIRROR_QUAD
} Mirror;

typedef struct Cartridge {
  byte * prg_rom;
  byte * chr_rom;
  
  byte prg_rom_size;
  byte chr_rom_size;
  
  byte mapper;
  
  Mirror mirror : 2;
  bool prg_ram : 1;
} Cartridge;

Cartridge * cartridge_new(GFile * rom_file);
byte cartridge_read(Cartridge * cartridge, uint16_t addr);

#endif
