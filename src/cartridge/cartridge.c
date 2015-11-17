#include <stdio.h>
#include <string.h>

#include <glib.h>

#include "cartridge.h"
#include "mapper/mapper.h"

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

static uint8_t nes_magic[] = {'N', 'E', 'S', 0x1A};

Cartridge * cartridge_create(GFile * rom_file) {
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

  // Mapper number
  uint8_t mapper_no = header.mapper_high << 4 | header.mapper_low;

  // Mirroring
  Mirror mirror;
  if (header.mirror_quad) {
    mirror = MIRROR_QUAD;
  } else if (header.mirror_vert) {
    mirror = MIRROR_VERTICAL;
  } else {
    mirror = MIRROR_HORIZONTAL;
  }

  Cartridge * cartridge = g_malloc(sizeof(Cartridge));
  cartridge->mapper_no = mapper_no;
  cartridge->prg_rom = prg_rom;
  cartridge->chr_rom = chr_rom;
  cartridge->prg_rom_size = prg_rom_size;
  cartridge->chr_rom_size = chr_rom_size;
  cartridge->mirror = mirror;
  cartridge->prg_ram = header.prg_ram;

  cartridge->mapper = mapper_create(cartridge);
  if (!cartridge->mapper) {
    g_free(cartridge->prg_rom);
    g_free(cartridge->chr_rom);
    g_free(cartridge);
    return NULL;
  }

  return cartridge;
}

void cartridge_destroy(Cartridge * cartridge) {
  mapper_destroy(cartridge->mapper);
  g_free(cartridge->prg_rom);
  g_free(cartridge->chr_rom);
  g_free(cartridge);
}

void cartridge_write(Cartridge * cartridge, uint16_t addr, uint8_t val) {
  mapper_write(cartridge->mapper, addr, val);
}

uint8_t cartridge_read(Cartridge * cartridge, uint16_t addr) {
  return mapper_read(cartridge->mapper, addr);
}
