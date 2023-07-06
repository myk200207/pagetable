
CC = clang
CFLAGS = -std=c11 -Wall -pedantic
LDFLAGS = -lm

all : libmlpt.a

libmlpt.a : network.o
	ar rcs libmlpt.a network.o
	
network.o:network.c mlpt.h config.h
	$(CC) -c $(CFLAGS) network.c mlpt.h config.h $(LDFLAGS)

clean:
	rm --force libmlpt.a network.o