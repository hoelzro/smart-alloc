#ifndef SMART_ALLOC_H
#define SMART_ALLOC_H

#include <stddef.h>

struct smart_alloc;

struct smart_alloc *smart_alloc_init(void *mem, size_t size);
void smart_alloc_destroy(struct smart_alloc *sa);
void * smart_alloc(struct smart_alloc *sa, size_t size);
void smart_free(struct smart_alloc *sa, void *p);

int smart_debug;

#define d() if(smart_debug) { printf("Here: %s:%d\n", __FILE__, __LINE__); }

#endif
