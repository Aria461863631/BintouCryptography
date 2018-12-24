CC=gcc
INSTALL=install
prefix=/usr/local
CFLAGS=-Wall -ggdb -O2 -I. 
LDFLAGS=

.PHONY : all clean 

all: sha3sum

sha3.o: sha3.c
	$(CC) -c $(CFLAGS) -o $@ $<

sha3sum: sha3.o sha3sum.c
	$(CC) -o $@ $^ ${LDFLAGS}

clean:
	-rm -f *.o sha3test
