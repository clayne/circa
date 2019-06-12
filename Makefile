CC=clang
WFLAGS=-pedantic -Wall
CFLAGS=$(WFLAGS) -pipe -std=c11
LDFLAGS=-I. -L.

CFLAGS_FAST= $(CFLAGS) -O3 -s -DNDEBUG
CFLAGS_SMALL=$(CFLAGS) -Os -s -DNDEBUG
CFLAGS_BUILD=$(CFLAGS) -O2    -fno-omit-frame-pointer
CFLAGS_DEBUG=$(CFLAGS) -Og -g -fno-omit-frame-pointer
CFLAGS_SANITIZED=$(CFLAGS_DEBUG) -fsanitize=undefined -fsanitize=address -fsanitize=leak

CFLAGS_EX=-std=gnu11 -Wno-everything

default: build

#
# Dependency Options
#

deps:
	-@cd lib/xxhash && $(MAKE) -s libxxhash.a >/dev/null
	-@ar -x lib/xxhash/libxxhash.a

#
# Build Options
#

fast: deps
	$(CC) -c $(CFLAGS_FAST) src/c/*.c $(LDFLAGS)
	ar -cvq libcirca.a *.o
	-@rm -f *.o

small: deps
	$(CC) -c $(CFLAGS_SMALL) src/c/*.c $(LDFLAGS)
	ar -cvq libcirca.a *.o
	-@rm -f *.o

build: deps
	$(CC) -c $(CFLAGS_BUILD) src/c/*.c $(LDFLAGS)
	ar -cvq libcirca.a *.o
	-@rm -f *.o

debug: deps
	$(CC) -c $(CFLAGS_DEBUG) src/c/*.c $(LDFLAGS)
	ar -cvq libcirca.a *.o
	-@rm -f *.o

sanitized: deps
	$(CC) -c $(CFLAGS_SANITIZED) src/c/*.c $(LDFLAGS)
	ar -cvq libcirca.a *.o
	-@rm -f *.o

#
# Tests
#

examples: debug
	$(CC) $(CFLAGS_DEBUG) $(CFLAGS_EX) ex/seq.c    -o seq.o    -lcirca $(LDFLAGS)
	$(CC) $(CFLAGS_DEBUG) $(CFLAGS_EX) ex/txt.c    -o txt.o    -lcirca $(LDFLAGS)
	$(CC) $(CFLAGS_DEBUG) $(CFLAGS_EX) ex/map.c    -o map.o    -lcirca $(LDFLAGS)
	$(CC) $(CFLAGS_DEBUG) $(CFLAGS_EX) ex/set.c    -o set.o    -lcirca $(LDFLAGS)
	$(CC) $(CFLAGS_DEBUG) $(CFLAGS_EX) ex/seqmap.c -o seqmap.o -lcirca $(LDFLAGS)

examples_sanitized: sanitized
	$(CC) $(CFLAGS_SANITIZED) $(CFLAGS_EX) ex/seq.c -o seq.o       -lcirca $(LDFLAGS)
	$(CC) $(CFLAGS_SANITIZED) $(CFLAGS_EX) ex/txt.c -o txt.o       -lcirca $(LDFLAGS)
	$(CC) $(CFLAGS_SANITIZED) $(CFLAGS_EX) ex/map.c -o map.o       -lcirca $(LDFLAGS)
	$(CC) $(CFLAGS_SANITIZED) $(CFLAGS_EX) ex/set.c -o set.o       -lcirca $(LDFLAGS)
	$(CC) $(CFLAGS_SANITIZED) $(CFLAGS_EX) ex/seqmap.c -o seqmap.o -lcirca $(LDFLAGS)

examples_header:
	$(CC) $(CFLAGS_DEBUG) $(CFLAGS_EX) -DCIRCA_HEADER_ONLY ex/seq.c    -o seq.o $(LDFLAGS)
	$(CC) $(CFLAGS_DEBUG) $(CFLAGS_EX) -DCIRCA_HEADER_ONLY ex/txt.c    -o txt.o $(LDFLAGS)
	$(CC) $(CFLAGS_DEBUG) $(CFLAGS_EX) -DCIRCA_HEADER_ONLY ex/map.c    -o map.o $(LDFLAGS)
	$(CC) $(CFLAGS_DEBUG) $(CFLAGS_EX) -DCIRCA_HEADER_ONLY ex/set.c    -o set.o $(LDFLAGS)
	$(CC) $(CFLAGS_DEBUG) $(CFLAGS_EX) -DCIRCA_HEADER_ONLY ex/seqmap.c -o seqmap.o $(LDFLAGS)

test: debug
	$(CC) $(CFLAGS_DEBUG) $(CFLAGS_EX) -fPIC tests/test.c -I. -L. -Ilib/snow -DSNOW_ENABLED -o test.o -lcirca -lm

test_sanitized: sanitized
	$(CC) $(CFLAGS_SANITIZED) $(CFLAGS_EX) -fPIC tests/test.c -I. -L. -Ilib/snow -DSNOW_ENABLED -o test.o -lcirca -lm

test_header:
	$(CC) $(CFLAGS_DEBUG) $(CFLAGS_EX) -DCIRCA_HEADER_ONLY tests/test.c -I. -L. -Ilib/snow -DSNOW_ENABLED -o test.o -lm

#
# Cleanup
#

clean:
	-@cd lib/xxhash && $(MAKE) -s clean >/dev/null
	-@rm -f $(BIN) *.o *.out *.a *.so *.data *.old

