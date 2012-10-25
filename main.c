#include <stdio.h>
#include <string.h>
#include "smart-alloc.h"

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
basic_free(void)
{
}

int
main(int argc, char **argv)
{
    basic_test();
    two_alloc();
    basic_free();
    return 0;
}
