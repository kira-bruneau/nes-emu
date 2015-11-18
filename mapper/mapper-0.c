#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "mapper/mapper.h"

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
  // TODO: CHR_RAM
  Cartridge * cartridge = mapper->cartridge;
  if (addr >= 0x6000 && addr <= 0x7FFF) {
    cartridge->save_ram[addr - 0x6000] = val;
  } else {
    assert(false);
  }
}

uint8_t mapper_read(Mapper * mapper, uint16_t addr) {
  // TODO: CHR_RAM
  Cartridge * cartridge = mapper->cartridge;
  if (addr >= 0x6000 && addr <= 0x7FFF) {
    return cartridge->save_ram[addr - 0x6000];
  } else if (addr >= 0x8000) {
    return cartridge->prg_rom[(addr - 0x8000) % (cartridge->prg_rom_size << 14)];
  } else {
    assert(false);
  }

  return 0;
}
