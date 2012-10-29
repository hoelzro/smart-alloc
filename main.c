#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "smart-alloc.h"

#define BIG_ALLOC 4000

static char *
smart_dup(struct smart_alloc *sa, const char *s)
{
    char *copy = smart_alloc(sa, strlen(s) + 1);
    if(copy) {
        strcpy(copy, s);
    }
    return copy;
}

void
basic_test(struct smart_alloc *sa)
{
    char *message = smart_dup(sa, "Hello, World!");

    printf("%s\n", message);
    smart_free(sa, message);

}

void
two_alloc(struct smart_alloc *sa)
{
    char *message1;
    char *message2;

    message1 = smart_dup(sa, "Hello");
    message2 = smart_dup(sa, "World");

    printf("%s, %s!\n", message1, message2);
    smart_free(sa, message1);
    smart_free(sa, message2);
}

void
oom(struct smart_alloc *sa)
{
    char *mem1 = smart_alloc(sa, BIG_ALLOC);
    char *mem2 = smart_alloc(sa, BIG_ALLOC);

    if(! mem2) {
        printf("%d: OK\n", __LINE__);
    } else {
        printf("%d: NOT OK\n", __LINE__);
    }

    smart_free(sa, mem1);
    smart_free(sa, mem2);
}

void
basic_free(struct smart_alloc *sa)
{
    char *mem = smart_alloc(sa, BIG_ALLOC);
    smart_free(sa, mem);
    mem = smart_alloc(sa, BIG_ALLOC);

    if(mem) {
        printf("%d: OK\n", __LINE__);
    } else {
        printf("%d: NOT OK\n", __LINE__);
    }

    // XXX free mem
}

void
free_null(struct smart_alloc *sa)
{
    smart_free(sa, NULL);
}

void checkered_alloc(struct smart_alloc *sa)
{
    int i = 0;
    int j;
    char *pointers[10];

    for(i = 0; i < 10; i++) {
        pointers[i] = smart_alloc(sa, 100);
        assert(pointers[i]);
        for(j = 0; j < 100; j++) {
            pointers[i][j] = 1;
        }
    }
    for(i = 1; i < 10; i += 2) {
        smart_free(sa, pointers[i]);
    }
    for(i = 1; i < 10; i += 2) {
        pointers[i] = smart_alloc(sa, 90);
        assert(pointers[i]);
        for(j = 0; j < 90; j++) {
            pointers[i][j] = 1;
        }
    }

    for(i = 0; i < 10; i++) {
        smart_free(sa, pointers[i]);
    }
}

int
main(int argc, char **argv)
{
    char memory[4096];
    struct smart_alloc *sa = smart_alloc_init(memory, sizeof(memory));

    basic_test(sa);
    two_alloc(sa);
    oom(sa);
    basic_free(sa);
    free_null(sa);
    checkered_alloc(sa);

    smart_alloc_destroy(sa);
    return 0;
}
