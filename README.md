# dataflow pass

A trival LLVM pass that instruct tainted memory access with [DFSsan](dfsan).

## Requirement:

- LLVM 3.8 or later
- clang
- compiler-rt (dataflow sanitizer)
- lit (for running test suite)

## Build:

```bash
mkdir build
cd build
cmake ..
make
```

## Run tests:

```bash
lit --show-tests dataflow/test
lit dataflow/test
```

sample tainted/untainted memory access statistics output as follow:
```
DF_RUNTIME: N/A:0: clean store 4 byte(s)
DF_RUNTIME: N/A:0: clean store 4 byte(s)
DF_RUNTIME: N/A:0: clean store 8 byte(s)
DF_RUNTIME: test_trivial.c:32: clean load 4 byte(s)
DF_RUNTIME: test_trivial.c:36: clean load 8 byte(s)
DF_RUNTIME: test_trivial.c:36: clean load 8 byte(s)
DF_RUNTIME: test_trivial.c:36: clean store 4 byte(s)
DF_RUNTIME: test_trivial.c:37: clean load 4 byte(s)
==32320==WARNING: DataFlowSanitizer: call to uninstrumented function bzero
DF_RUNTIME: test_trivial.c:43: clean load 4 byte(s)
DF_RUNTIME: test_trivial.c:43: clean store 4 byte(s)
DF_RUNTIME: test_trivial.c:44: clean load 4 byte(s)
DF_RUNTIME: test_trivial.c:45: clean load 4 byte(s)
DF_RUNTIME: label initialized
DF_RUNTIME: test_trivial.c:47: clean load 4 byte(s)
DF_RUNTIME: test_trivial.c:48: clean load 4 byte(s)
DF_RUNTIME: N/A:0: clean store 8 byte(s)
DF_RUNTIME: N/A:0: clean store 4 byte(s)
DF_RUNTIME: test_trivial.c:17: clean load 8 byte(s)
DF_RUNTIME: test_trivial.c:17: tainted load 1 byte(s)
DF_RUNTIME: test_trivial.c:17: tainted store 1 byte(s)
DF_RUNTIME: test_trivial.c:19: clean load 8 byte(s)
DF_RUNTIME: test_trivial.c:19: tainted load 1 byte(s)
DF_RUNTIME: test_trivial.c:19: tainted store 1 byte(s)
DF_RUNTIME: test_trivial.c:21: tainted load 1 byte(s)
DF_RUNTIME: test_trivial.c:21: tainted load 1 byte(s)
DF_RUNTIME: test_trivial.c:21: clean load 8 byte(s)
DF_RUNTIME: test_trivial.c:21: clean load 4 byte(s)
DF_RUNTIME: test_trivial.c:21: tainted store 1 byte(s)
DF_RUNTIME: test_trivial.c:23: clean load 8 byte(s)
DF_RUNTIME: test_trivial.c:23: clean load 4 byte(s)
DF_RUNTIME: test_trivial.c:23: clean store 1 byte(s)
DF_RUNTIME: total 12 load, 4 tainted, 8 clean
DF_RUNTIME: total 6 store, 3 tainted, 3 clean
DF_RUNTIME: test_trivial.c:52: clean store 4 byte(s)
DF_RUNTIME: test_trivial.c:53: clean load 4 byte(s)
DF_RUNTIME: total 12 load, 4 tainted, 8 clean
DF_RUNTIME: total 6 store, 3 tainted, 3 clean
```


[dfsan]:https://clang.llvm.org/docs/DataFlowSanitizer.html
