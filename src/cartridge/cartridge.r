#ifndef CARTRIDGE_R
#define CARTRIDGE_R

#include <stdint.h>
#include <stdbool.h>

typedef struct Mapper Mapper;
typedef struct Cartridge Cartridge;

typedef enum {
  MIRROR_HORIZONTAL,
  MIRROR_VERTICAL,
  MIRROR_QUAD
} Mirror;

struct Cartridge {
  Mapper * mapper; // Maybe this shouldn't be "public"
  int mapper_no;   // Maybe this shouldn't be "public"

  uint8_t * prg_rom;
  uint8_t * chr_rom;

  uint8_t prg_rom_size;
  uint8_t chr_rom_size;

  Mirror mirror : 2;
  bool prg_ram : 1;
};

#endif
