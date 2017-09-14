CC=clang
CFLAGS=-fsanitize=dataflow -g -std=c11
LDFLAGS=-fsanitize=dataflow
BUILD=../../build

sources=$(wildcard test_*.c)
objects=$(patsubst %.c, %.o, $(sources))
targets=$(patsubst %.c, %, $(sources))

all: $(targets)

runtime.o: runtime.c runtime.h
	$(CC) $(CFLAGS) -c $< -o $@

#test_trivial.o: test_trivial.c
#	$(CC) $(CFLAGS) -Xclang -load -Xclang $(BUILD)/dataflow/libLoadStorePass.so -c $< -o $@
#test_trivial: runtime.o test_trivial.o
#	$(CC) $(LDFLAGS) $^ -o $@
$(objects): %.o:%.c
	$(CC) $(CFLAGS) -Xclang -load -Xclang $(BUILD)/dataflow/libLoadStorePass.so -c $< -o $@
$(targets): %: %.o runtime.o
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	rm -f $(targets) $(objects) runtime.o
.PHONY: clean all
