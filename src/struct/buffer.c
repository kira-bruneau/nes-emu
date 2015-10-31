#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <glib.h>

#include "buffer.h"

struct Buffer {
  size_t elem_size;
  size_t capacity;
  size_t size;
  size_t index;
  
  GMutex mutex;
  GCond cond;
};

Buffer * buffer_create(size_t elem_size, size_t capacity) {
  Buffer * buffer = malloc(sizeof(Buffer) + elem_size * capacity);
  buffer->elem_size = elem_size;
  buffer->capacity = capacity;
  buffer->size = 0;
  buffer->index = 0;

  g_mutex_init(&buffer->mutex);
  g_cond_init(&buffer->cond);
  return buffer;
}

void buffer_destroy(Buffer * buffer) {
  g_mutex_clear(&buffer->mutex);
  g_cond_clear(&buffer->cond);
  free(buffer);
}

int buffer_write(Buffer * buffer, const void * in, size_t num_elems) {
  g_mutex_lock(&buffer->mutex);
  
  void * raw = (char *)(buffer + 1);
  size_t index = (buffer->index + buffer->size) % buffer->capacity;
  
  size_t i;
  for (i = 0; i < num_elems; ++i) {
    size_t new_size;
    while ((new_size = buffer->size + 1) > buffer->capacity) {
      // Buffer full -> wait for elements to be consumed
      g_cond_wait(&buffer->cond, &buffer->mutex);
    }

    memcpy(raw + buffer->elem_size * index, in, buffer->elem_size);
    in += buffer->elem_size;
    index = (index + 1) % buffer->capacity;
    
    buffer->size = new_size;
  }

  g_cond_signal(&buffer->cond);
  g_mutex_unlock(&buffer->mutex);
  return i;
}

int buffer_read(Buffer * buffer, void * out, size_t num_elems) {
  g_mutex_lock(&buffer->mutex);
  
  void * raw = (char *)(buffer + 1);
  size_t index = buffer->index;
  
  size_t i;
  for (i = 0; i < num_elems; ++i) {
    size_t new_size;
    if ((new_size = buffer->size - 1) > buffer->size) {
      // Buffer empty -> abort
      break;
    }

    memcpy(out, raw + buffer->elem_size * index, buffer->elem_size);
    out += buffer->elem_size;
    index = (index + 1) % buffer->capacity;
    
    buffer->size = new_size;
  }

  buffer->index = index;
  g_cond_signal(&buffer->cond);
  g_mutex_unlock(&buffer->mutex);
  return i;
}

static void buffer_print(Buffer * buffer) {
  printf("%zu %zu %zu [", buffer->capacity, buffer->index, buffer->size);

  float * data = (float *)(buffer + 1);
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
    
    printf("%f", *data++);
  } while (1);
  
  printf("]\n");
}
