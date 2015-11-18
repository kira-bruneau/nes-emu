#define _GNU_SOURCE

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

static GModule * mapper_load(int mapper_no) {
  char * mapper_file;
  asprintf(&mapper_file, "./mapper/mapper-%i", mapper_no);

  // Try to load the module
  // TODO: DEBUG
  /* GModule * module = g_module_open(mapper_file, G_MODULE_BIND_LAZY); */
  GModule * module = NULL;

  // Otherwise, try to compile mapper source code and try again
  if (!module) {
    char * compile_command;
    asprintf(&compile_command,
             "gcc -fPIC -shared -I./src -o %s.so %s.c",
             mapper_file, mapper_file);

    printf("%s\n", compile_command);
    if (system(compile_command) == 0) {
      module = g_module_open(mapper_file, G_MODULE_BIND_LAZY);
    }
  }

  free(mapper_file);
  return module;
}

Mapper * mapper_create(Cartridge * cartridge) {
  Mapper * mapper = g_malloc(sizeof(Mapper));
  int mapper_no = cartridge->mapper_no;

  mapper->module = mapper_load(mapper_no);
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
