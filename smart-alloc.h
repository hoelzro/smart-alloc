#ifndef SMART_ALLOC_H
#define SMART_ALLOC_H

#include <stddef.h>

void * smart_alloc(size_t size);
void smart_free(void *p);

int smart_debug;

#define d() if(smart_debug) { printf("Here: %s:%d\n", __FILE__, __LINE__); }

#endif
