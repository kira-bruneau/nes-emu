#include <glib.h>
#include <gio/gio.h>

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "util.h"

/**
 * Find ROM files recursively in a directory
 */
static GList * find_roms(GList * rom_list, const char * dir_name) {
  GDir * dir = g_dir_open(dir_name, 0, NULL);
  if (!dir) {
    return rom_list;
  }

  const gchar * file_name = g_dir_read_name(dir);
  while (file_name != NULL) {
    gchar * file_path = g_build_filename(dir_name, file_name, NULL);

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
  const gchar * file_name;

  do {
    int index = 0;
    printf("%i) Quit\n", index++);
    g_list_foreach(rom_list, list_rom, &index);
    printf("Select action: ");

    int input;
    if (scanf("%d", &input) != 1) {
      // This just does an infinite loop for some reason
      continue;
    }

    if (input == 0) {
      return NULL;
    }

    file_name = g_list_nth_data(rom_list, input - 1);
    if (!file_name) {
      printf("%s\n", "Invalid!");
      continue;
    }

    break;
  } while (1);

  return g_file_new_for_path(file_name);
}

/*
 * Parse a NES ROM file
 */
static void read_rom(GFile * rom_file) {
  GInputStream * stream = (GInputStream *)g_file_read(rom_file, NULL, NULL);
  if (!stream) {
    return;
  }

  // Read header
  struct NESHeader {
    byte magic[4];
    byte prg_rom_size;
    byte chr_rom_size;

    // Flags 6
    byte mirroring        : 1;
    byte prg_ram          : 1;
    byte trainer          : 1;
    byte four_screen_vram : 1;
    byte mapper_lower     : 4;

    // Flags 7
    byte vs_unisystem : 1;
    byte playchoice10 : 1;
    byte version      : 2;
    byte mapper_upper : 4;

    byte prg_ram_size;
    byte zero[7];
  } header;

  g_input_stream_read(stream, &header, sizeof(header), NULL, NULL);

  // Read trainer (don't worry about this yet)
  if (header.trainer) {
    g_input_stream_skip(stream, 512, NULL, NULL);
  }

  // Read PRG ROM data
  size_t prg_rom_size = 16384 * header.prg_rom_size;
  int * prg_rom = g_malloc(prg_rom_size);
  g_input_stream_read(stream, prg_rom, prg_rom_size, NULL, NULL);
  g_free(prg_rom);

  // Read CHR ROM data
  size_t chr_rom_size = 8192 * header.chr_rom_size;
  void * chr_rom = g_malloc(chr_rom_size);
  g_input_stream_read(stream, &chr_rom, chr_rom_size, NULL, NULL);
  g_free(chr_rom);
}

int main() {
  GList * rom_list = NULL;
  rom_list = find_roms(rom_list, "roms/");
  rom_list = g_list_sort(rom_list, (GCompareFunc)strcmp);

  GFile * rom_file = select_rom(rom_list);
  g_list_free_full(rom_list, g_free);
  if (!rom_file) {
    return 0;
  }

  read_rom(rom_file);
  g_object_unref(rom_file);
  return 0;
}
