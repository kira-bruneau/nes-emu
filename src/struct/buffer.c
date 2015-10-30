#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "buffer.h"

struct Buffer {
  size_t capacity;
  size_t size;
  size_t index;
};

Buffer * buffer_create(size_t capacity) {
  Buffer * buffer = malloc(sizeof(Buffer) + capacity);
  buffer->capacity = capacity;
  buffer->size = 0;
  buffer->index = 0;
  return buffer;
}

void buffer_destroy(Buffer * buffer) {
  free(buffer);
}

size_t buffer_size(Buffer * buffer) {
  return buffer->size;
}

size_t buffer_write(Buffer * buffer, const void * in, size_t size, size_t nmemb) {
  const char * data_in = in;
    
  char * raw_buffer = (char *)(buffer + 1);
  size_t index = (buffer->index + buffer->size) % buffer->capacity;
  
  size_t i, j;
  for (i = 0; i < nmemb && buffer->size + size <= buffer->capacity; ++i) {
    for (j = 0; j < size; ++j) {
      raw_buffer[index++] = *data_in++;
      index %= buffer->capacity;
    }
    
    buffer->size += size;
  }

  return i;
}

size_t buffer_read(Buffer * buffer, void * out, size_t size, size_t nmemb) {
  char * data_out = out;
  
  char * raw_buffer = (char *)(buffer + 1);
  size_t index = buffer->index;
  
  size_t i, j;
  for (i = 0; i < nmemb && buffer->size - size <= buffer->size; ++i) {
    for (j = 0; j < size; ++j) {
      *data_out++ = raw_buffer[index++];
      index %= buffer->capacity;
    }
    
    buffer->size -= size;
  }

  buffer->index = index;
  return i;
}

void buffer_print(Buffer * buffer) {
  printf("%zu %zu %zu [", buffer->capacity, buffer->index, buffer->size);

  unsigned char * data = (unsigned char *)(buffer + 1);
  size_t start = buffer->index;
  size_t end = (buffer->index + buffer->size) % buffer->capacity;
  
  size_t i = 0;
  do {
    if (i == start && i == end) {
      printf("|");
    } else if (i == start) {
      printf("{");
    } else if (i == end) {
      printf("}");
    } else {
      printf(" ");
    }

    if (i++ >= buffer->capacity) {
      break;
    }
    
    printf("%02X", *data++);
  } while (1);
  
  printf("]\n");
}
