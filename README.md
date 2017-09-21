# dataflow pass

A trivial LLVM pass that instruct tainted memory access with [DFSsan](dfsan).

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
## Simple example

As for target source `example/target.c`
```text
1	#include <string.h>
2	#include "runtime.h"
3	#define MAXSIZE 256
4	int main()
5	{
6	    char src[MAXSIZE] = {1};
7	    char dst[MAXSIZE] = {0};
8	    df_init(src, MAXSIZE);
9	    memcpy(dst, src, MAXSIZE/2);
10	    char temp;
11	    temp = dst[0];
12	    temp = dst[MAXSIZE/2 - 1];
13	    temp = dst[MAXSIZE/2];
14	    df_stat();
15	    return 0;
16	}
```

1). compile target source with clang
```bash
cd example
clang -g -fsanitize=dataflow -std=c11 -Xclang -load \
-Xclang ../build/dataflow/libLoadStorePass.so -I../dataflow/runtime -c target.c -o target.o
```

2). compile runtime library
```bash
clang -g -fsanitize=dataflow ../dataflow/runtime/runtime.c \
-I../dataflow/runtime -c -o runtime.o
```

3). and link them together
```bash
clang -fsanitize=dataflow target.o runtime.o -o target
```

Sample output of runtime checking:
```text
DF_RUNTIME: N/A:0: clean store 4 byte(s)
DF_RUNTIME: target.c:6: clean store 256 byte(s)
DF_RUNTIME: target.c:6: clean store 1 byte(s)
DF_RUNTIME: target.c:7: clean store 256 byte(s)
DF_RUNTIME: label initialized
DF_RUNTIME: target.c:9: tainted store 128 byte(s)
DF_RUNTIME: target.c:9: tainted load 128 byte(s)
DF_RUNTIME: target.c:11: tainted load 1 byte(s)
DF_RUNTIME: target.c:11: tainted store 1 byte(s)
DF_RUNTIME: target.c:12: tainted load 1 byte(s)
DF_RUNTIME: target.c:12: tainted store 1 byte(s)
DF_RUNTIME: target.c:13: clean load 1 byte(s)
DF_RUNTIME: target.c:13: clean store 1 byte(s)
DF_RUNTIME: total 4 load, 3 tainted, 1 clean
DF_RUNTIME: total 4 store, 3 tainted, 1 clean
```


## Run tests:

```bash
lit --show-tests dataflow/test
lit -j8 dataflow/test
```



[dfsan]:https://clang.llvm.org/docs/DataFlowSanitizer.html
