#
# circa/Makefile | Circa's Makefile.
# https://github.com/davidgarland/circa
#

CC=cc
CDBG=-fsanitize=undefined -Og -DCIRCA_DBG
CFAST=-Os -DNDEBUG
CFLAGS=-pipe
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
	-@git remote add -f snow https://github.com/mortie/snow
	-@git fetch snow master
	-@git subtree pull --prefix lib/snow snow master --squash

build:
	-@$(CC) -c $(CFLAGS) *.h $(LDFLAGS)

test: tests/test.c
	-@$(CC) $(CFLAGS) $(CDBG) tests/test.c -Ilib/snow -DSNOW_ENABLED -g -o test.o

examples:
	-@$(CC) $(CFLAGS) $(CFAST) ex/str/readme.c  -o readme.o $(LDFLAGS)
	-@$(CC) $(CFLAGS) $(CFAST) ex/seq/odd.c     -o odd.o    $(LDFLAGS)
	-@$(CC) $(CFLAGS) $(CFAST) ex/dict/fruits.c -o fruits.o $(LDFLAGS)

clean:
	-@rm -rf *.o *.gch $(MUTE)
