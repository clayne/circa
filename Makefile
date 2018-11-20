CC=cc
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
	$(CC) $(CFLAGS) $(CDEBUG) tests/test.c -I. -Ilib/snow -DSNOW_ENABLED -g -L. -lcirca-a $(LDFLAGS)
	-@./a.out
	#-@rm -rf *.a *.out

clean:
	-@rm -rf *.o *.out *.a *.so
