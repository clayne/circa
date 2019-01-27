CC=clang
CFLAGS=-pipe
WFLAGS=-Weverything
LDFLAGS=-I. -L.

BUILD=-O2 -DNDEBUG -fno-omit-frame-pointer
FAST=-O3 -s -DNDEBUG -fomit-frame-pointer
SMALL=-Os -s -DNDEBUG -fomit-frame-pointer
DEBUG=-Og -g -fno-inline -fno-omit-frame-pointer

build:
	$(CC) $(CFLAGS) $(BUILD) -c src/c/*.c $(LDFLAGS)
	ar -cvq libcirca.a *.o 
	-@rm -rf *.dSYM *.o

fast:
	$(CC) $(CFLAGS) $(FAST) -c src/c/*.c $(LDFLAGS)
	ar -cvq libcirca.a *.o
	-@rm -rf *.dSYM *.o

small:
	$(CC) $(CFLAGS) $(SMALL) -c src/c/*.c $(LDFLAGS)
	ar -cvq libcirca.a *.o
	-@rm -rf *.dSYM *.o

debug:
	$(CC) $(CFLAGS) $(DEBUG) -c src/c/*.c $(LDFLAGS)
	ar -cvq libcirca.a *.o
	-@rm -rf *.dSYM *.o

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
	$(CC) $(CFLAGS) $(DBG) -o set.o    ex/map/set.c     -lcirca $(LDFLAGS)
	$(CC) $(CFLAGS) $(DBG) -o sum.o    ex/seq/sum.c     -lcirca $(LDFLAGS)
	$(CC) $(CFLAGS) $(DBG) -o fruits.o ex/dict/fruits.c -lcirca $(LDFLAGS)
	$(CC) $(CFLAGS) $(DBG) -o oddsq.o  ex/seq/oddsq.c   -lcirca $(LDFLAGS)
	$(CC) $(CFLAGS) $(DBG) -o self.o   ex/txt/self.c    -lcirca $(LDFLAGS)
	-@rm -rf *.a *.dSYM

test: debug
	$(CC) $(CFLAGS) $(DBG) tests/test.c -L. -I. -Ilib/snow -DSNOW_ENABLED -o test.o -lcirca
	-@rm -rf *.dSYM

clean:
	-@rm -rf *.o *.out *.a *.dSYM src/h/*.gch libcirca-build libcirca-fast libcirca-small libcirca-debug
