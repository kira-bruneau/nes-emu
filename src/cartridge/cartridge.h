#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <stdint.h>
#include <stdbool.h>

#include <gio/gio.h>

typedef struct Mapper Mapper;
typedef struct Cartridge Cartridge;

typedef enum {
  MIRROR_HORIZONTAL,
  MIRROR_VERTICAL,
  MIRROR_QUAD
} Mirror;

struct Cartridge {
  Mapper * mapper;
  int mapper_no;

  uint8_t * prg_rom;
  uint8_t * chr_rom;

  uint8_t prg_rom_size;
  uint8_t chr_rom_size;

  Mirror mirror : 2;
  bool prg_ram : 1;
};

Cartridge * cartridge_create(GFile * rom_file);
void cartridge_destroy(Cartridge * cartridge);

void cartridge_write(Cartridge * cartridge, uint16_t addr, uint8_t val);
uint8_t cartridge_read(Cartridge * cartridge, uint16_t addr);

#endif
