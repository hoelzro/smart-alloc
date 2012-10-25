CFLAGS+=-Wall -Wextra -Wunused -Werror -Wpedantic

main: main.c smart-alloc.o
	gcc -o $@ $^

smart-alloc.o: smart-alloc.c smart-alloc.h
	gcc -c -o $@ $<
