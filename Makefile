CC=clang
CFLAGS=-O2 -pipe
LDFLAGS=

CDEBUG=-Og -fsanitize=undefined -fno-omit-frame-pointer
CRELEASE=-O3 -DNDEBUG
CNATIVE=-march=native -flto

default: build

build:
	$(CC) -c $(CFLAGS) src/c/*.c $(LDFLAGS)
	ar -cvq libcirca-a.a *.o
	-@rm -rf *.o

debug:
	$(CC) -c $(CFLAGS) $(CDEBUG) src/c/*.c $(LDFLAGS)
	ar -cvq libcirca-a.a *.o
	-@rm -rf *.o

release:
	$(CC) -c $(CFLAGS) $(CRELEASE) src/c/*.c $(LDFLAGS)
	ar -cvq libcirca-a.a *.o
	-@rm -rf *.o

native:
	$(CC) -c $(CFLAGS) $(CNATIVE) src/c/*.c $(LDFLAGS)
	ar -cvq libcirca-a.a *.o
	-@rm -rf *.o

test: debug

clean:
	-@rm -rf *.o *.out *.a *.so
