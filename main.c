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
}

void
free_null(void)
{
    smart_free(NULL);
}

int
main(int argc, char **argv)
{
    basic_test();
    two_alloc();
    oom();
    basic_free();
    free_null();
    return 0;
}
