#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "smart-alloc.h"

#define BIG_ALLOC 4000

static char *
smart_dup(const char *s)
{
    char *copy = smart_alloc(strlen(s) + 1);
    if(copy) {
        strcpy(copy, s);
    }
    return copy;
}

void
basic_test(void)
{
    char *message = smart_dup("Hello, World!");

    printf("%s\n", message);
    smart_free(message);

}

void
two_alloc(void)
{
    char *message1;
    char *message2;

    message1 = smart_dup("Hello");
    message2 = smart_dup("World");

    printf("%s, %s!\n", message1, message2);
    smart_free(message1);
    smart_free(message2);
}

void
oom(void)
{
    char *mem1 = smart_alloc(BIG_ALLOC);
    char *mem2 = smart_alloc(BIG_ALLOC);

    if(! mem2) {
        printf("%d: OK\n", __LINE__);
    } else {
        printf("%d: NOT OK\n", __LINE__);
    }

    smart_free(mem1);
    smart_free(mem2);
}

void
basic_free(void)
{
    char *mem = smart_alloc(BIG_ALLOC);
    smart_free(mem);
    mem = smart_alloc(BIG_ALLOC);

    if(mem) {
        printf("%d: OK\n", __LINE__);
    } else {
        printf("%d: NOT OK\n", __LINE__);
    }
}

void
free_null(void)
{
    smart_free(NULL);
}

void checkered_alloc(void)
{
    int i = 0;
    int j;
    char *pointers[10];

    for(i = 0; i < 10; i++) {
        pointers[i] = smart_alloc(100);
        assert(pointers[i]);
        for(j = 0; j < 100; j++) {
            pointers[i][j] = 1;
        }
    }
    for(i = 1; i < 10; i += 2) {
        smart_free(pointers[i]);
    }
    for(i = 1; i < 10; i += 2) {
        printf("i = %d\n", i);
        pointers[i] = smart_alloc(90);
        assert(pointers[i]);
        for(j = 0; j < 90; j++) {
            pointers[i][j] = 1;
        }
    }

    for(i = 0; i < 10; i++) {
        smart_free(pointers[i]);
    }
}

int
main(int argc, char **argv)
{
    basic_test();
    two_alloc();
    oom();
    basic_free();
    free_null();
    checkered_alloc();
    return 0;
}
