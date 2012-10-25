#ifndef SMART_ALLOC_H
#define SMART_ALLOC_H

#include <stddef.h>

void * smart_alloc(size_t size);
void smart_free(void *p);

#endif
