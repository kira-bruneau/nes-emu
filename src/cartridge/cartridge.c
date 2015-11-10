#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <glib.h>

#include "cartridge.h"

struct Cartridge {
  uint8_t * prg_rom;
  uint8_t * chr_rom;

  uint8_t prg_rom_size;
  uint8_t chr_rom_size;

  uint8_t mapper;

  Mirror mirror : 2;
  bool prg_ram : 1;
};

static uint8_t nes_magic[] = {'N', 'E', 'S', 0x1A};

typedef struct NESHeader {
  uint8_t magic[4];
  uint8_t prg_rom_size;
  uint8_t chr_rom_size;

  // Flags 6
  uint8_t mirror_vert  : 1;
  uint8_t prg_ram      : 1;
  uint8_t trainer      : 1;
  uint8_t mirror_quad  : 1;
  uint8_t mapper_low   : 4;

  // Flags 7
  uint8_t vs_unisystem : 1;
  uint8_t playchoice10 : 1;
  uint8_t version      : 2;
  uint8_t mapper_high  : 4;

  uint8_t prg_ram_size;
  uint8_t zero[7];
} NESHeader;

Cartridge * cartridge_create(GFile * rom_file) {
  Cartridge * cartridge = g_malloc(sizeof(Cartridge));

  GInputStream * stream = (GInputStream *)g_file_read(rom_file, NULL, NULL);
  if (stream == NULL) {
    return NULL;
  }

  // Read header
  NESHeader header;
  g_input_stream_read(stream, &header, sizeof(header), NULL, NULL);

  if (memcmp(&header.magic, nes_magic, 4) != 0) {
    fprintf(stderr, "ERROR: File is not a ROM file!\n");
    return NULL;
  }

  // Read trainer (don't worry about this yet)
  if (header.trainer) {
    g_input_stream_skip(stream, 512, NULL, NULL);
  }

  // Read PRG ROM data
  uint8_t * prg_rom;
  uint8_t prg_rom_size = header.prg_rom_size;
  if (prg_rom_size != 0) {
    prg_rom = g_malloc(prg_rom_size << 14);
    g_input_stream_read(stream, prg_rom, prg_rom_size << 14, NULL, NULL);
  } else {
    prg_rom = NULL;
  }

  // Read CHR ROM data
  void * chr_rom;
  uint8_t chr_rom_size = header.chr_rom_size;
  if (chr_rom_size != 0) {
    chr_rom = g_malloc(chr_rom_size << 13);
    g_input_stream_read(stream, chr_rom, chr_rom_size << 13, NULL, NULL);
  } else {
    chr_rom = NULL;
  }

  g_input_stream_close(stream, NULL, NULL);

  // Mapper
  uint8_t mapper = header.mapper_high << 4 | header.mapper_low;

  // Mirroring
  Mirror mirror;
  if (header.mirror_quad) {
    mirror = MIRROR_QUAD;
  } else if (header.mirror_vert) {
    mirror = MIRROR_VERTICAL;
  } else {
    mirror = MIRROR_HORIZONTAL;
  }

  cartridge->prg_rom = prg_rom;
  cartridge->chr_rom = chr_rom;
  cartridge->prg_rom_size = prg_rom_size;
  cartridge->chr_rom_size = chr_rom_size;
  cartridge->mapper = mapper;
  cartridge->mirror = mirror;
  cartridge->prg_ram = header.prg_ram;

  return cartridge;
}

uint8_t cartridge_read(Cartridge * cartridge, uint16_t addr) {
  return cartridge->prg_rom[addr % (cartridge->prg_rom_size << 14)];
}
