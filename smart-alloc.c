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

struct alloc {
    size_t size;
};

static void
dump_memory(const char *message)
{
    struct free_list *node = fl_head;

    if(smart_debug) {
        printf("%s\n", message);
    }
    while(node) {
        if(smart_debug) {
            printf("node = %p, size = %d, next = %p\n", node, node->size, node->next);
        }
        if(node == node->next) {
            printf("loop detected! terminating\n");
            node = 0;
            (void)node->size;
        }
        node = node->next;
    }
}

static int
init_memory(void)
{
    int i;

    for(i = 0; i < ARENA_SIZE; i++) {
        memory[i] = 0;
    }

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
    struct alloc *header;

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

    next_node = (struct free_list *) (((char *) node) + sizeof(struct free_list) + size);
    if(node->next != next_node && node->size - size > sizeof(struct free_list)) {
        next_node->next = node->next;
        next_node->size = node->size - (sizeof(struct free_list) + size);
    }
    *origin = next_node;

    header       = (struct alloc *) node;
    header->size = size;

    return (void *) node + sizeof(struct alloc);
}

void
smart_free(void *p)
{
    struct free_list **origin  = &fl_head;
    struct free_list *node     = fl_head;
    struct free_list *new_node = NULL;
    struct alloc *header       = (struct alloc *) (p - sizeof(struct alloc));
    size_t size;

    if(!p) {
        return;
    }

    while(node && ((void *) node) < p) {
        origin = &(node->next);
        node   = node->next;
    }

    new_node       = (struct free_list *) header;
    size           = header->size;
    new_node->size = size;
    new_node->next = node;
    *origin        = new_node;
}
