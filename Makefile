CC=gcc
CFLAGS=-O2

cscroll: cscroll.c
	$(CC) -o cscroll cscroll.c && \
	cp ./cscroll /usr/local/bin/

clean:
	rm -f ./cscroll

uninstall:
	rm /usr/local/bin/cscroll
