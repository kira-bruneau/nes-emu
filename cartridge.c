#include "cartridge.h"

#include <stdio.h>
#include <string.h>

Cartridge * cartridge_new(GFile * rom_file) {
  Cartridge * cartridge = g_malloc(sizeof(cartridge));

  GInputStream * stream = (GInputStream *)g_file_read(rom_file, NULL, NULL);
  if (!stream) {
    return NULL;
  }

  // Read header
  struct NESHeader {
    byte magic[4];
    byte prg_rom_size;
    byte chr_rom_size;

    // Flags 6
    byte mirror_vert  : 1;
    byte prg_ram      : 1;
    byte trainer      : 1;
    byte mirror_quad  : 1;
    byte mapper_lower : 4;

    // Flags 7
    byte vs_unisystem : 1;
    byte playchoice10 : 1;
    byte version      : 2;
    byte mapper_upper : 4;

    byte prg_ram_size;
    byte zero[7];
  } header;

  g_input_stream_read(stream, &header, sizeof(header), NULL, NULL);

  static byte nes_magic[] = {'N', 'E', 'S', 0x1A};
  if (memcmp(&header.magic, nes_magic, 4)) {
    fprintf(stderr, "ERROR: File is not a ROM file!\n");
    return NULL;
  }

  // Read trainer (don't worry about this yet)
  if (header.trainer) {
    g_input_stream_skip(stream, 512, NULL, NULL);
  }

  // Read PRG ROM data
  size_t prg_rom_size = 16384 * header.prg_rom_size;
  byte * prg_rom = g_malloc(prg_rom_size);
  g_input_stream_read(stream, prg_rom, prg_rom_size, NULL, NULL);

  // Read CHR ROM data
  size_t chr_rom_size = 8192 * header.chr_rom_size;
  byte * chr_rom = g_malloc(chr_rom_size);
  g_input_stream_read(stream, &chr_rom, chr_rom_size, NULL, NULL);

  // Mapper
  int mapper = header.mapper_lower | header.mapper_upper << 4;

  // Mirroring
  Mirror mirror;
  if (header.mirror_quad) {
    mirror = MIRROR_QUAD;
  } else if (header.mirror_vert) {
    mirror = MIRROR_VERTICAL;
  } else {
    mirror = MIRROR_HORIZONTAL;
  }

  cartridge->prg_rom_size = prg_rom_size;
  cartridge->prg_rom = prg_rom;
  cartridge->chr_rom_size = chr_rom_size;
  cartridge->chr_rom = chr_rom;
  cartridge->mapper = mapper;
  cartridge->mirror = mirror;
  cartridge->prg_ram = header.prg_ram;

  return cartridge;
}

byte cartridge_read(Cartridge * cartridge, uint16_t addr) {
  return cartridge->prg_rom[addr % cartridge->prg_rom_size];
}
