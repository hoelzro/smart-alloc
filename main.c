#include <stdio.h>
#include <string.h>
#include "smart-alloc.h"

int
main(int argc, char **argv)
{
    char *message = smart_alloc(sizeof("Hello, World!"));

    strcpy(message, "Hello, World!");
    printf("%s\n", message);
    /*smart_free(message);*/

    return 0;
}
