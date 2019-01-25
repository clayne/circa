CC=clang
CFLAGS=-pipe
WFLAGS=-Weverything
LDFLAGS=-I. -L.

DBG=-Og -g -fno-omit-frame-pointer

build:
	$(CC) $(CFLAGS) $(WFLAGS) -O2 -c src/c/*.c $(LDFLAGS)
	ar -cvq libcirca.a *.o 
	-@rm -rf *.dSYM *.o

debug:
	$(CC) $(CFLAGS) $(WFLAGS) $(DBG) -c src/c/*.c $(LDFLAGS)
	ar -cvq libcirca.a *.o
	-@rm -rf *.dSYM *.o

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
	-@rm -rf *.o *.out *.a *.dSYM
