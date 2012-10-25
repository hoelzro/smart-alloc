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

int
main(int argc, char **argv)
{
    basic_test();
    return 0;
}
