#                                         #######################
# Makefile | The Circa Core Library for C #######################
# https://github.com/davidgarland/circa   #######################
#                                         #######################

CC=clang
CFLAGS=-Og
LDFLAGS=

CLEANUP=test odd sum read

MUTE=>/dev/null 2>/dev/null || true

default: test

build:
	-@$(CC) $(CFLAGS) ex/odd.c -o odd $(CFLAGS)
	-@$(CC) $(CFLAGS) ex/read.c -o read $(CFLAGS)
	-@$(CC) $(CFLAGS) ex/sum.c -o sum $(CFLAGS)

test:
	-@$(CC) $(CFLAGS) tests/test.c -Ilib/snow -DSNOW_ENABLED -g -o test

clean:
	-@rm -rf $(CLEANUP) *.o *.out *.a *.so *.gch src/*.gch src/core/*.gch $(MUTE)
