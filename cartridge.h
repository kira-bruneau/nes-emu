#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "util.h"

typedef enum Mirror {
  MIRROR_HORIZONTAL,
  MIRROR_VERTICAL,
  MIRROR_QUAD
} Mirror;

typedef struct Cartridge {
  byte * prg_rom;
  byte * chr_rom;
  int mapper;
  Mirror mirror;
  bool prg_ram;
} Cartridge;

#endif
