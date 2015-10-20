#include <glib.h>
#include <gio/gio.h>

#include <stdio.h>

#include "nes.h"

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

int main() {
  GList * rom_list = NULL;
  rom_list = find_roms(rom_list, "roms/");
  rom_list = g_list_sort(rom_list, (GCompareFunc)g_strcmp0);

  GFile * rom_file = select_rom(rom_list);
  g_list_free_full(rom_list, g_free);
  if (!rom_file) {
    return 0;
  }

  NES * nes = nes_new(rom_file);
  g_object_unref(rom_file);
  return 0;
}
