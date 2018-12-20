CC=clang
CFLAGS=-pipe
WFLAGS=-Weverything
LDFLAGS=-I. -L.

build:
	$(CC) $(CFLAGS) $(WFLAGS) -c src/c/*.c $(LDFLAGS)
	ar -cvq libcirca.a *.o 
	rm *.o

ex: build
	$(CC) $(CFLAGS) -o oddsq.o ex/seq/oddsq.c -lcirca $(LDFLAGS)
	$(CC) $(CFLAGS) -o self.o  ex/txt/self.c  -lcirca $(LDFLAGS)
	rm *.a

clean:
	-@rm -rf *.o *.out *.a
