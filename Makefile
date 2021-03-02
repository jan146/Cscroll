CC=gcc
CFLAGS=-O2

cscroll: cscroll.c
	$(CC) -o cscroll cscroll.c

clean:
	rm -f ./cscroll
