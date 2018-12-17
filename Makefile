CC=clang
CFLAGS=-pipe
WFLAGS=-Weverything -Wno-cast-align -Wno-padded -Wno-language-extension-token -Wno-unused-command-line-argument
LDFLAGS=-I. -L.

build:
	$(CC) $(CFLAGS) $(WFLAGS) -c src/c/*.c $(LDFLAGS)
	ar -cvq libcirca.a *.o 
	rm *.o

ex: build
	$(CC) $(CFLAGS) $(WFLAGS) -o odd.o  ex/seq/oddsq.c -lcirca $(LDFLAGS)
	$(CC) $(CFLAGS) $(WFLAGS) -o self.o ex/txt/self.c  -lcirca $(LDFLAGS)
	rm *.a

clean:
	-@rm -rf *.o *.out *.a
