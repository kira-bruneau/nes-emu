#include <glib.h>
#include <gio/gio.h>

#include <stdio.h>
#include <stdlib.h>

#include "nes.h"
#include "cartridge.h"
#include "util.h"

/**
 * Find ROM files recursively in a directory
 */
static GList * find_roms(GList * rom_list, const char * dir_name) {
  GDir * dir = g_dir_open(dir_name, 0, NULL);
  if (!dir) {
    return rom_list;
  }

  const char * file_name = g_dir_read_name(dir);
  while (file_name != NULL) {
    char * file_path = g_build_filename(dir_name, file_name, NULL);

    if (g_file_test(file_path, G_FILE_TEST_IS_REGULAR))  {
      rom_list = g_list_append(rom_list, file_path);
    } else if (g_file_test(file_path, G_FILE_TEST_IS_DIR)) {
      find_roms(rom_list, file_path);
      g_free(file_path);
    } else {
      g_free(file_path);
    }

    file_name = g_dir_read_name(dir);
  }

  g_dir_close(dir);
  return rom_list;
}

/**
 * List a single ROM item for selection
 */
static void list_rom(gpointer rom_name, gpointer user_data) {
  int * index = user_data;
  printf("%i) Play \"%s\"\n", *index, rom_name);
  *index += 1;
}

/*
 * Prompt the user to select a ROM from a list
 */
static GFile * select_rom(GList * rom_list) {
  const char * file_name;

  do {
    int index = 0;
    printf("%i) Quit\n", index++);
    g_list_foreach(rom_list, list_rom, &index);
    printf("Select action: ");

    char buffer[16];
    if (fgets(buffer, ARRAY_LENGTH(buffer), stdin) == NULL) {
      return NULL;
    }

    char * ptr;
    int input = strtoul(buffer, &ptr, 10);
    if (ptr == buffer) {
      printf("Error: %s\n", "Invalid");
      return NULL;
    }

    if (input == 0) {
      return NULL;
    }

    file_name = g_list_nth_data(rom_list, input - 1);
    if (!file_name) {
      printf("Error: %s\n", "Out of range");
      return NULL;
    }

    break;
  } while (1);

  return g_file_new_for_path(file_name);
}

int main() {
  const char * dir = "roms/";

  GList * rom_list = NULL;
  rom_list = find_roms(rom_list, dir);
  if (rom_list == NULL) {
    fprintf(stderr, "No ROMs found at '%s'\n", dir);
    return 1;
  }

  rom_list = g_list_sort(rom_list, (GCompareFunc)g_strcmp0);

  GFile * rom_file = select_rom(rom_list);
  g_list_free_full(rom_list, g_free);
  if (!rom_file) {
    return 0;
  }

  Cartridge * cartridge = cartridge_new(rom_file);
  g_object_unref(rom_file);

  NES * nes = nes_new();
  nes_load(nes, cartridge);
  return 0;
}
