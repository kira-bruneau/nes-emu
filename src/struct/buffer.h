#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>

/**
 * TODO: Create implementation that doesn't block and is lockless (mintomic)
 */

typedef struct Buffer Buffer;

Buffer * buffer_create(size_t elem_size, size_t capacity);
void buffer_destroy(Buffer * buffer);

int buffer_write(Buffer * buffer, const void * in, size_t num_elems);
int buffer_read(Buffer * buffer, void * out, size_t num_elems);

#endif
