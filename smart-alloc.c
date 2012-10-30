#include "smart-alloc.h"

#include <stdio.h>

int smart_debug = 0;

struct free_list {
    struct free_list *next;
    size_t size;
};

struct smart_alloc {
    size_t size;
    struct free_list *fl_head;
};

struct alloc {
    size_t size;
};

static void
dump_memory(struct smart_alloc *sa, const char *message)
{
    struct free_list *node       = NULL;
    struct free_list *final_node = NULL;
    char *end_memory;
    char *end_free_region;

    if(! sa) {
        printf("sa is NULL!\n");
        node = 0;
        (void)node->size;
    }
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
        final_node = node;
        node       = node->next;
    }
    end_memory      = ((char *) sa) + sa->size;
    end_free_region = ((char *) final_node) + sizeof(struct free_list) + final_node->size;

    if(end_free_region > end_memory) {
        printf("final node's region extends beyond memory!\n");
        printf("end of memory: %p\n", end_memory);
        printf("end of free region: %p\n", end_free_region);
        node = 0;
        (void)node->size;
    }
}

struct smart_alloc *
smart_alloc_init(void *mem, size_t size)
{
    size_t i;
    char *memory           = (char *) mem;
    struct smart_alloc *sa = (struct smart_alloc *) mem;

    for(i = 0; i < size; i++) {
        memory[i] = 0;
    }

    sa->size          = size;
    sa->fl_head       = (struct free_list *) (memory + sizeof(struct smart_alloc));
    sa->fl_head->next = NULL;
    sa->fl_head->size = size - sizeof(struct smart_alloc) - sizeof(struct free_list);

    return sa;
}

void
smart_alloc_destroy(struct smart_alloc *sa)
{
}

void *
smart_alloc(struct smart_alloc *sa, size_t size)
{
    struct free_list **origin;
    struct free_list *node;
    struct free_list *next_node;
    struct alloc *header;

    dump_memory(sa, "before alloc");

    origin = &(sa->fl_head);
    node   = sa->fl_head;

    while(node && node->size < size) {
        origin = &(node->next);
        node   = node->next;
    }

    if(! node) {
        return NULL;
    }

    dump_memory(sa, "mid alloc");
    if(node->size - size < sizeof(struct free_list)) {
        size = node->size;
    }
    if(size == node->size) {
        next_node = node->next;
    } else {
        next_node       = (struct free_list *) (((char *) node) + sizeof(struct free_list) + size);
        next_node->next = node->next;
        next_node->size = node->size - (sizeof(struct free_list) + size);
    }
    *origin = next_node;

    header       = (struct alloc *) node;
    header->size = size;

    dump_memory(sa, "after alloc");
    return (void *) node + sizeof(struct alloc);
}

void
smart_free(struct smart_alloc *sa, void *p)
{
    struct free_list **origin;
    struct free_list *node;
    struct free_list *new_node;
    struct alloc *header;
    size_t size;

    origin   = &(sa->fl_head);
    node     = sa->fl_head;
    new_node = NULL;
    header   = (struct alloc *) (p - sizeof(struct alloc));

    dump_memory(sa, "before free");
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

    dump_memory(sa, "after free");
}
