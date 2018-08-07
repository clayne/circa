#
# circa/Makefile | Circa's Makefile.
# https://github.com/davidgarland/circa
#

CC=clang
CFLAGS=
LDFLAGS=

#
# Env Handling
#

CFLAGS+=$(shell printenv CFLAGS)
LDFLAGS+=$(shell printenv LDFLAGS)

#
# Variables
#

MUTE=>/dev/null 2>/dev/null 

#
# Build Options
#

default: build

update:
	-@git fetch snow master
	-@git subtree pull --prefix lib/snow snow master --squash

build:
	-@$(CC) -c $(CFLAGS) *.h $(LDFLAGS)

test: tests/test.c
	-@$(CC) $(CFLAGS) tests/test.c -Ilib/snow -DSNOW_ENABLED -g -o test.o

examples: examples/seq/odd.c
	-@$(CC) $(CFLAGS) examples/seq/odd.c -o odd.o $(LDFLAGS)

clean:
	-@rm -rf *.o *.gch $(MUTE)
