#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <stdint.h>

#include <gio/gio.h>

typedef struct Cartridge Cartridge;

Cartridge * cartridge_create(GFile * rom_file);
void cartridge_destroy(Cartridge * cartridge);

void cartridge_write(Cartridge * cartridge, uint16_t addr, uint8_t val);
uint8_t cartridge_read(Cartridge * cartridge, uint16_t addr);

#endif
