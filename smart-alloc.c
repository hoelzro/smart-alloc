#include "smart-alloc.h"

#include <stdio.h>

#define ARENA_SIZE (4096)
#define END_OF_MEMORY() ((void *) (memory + ARENA_SIZE))

int smart_debug = 0;

struct free_list {
    struct free_list *next;
    size_t size;
};

struct smart_alloc {
    size_t size;
    struct free_list *fl_head;
};

static char memory[ARENA_SIZE];
static struct smart_alloc *sa = NULL;

struct alloc {
    size_t size;
};

static void
dump_memory(const char *message)
{
    struct free_list *node       = NULL;
    struct free_list *final_node = NULL;
    char *end_memory;
    char *end_free_region;

    node = sa->fl_head;

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

    sa                = (struct smart_alloc *) memory;
    sa->size          = ARENA_SIZE;
    sa->fl_head       = (struct free_list *) (memory + sizeof(struct smart_alloc));
    sa->fl_head->next = NULL;
    sa->fl_head->size = ARENA_SIZE - sizeof(struct smart_alloc) - sizeof(struct free_list);

    return 0;
}

void *
smart_alloc(size_t size)
{
    struct free_list **origin;
    struct free_list *node;
    struct free_list *next_node;
    struct alloc *header;

    if(! sa) {
        if(init_memory()) {
            return NULL;
        }
    }

    dump_memory("before alloc");

    origin = &(sa->fl_head);
    node   = sa->fl_head;

    while(node && node->size < size) {
        origin = &(node->next);
        node   = node->next;
    }

    if(! node) {
        return NULL;
    }

    dump_memory("mid alloc");
    if(node->size - size < sizeof(struct free_list)) {
        size = node->size;
    }
    next_node = (struct free_list *) (((char *) node) + sizeof(struct free_list) + size);
    if(((void *) next_node) >= END_OF_MEMORY()) {
        next_node = NULL;
    } else if(node->next != next_node) {
        next_node->next = node->next;
        next_node->size = node->size - (sizeof(struct free_list) + size);
    }
    *origin = next_node;

    header       = (struct alloc *) node;
    header->size = size;

    dump_memory("after alloc");
    return (void *) node + sizeof(struct alloc);
}

void
smart_free(void *p)
{
    struct free_list **origin;
    struct free_list *node;
    struct free_list *new_node;
    struct alloc *header;
    size_t size;

    if(! sa) {
        init_memory();
    }

    origin   = &(sa->fl_head);
    node     = sa->fl_head;
    new_node = NULL;
    header   = (struct alloc *) (p - sizeof(struct alloc));

    dump_memory("before free");
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

    dump_memory("after free");
}
