CFLAGS=-Wall -g -O0 -I. -Isrc -Itests
LDFLAGS=
CC=gcc
VALGRIND=valgrind --malloc-fill=22 --free-fill=33 
COVERAGE=NO

ifeq ($(COVERAGE),YES)
	CFLAGS+=--coverage -fprofile-abs-path	
	LDFLAGS+=--coverage
$(info code covering analysis enabled)
endif

.PHONY=all clean

## build 'program' and 'cat' in build directory
all: build/program build/cat

## remove every files in build and docs directories
clean:
	rm -rf build/* docs

## build the documentation in the docs directory
doc: 
	doxygen Doxyfile

## build and run the tests
## to enable coverage, set COVERAGE=YES
test: build/test_mfile build/test_library
	cp tests/data.txt tests/lib.txt build/
# following command is a little bit complexe: we move to build
# directory and then we run each test without build/ as we are
# in build directory
	@echo "*** RUNNING TESTS ***"
	@cd build ;\
	for t in $^; \
	do \
		echo TEST: $$t;\
		$(VALGRIND) ./$${t#build/} ;\
	done
ifeq ($(COVERAGE),YES)
	@echo "*** COVERING ANALYSIS ***"
	cd build ; gcov *gcno
endif

## links:

build/program: build/program.o build/library.o build/mfile.o
	$(CC) $(LDFLAGS) $^ -o $@

build/test_mfile: build/test_mfile.o build/mfile.o
	$(CC) $(LDFLAGS) $^ -o $@
build/test_library: build/test_library.o build/library.o build/mfile.o
	$(CC) $(LDFLAGS) $^ -o $@
build/cat: build/cat.o build/mfile.o
	$(CC) $(LDFLAGS) $^ -o $@

## dependencies

build/library.o: src/library.c src/library_impl.h src/library.h src/mfile.h
build/mfile.o: src/mfile.c src/mfile.h
build/program.o: src/program.c src/library.h
build/test_library.o: test test
build/test_mfile.o: test test src/mfile.h
build/cat.o: examples/cat.c src/mfile.h

build/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: test
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: examples/%.c
	$(CC) $(CFLAGS) -c $< -o $@
