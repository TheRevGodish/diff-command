CFLAGS	= -Wall -g -O0 -Iinclude
CC		= gcc
VPATH	= src
SRCS 	= diff.c file.c hash.c matrix.c myers.c display.c utils.c
TARGET 	= diff
OBJS    = $(addprefix build/, $(SRCS:.c=.o))
TEST_SRCS = test/test_file.c
TEST_BINS = $(TEST_SRCS:.c=)
VALGRIND= valgrind --malloc-fill=22 --free-fill=33
COVERAGE= NO

.PHONY	= all clean run doc test

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

# règle pour compiler un test
test/%: test/%.c build/file.o build/hash.o build/utils.o
	$(CC) $(CFLAGS) $^ -o $@

# règle pour lancer tous les tests
test: $(TEST_BINS)
	@for t in $(TEST_BINS); do \
		echo "Running $$t..."; \
		./$$t; \
	done

## TODO: cible pour le /build et le /bin
