#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "buffer-debug.h"

static void buffer_debug_write(Buffer * buffer, char * in) {
  int len = strlen(in) - 1;
  if (in[len] == '\n') {
    in[len] = '\0';
  }

  buffer_write(buffer, in, sizeof(char), len);
  printf("Wrote '%s'\n", in);
}

static void buffer_debug_read(Buffer * buffer, char * in) {
  int num = strtol(in, NULL, 10);

  char results[num];
  num = buffer_read(buffer, &results, sizeof(char), num);
  printf("Read %i values\n", num);

  int i;
  for (i = 0; i < num; ++i) {
    printf("%c", results[i]);
  }

  printf("\n");
}

void buffer_debug(void) {
  Buffer * buffer = buffer_create(16);

  char input_buffer[256];
  while (fgets(input_buffer, 256, stdin) != NULL) {
    char * ptr = input_buffer;

    char command = *ptr++;
    for (; (*ptr == ' ' || *ptr == '\n') && *ptr != '\0'; ++ptr);

    switch (command) {
    case 'w':
      buffer_debug_write(buffer, ptr);
      break;
    case 'r':
      buffer_debug_read(buffer, ptr);
      break;
    case ' ': case '\t': case'\n':
      break;
    default:
      fprintf(stderr, "Invalid command\n");
      break;
    }

    buffer_print(buffer);
  }

  buffer_destroy(buffer);
}
