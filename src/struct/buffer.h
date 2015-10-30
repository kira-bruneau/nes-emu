#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>

typedef struct Buffer Buffer;

Buffer * buffer_create(size_t capacity);
void buffer_destroy(Buffer * buffer);

size_t buffer_size(Buffer * buffer);
size_t buffer_write(Buffer * buffer, const void * in, size_t size, size_t nmemb);
size_t buffer_read(Buffer * buffer, void * out, size_t size, size_t nmemb);
void buffer_print(Buffer * buffer);

#endif
