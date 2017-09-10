# dataflow pass

A trival LLVM pass that instruct tainted memory access with [DFSsan](dfsan).

## Requirement:

- LLVM 3.8 or later
- clang
- compiler-rt

## Build:

```bash
mkdir build
cd build
cmake ..
make
```

## Run:

```bash
cd dataflow/test
make clean && make
./a.out /path/to/tainted_file
```

sample tainted/untainted memory access statistics output as follow:
```
[0] runtime store check: 0x7ffd93c89cb8:4 in N/A:0
[0] runtime store check: 0x7ffd93c89cb4:4 in N/A:0
[0] runtime store check: 0x7ffd93c89ca8:8 in N/A:0
[0] runtime load check: 0x7ffd93c89cb4:4 in example.c:24
[0] runtime load check: 0x7ffd93c89ca8:8 in example.c:28
[0] runtime load check: 0x7ffd93c89dd0:8 in example.c:28
[0] runtime store check: 0x7ffd93c89ca4:4 in example.c:28
[0] runtime load check: 0x7ffd93c89ca4:4 in example.c:29
[0] runtime store check: 0x7ffd93c894a0:2049 in example.c:34
[0] runtime load check: 0x7ffd93c89ca4:4 in example.c:35
[0] runtime store check: 0x7ffd93c8949c:4 in example.c:35
[0] runtime load check: 0x7ffd93c8949c:4 in example.c:36
read 1761 from file
[0] runtime load check: 0x7ffd93c8949c:4 in example.c:37
label initialized
[0] runtime load check: 0x7ffd93c8949c:4 in example.c:39
[0] runtime load check: 0x7ffd93c8949c:4 in example.c:40
[0] runtime store check: 0x7ffd93c89130:8 in N/A:0
[0] runtime store check: 0x7ffd93c8912c:4 in N/A:0
[0] runtime load check: 0x7ffd93c89130:8 in example.c:12
[1] runtime load check: 0x7ffd93c894a0:1 in example.c:12
[1] runtime store check: 0x7ffd93c8912b:1 in example.c:12
[0] runtime load check: 0x7ffd93c89130:8 in example.c:13
[1] runtime load check: 0x7ffd93c894a9:1 in example.c:13
[1] runtime store check: 0x7ffd93c8912a:1 in example.c:13
[1] runtime load check: 0x7ffd93c8912b:1 in example.c:14
[1] runtime load check: 0x7ffd93c8912a:1 in example.c:14
[0] runtime load check: 0x7ffd93c89130:8 in example.c:14
[0] runtime load check: 0x7ffd93c8912c:4 in example.c:14
[1] runtime store check: 0x7ffd93c89b81:1 in example.c:14
[0] runtime load check: 0x7ffd93c89130:8 in example.c:15
[0] runtime load check: 0x7ffd93c8912c:4 in example.c:15
[0] runtime store check: 0x7ffd93c89b81:1 in example.c:15
[0] runtime store check: 0x7ffd93c89cb8:4 in example.c:43
[0] runtime load check: 0x7ffd93c89cb8:4 in example.c:44
```


[dfsan]:https://clang.llvm.org/docs/DataFlowSanitizer.html
