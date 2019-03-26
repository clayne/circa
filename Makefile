CC=clang
CFLAGS=-pipe
WFLAGS=-Weverything
LDFLAGS=-I. -L.

BUILD=-O2 -DNDEBUG -fno-omit-frame-pointer
FAST=-O3 -s -DNDEBUG -fomit-frame-pointer
SMALL=-Os -s -DNDEBUG -fomit-frame-pointer
DEBUG=-Og -g -fno-inline -fno-omit-frame-pointer

default: build

deps:
	-@cd lib/xxhash && $(MAKE) -s libxxhash.a >/dev/null
	-@ar -x lib/xxhash/libxxhash.a
	-@rm -rf *SYM*

build: deps
	$(CC) $(CFLAGS) $(BUILD) -c src/c/*.c $(LDFLAGS)
	ar -cvq libcirca.a *.o
	-@rm -rf *.dSYM *.o

fast: deps
	$(CC) $(CFLAGS) $(FAST) -c src/c/*.c $(LDFLAGS)
	ar -cvq libcirca.a *.o
	-@rm -rf *.dSYM *.o

small: deps
	$(CC) $(CFLAGS) $(SMALL) -c src/c/*.c $(LDFLAGS)
	ar -cvq libcirca.a *.o
	-@rm -rf *.dSYM *.o

debug: deps
	$(CC) $(CFLAGS) $(DEBUG) -c src/c/*.c $(LDFLAGS)
	ar -cvq libcirca.a *.o
	-@rm -rf *.dSYM *.o

disas: deps
	$(CC) $(CFLAGS) $(FAST) -S src/c/*.c $(DLFLAGS)

compare:
	$(MAKE) build
	mv libcirca.a libcirca-build
	$(MAKE) fast
	mv libcirca.a libcirca-fast
	$(MAKE) small
	mv libcirca.a libcirca-small
	$(MAKE) debug
	mv libcirca.a libcirca-debug

ex: debug
	$(CC) $(CFLAGS) $(DEBUG) -o nums.o    ex/map/nums.c      -lcirca $(LDFLAGS)
	$(CC) $(CFLAGS) $(DEBUG) -o sum.o     ex/seq/sum.c       -lcirca $(LDFLAGS)
	$(CC) $(CFLAGS) $(DEBUG) -o queue.o   ex/seq/queue.c     -lcirca $(LDFLAGS)
	$(CC) $(CFLAGS) $(DEBUG) -o seqkey.o  ex/seqmap/seqkey.c -lcirca $(LDFLAGS)
	$(CC) $(CFLAGS) $(DEBUG) -o slice.o   ex/txt/slice.c     -lcirca $(LDFLAGS)
	$(CC) $(CFLAGS) $(DEBUG) -o self.o    ex/txt/self.c      -lcirca $(LDFLAGS)
	$(CC) $(CFLAGS) $(DEBUG) -o fruits.o  ex/dict/fruits.c   -lcirca $(LDFLAGS)
	-@rm -rf *.a *.dSYM

test: debug
	$(CC) $(CFLAGS) $(DEBUG) tests/test.c -L. -I. -Ilib/snow -DSNOW_ENABLED -o test.o -lcirca -lm
	-@rm -rf *.dSYM

clean:
	-@cd lib/xxhash && $(MAKE) -s clean >/dev/null
	-@rm -rf *.o *.s *.out *.a *.dSYM src/h/*.gch libcirca-build libcirca-fast libcirca-small libcirca-debug
