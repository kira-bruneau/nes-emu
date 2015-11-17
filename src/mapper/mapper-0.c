#include <stdio.h>
#include <stdlib.h>

#include "mapper.h"

struct Mapper {
  Cartridge * cartridge;
};

Mapper * mapper_create(Cartridge * cartridge) {
  Mapper * mapper = malloc(sizeof(Mapper));
  mapper->cartridge = cartridge;
  return mapper;
}

void mapper_destroy(Mapper * mapper) {
  free(mapper);
}

void mapper_write(Mapper * mapper, uint16_t addr, uint8_t val) {
  (void)mapper;
  (void)addr;
  (void)val;
}

uint8_t mapper_read(Mapper * mapper, uint16_t addr) {
  Cartridge * cartridge = mapper->cartridge;
  return cartridge->prg_rom[addr % (cartridge->prg_rom_size << 14)];
}
