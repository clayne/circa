#################################################################
# Makefile | The Circa Core Library for C #######################
# Author: David Garland (https://github.com/davidgarland/circa) #
#################################################################

CC=clang
CFLAGS=-Og
LDFLAGS=

MUTE=>/dev/null 2>/dev/null || true

default: build

build:
	-@$(CC) $(CFLAGS) tests/test.c -o test $(LDFLAGS)

clean:
	-@rm -rf test *.o *.out *.a *.so *.gch src/*.gch src/core/*.gch $(MUTE)
