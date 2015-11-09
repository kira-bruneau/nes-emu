#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <gio/gio.h>

#include "util.h"

typedef enum Mirror {
  MIRROR_HORIZONTAL,
  MIRROR_VERTICAL,
  MIRROR_QUAD
} Mirror;

typedef struct Cartridge Cartridge;

Cartridge * cartridge_create(GFile * rom_file);
byte cartridge_read(Cartridge * cartridge, uint16_t addr);

#endif
