CC=clang
CFLAGS=-pipe
WFLAGS=
DFLAGS=-Og -fno-omit-frame-pointer -DCIRCA_DBG
RFLAGS=-O2 -DNDEBUG
LDFLAGS=

ifeq ($(CC),clang)
	WFLAGS+=-Weverything
else
	WFLAGS+=-Wall -Wextra -Wpedantic
endif

ifeq ($(CC),clang)
  UBSAN=-fsanitize=undefined
  MSAN=-fsanitize=memory -fPIE
  ASAN=-fsanitize=address
  LSAN=-fsanitize=leak
  TSAN=-fsanitize=thread
else
	UBSAN=-fsanitize=undefined
	MSAN=
	ASAN=-fsanitize=address
	LSAN=-fsanitize=leak
	TSAN=-fsanitize=thread
endif

SNOW=-I. -Ilib/snow -DSNOW_ENABLED -g -L.
CLEAN=*.o *.out *.a *.so *.dSYM

default: build

#
# Main Builds
#

build:
	$(CC) $(CFLAGS) $(DFLAGS) $(WFLAGS) -c src/c/*.c $(LDFLAGS)
	ar -cvq libcirca.a *.o
	rm *.o

build_test:
	$(CC) $(CFLAGS) $(DFLAGS) $(WFLAGS) -c src/c/*.c $(LDFLAGS)
	-@ar -cvq libcirca.a *.o
	-@rm *.o

#
# Sanitizer Builds
#

build_ubsan:
	-@$(CC) $(CFLAGS) $(DFLAGS) $(UBSAN) -c src/c/*.c $(LDFLAGS)
	-@ar -cvq libcirca.a *.o >/dev/null
	-@rm *.o

build_msan:
	-@$(CC) $(CFLAGS) $(DFLAGS) $(MSAN) -c src/c/*.c $(LDFLAGS)
	-@ar -cvq libcirca.a *.o >/dev/null
	-@rm *.o

build_asan:
	-@$(CC) $(CFLAGS) $(DFLAGS) $(ASAN) -c src/c/*.c $(LDFLAGS)
	-@ar -cvq libcirca.a *.o >/dev/null
	-@rm *.o

build_lsan:
	-@$(CC) $(CFLAGS) $(DFLAGS) $(LSAN) -c src/c/*.c $(LDFLAGS)
	-@ar -cvq libcirca.a *.o >/dev/null
	-@rm *.o

build_tsan:
	-@$(CC) $(CFLAGS) $(DFLAGS) $(TSAN) -c src/c/*.c $(LDFLAGS)
	-@ar -cvq libcirca.a *.o >/dev/null
	-@rm *.o

#
# Main Test
#

test: build_test
	$(CC) $(CFLAGS) $(DFLAGS) $(WFLAGS) tests/test.c $(SNOW) -lcirca $(LDFLAGS)
	-@echo ""
	-@./a.out
	-@echo ""
	-@rm -rf $(CLEAN)

valgrind: build_test
	$(CC) $(CFLAGS) $(DFLAGS) $(WFLAGS) tests/test.c $(SNOW) -lcirca $(LDFLAGS)
	-@echo ""
	-@valgrind --leak-check=full ./a.out
	-@echo ""
	-@rm -rf $(CLEAN)

#
# Sanitizer Tests
#

ubsan: build_ubsan
	-@tput setaf 3; echo -e "\n -- UBSan -- \n"; tput sgr0
	-@$(CC) $(CFLAGS) $(DFLAGS) $(UBSAN) tests/test.c $(SNOW) -lcirca $(LDFLAGS)
	-@./a.out >/dev/null
	-@rm -rf $(CLEAN)

msan: build_msan
	-@tput setaf 3; echo -e "\n -- MSan -- \n"; tput sgr0
	-@$(CC) $(CFLAGS) $(DFLAGS) $(MSAN) tests/test.c $(SNOW) -lcirca $(LDFLAGS)
	-@./a.out >/dev/null
	-@rm -rf $(CLEAN)

asan: build_asan
	-@tput setaf 3; echo -e "\n -- ASan -- \n"; tput sgr0
	-@$(CC) $(CFLAGS) $(DFLAGS) $(ASAN) tests/test.c $(SNOW) -lcirca $(LDFLAGS)
	-@./a.out >/dev/null
	-@rm -rf $(CLEAN)

lsan: build_lsan
	-@tput setaf 3; echo -e "\n -- LSan -- \n"; tput sgr0
	-@$(CC) $(CFLAGS) $(DFLAGS) $(LSAN) tests/test.c $(SNOW) -lcirca $(LDFLAGS)
	-@./a.out >/dev/null
	-@rm -rf $(CLEAN)

tsan: build_tsan
	-@tput setaf 3; echo -e "\n -- TSan -- \n"; tput sgr0
	-@$(CC) $(CFLAGS) $(DFLAGS) $(TSAN) tests/test.c $(SNOW) -lcirca $(LDFLAGS)
	-@./a.out >/dev/null
	-@rm -rf $(CLEAN)

san:
	-@$(MAKE) --no-print-directory ubsan
	-@$(MAKE) --no-print-directory msan
	-@$(MAKE) --no-print-directory asan
	-@$(MAKE) --no-print-directory lsan
	-@$(MAKE) --no-print-directory tsan

clean:
	rm -rf $(CLEAN)
