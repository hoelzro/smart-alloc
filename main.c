#include <stdio.h>
#include <string.h>
#include "smart-alloc.h"

void
basic_test(void)
{
    char *message = smart_alloc(sizeof("Hello, World!"));

    strcpy(message, "Hello, World!");
    printf("%s\n", message);
    /*smart_free(message);*/

}

int
main(int argc, char **argv)
{
    basic_test();
    return 0;
}
