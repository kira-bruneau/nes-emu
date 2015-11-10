#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <stdint.h>

#include <gio/gio.h>

typedef enum Mirror {
  MIRROR_HORIZONTAL,
  MIRROR_VERTICAL,
  MIRROR_QUAD
} Mirror;

typedef struct Cartridge Cartridge;

Cartridge * cartridge_create(GFile * rom_file);
uint8_t cartridge_read(Cartridge * cartridge, uint16_t addr);

#endif
