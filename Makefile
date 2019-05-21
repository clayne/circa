CC=cc
CFLAGS=
LDFLAGS=-Isrc/h

default: build

build:
	$(CC) -c $(CFLAGS) src/c/*.c $(LDFLAGS)

clean:
	-@rm -f *.o *.out *.a *.so
