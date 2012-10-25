#include "smart-alloc.h"

#include <stdio.h>

#define ARENA_SIZE (4096)

int smart_debug = 0;

struct free_list *fl_head = NULL;
char memory[ARENA_SIZE];

struct free_list {
    struct free_list *next;
    size_t size;
};

static int
init_memory(void)
{
    fl_head       = (struct free_list *) memory;
    fl_head->next = NULL;
    fl_head->size = ARENA_SIZE - sizeof(struct free_list);

    return 0;
}

void *
smart_alloc(size_t size)
{
    struct free_list **origin;
    struct free_list *node;
    struct free_list *next_node;

    if(! fl_head) {
        if(init_memory()) {
            return NULL;
        }
    }
    origin = &fl_head;
    node   = fl_head;

    while(node && node->size < size) {
        origin = &(node->next);
        node   = node->next;
    }

    if(! node) {
        return NULL;
    }

    next_node       = (struct free_list *) (((char *) node) + sizeof(struct free_list) + size);
    next_node->next = node->next;
    next_node->size = node->size - (sizeof(struct free_list) * 2 + size);
    *origin         = next_node;

    return (void *) node + sizeof(struct free_list);
}

void
smart_free(void *p)
{
}
