#include <stdio.h>
#include <stdlib.h>

#include <glib.h>
#include <gmodule.h>

#include "mapper.h"
#include "array.h"

struct Mapper {
  Mapper * raw;
  GModule * module;

  Mapper * (*create)(Cartridge * cartridge);
  void (*destroy)(Mapper * mapper);
  void (*write)(Mapper * mapper, uint16_t addr, uint8_t val);
  uint8_t (*read)(Mapper * mapper, uint16_t addr);
};

struct {
  const char * name;
  size_t offset;
} symbol_map[] = {
  {"mapper_create", offsetof(Mapper, create)},
  {"mapper_destroy", offsetof(Mapper, destroy)},
  {"mapper_write", offsetof(Mapper, write)},
  {"mapper_read", offsetof(Mapper, read)}
};

Mapper * mapper_create(Cartridge * cartridge) {
  Mapper * mapper = g_malloc(sizeof(Mapper));
  int mapper_no = cartridge->mapper_no;

  char mapper_file[32];
  snprintf(mapper_file, ARRAY_LENGTH(mapper_file), "./mapper/%i", mapper_no);
  mapper->module = g_module_open(mapper_file, G_MODULE_BIND_LAZY);
  if (!mapper->module) {
    fprintf(stderr, "Failed to load mapper #%i\n", mapper_no);
    g_free(mapper);
    return NULL;
  }

  for (size_t i = 0; i < ARRAY_LENGTH(symbol_map); ++i) {
    gpointer * symbol = (gpointer *)((char *)mapper + symbol_map[i].offset);
    if (!g_module_symbol(mapper->module, symbol_map[i].name, symbol)) {
      fprintf(stderr, "Failed to load symbol '%s' for mapper #%i\n", symbol_map[i].name, mapper_no);
      g_module_close(mapper->module);
      g_free(mapper);
      return NULL;
    }
  }

  mapper->raw = mapper->create(cartridge);
  if (!mapper->raw) {
    g_module_close(mapper->module);
    g_free(mapper);
    return NULL;
  }

  return mapper;
}

void mapper_destroy(Mapper * mapper) {
  mapper->destroy(mapper->raw);
  g_module_close(mapper->module);
  g_free(mapper);
}

void mapper_write(Mapper * mapper, uint16_t addr, uint8_t val) {
  mapper->write(mapper->raw, addr, val);
}

uint8_t mapper_read(Mapper * mapper, uint16_t addr) {
  return mapper->read(mapper->raw, addr);
}
