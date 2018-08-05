#
# circa/Makefile | Circa's Makefile.
# https://github.com/davidgarland/circa
#

CC=clang
CFLAGS=-Weverything
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

build:
	-@$(CC) -c $(CFLAGS) *.h $(LDFLAGS)

ex: ex/seq/odd.c 
	-@$(CC) $(CFLAGS) ex/seq/odd.c -o odd.o $(LDFLAGS)

clean:
	-@rm -rf *.o *.gch $(MUTE)
