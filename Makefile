CFLAGS+=-g

main: main.c smart-alloc.o
	gcc $(CFLAGS) -o $@ $^

smart-alloc.o: smart-alloc.c smart-alloc.h
	gcc $(CFLAGS) -c -o $@ $<
