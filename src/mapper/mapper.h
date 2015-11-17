#ifndef MAPPER_H
#define MAPPER_H

#include <stdint.h>
#include "cartridge/cartridge.h"

typedef struct Mapper Mapper;

Mapper * mapper_create(Cartridge * cartridge);
void mapper_destroy(Mapper * mapper);
void mapper_write(Mapper * mapper, uint16_t addr, uint8_t val);
uint8_t mapper_read(Mapper * mapper, uint16_t addr);

#endif
