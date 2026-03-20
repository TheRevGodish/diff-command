CFLAGS	= -Wall -g -O0 -Ilib ##-Itests
CC		= gcc
VPATH	= src
SRCS 	= diff.c readfile.c hash.c matrix.c myers.c display.c utils.c
TARGET 	= diff
OBJS    = $(addprefix build/, $(SRCS:.c=.o))
VALGRIND= valgrind --malloc-fill=22 --free-fill=33
COVERAGE= NO

.PHONY	= all clean run

all: $(TARGET)

clean:
	rm -f $(OBJS) bin/$(TARGET)

doc: 
	doxygen Doxyfile

run: $(TARGET)
	./bin/$(TARGET) ./resources/data1 ./resources/data2

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o bin/$@ $^

build/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

##build/%.o: test
	##$(CC) $(CFLAGS) -c $< -o $@

## to enable coverage, set COVERAGE=YES
##test: test/test_mfile test/test_library
	##cp tests/data.txt tests/lib.txt build/
##build/test_mfile: build/test_mfile.o build/mfile.o
	##$(CC) $(LDFLAGS) $^ -o $@
##build/test_library: build/test_library.o build/library.o build/mfile.o
	##$(CC) $(LDFLAGS) $^ -o $@
