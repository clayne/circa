CC=clang
CFLAGS=-pipe
WFLAGS=-Weverything
LDFLAGS=-I. -L.

build:
	$(CC) $(CFLAGS) $(WFLAGS) -O2 -c src/c/*.c $(LDFLAGS)
	ar -cvq libcirca.a *.o 
	rm *.o

debug:
	$(CC) $(CFLAGS) $(WFLAGS) -Og -fno-omit-frame-pointer -fsanitize=undefined -c src/c/*.c $(LDFLAGS)
	ar -cvq libcirca.a *.o
	rm *.o

ex: build
	$(CC) $(CFLAGS) -o oddsq.o ex/seq/oddsq.c -lcirca $(LDFLAGS)
	$(CC) $(CFLAGS) -o self.o  ex/txt/self.c  -lcirca $(LDFLAGS)
	rm *.a

test: debug
	$(CC) $(CFLAGS) -Og -fno-omit-frame-pointer -fsanitize=undefined tests/test.c -L. -I. -Ilib/snow -DSNOW_ENABLED -g -o test.o -lcirca

clean:
	-@rm -rf *.o *.out *.a
