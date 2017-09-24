CC=clang
CFLAGS=-fsanitize=dataflow -g -std=c11
LDFLAGS=-fsanitize=dataflow
BUILD=../build
RUNTIME=../dataflow/runtime

sources=$(wildcard example_*.c)
objects=$(patsubst %.c, %.o, $(sources))
targets=$(patsubst %.c, %, $(sources))

all: $(targets)

runtime.o: $(RUNTIME)/runtime.c $(RUNTIME)/runtime.h
	$(CC) $(CFLAGS) -I$(RUNTIME) -c $< -o $@

$(objects): %.o:%.c
	$(CC) $(CFLAGS) -I$(RUNTIME) -Xclang -load -Xclang $(BUILD)/dataflow/libLoadStorePass.so -c $< -o $@
$(targets): %: %.o runtime.o
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	rm -f $(targets) $(objects) runtime.o
.PHONY: clean all
