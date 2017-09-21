// RUN: clang -c -g -fsanitize=dataflow -std=c11 -Xclang -load -Xclang %buildir/dataflow/libLoadStorePass.so -I%runtimedir -c %s -o %t.o > %t.log 2>&1
// RUN: clang -fsanitize=dataflow %buildir/runtime.c.o %t.o -o %t.out >> %t.log 2>&1
// RUN: %t.out > %t1.log 2>&1
// RUN: FileCheck -input-file=%t1.log %s
#include "runtime.h"

#define MAXSIZE 256
int main()
{
    int buf[MAXSIZE] = {1};
    df_init(buf, sizeof(int));

    for (int i=0; i<MAXSIZE-1; i++) {
        buf[i+1] = buf[i] ^ 0x26 ;
    }

    // CHECK: test_pass.c:[[@LINE+1]]: tainted store 4 byte(s)
    int _temp = buf[MAXSIZE-1];

    df_stat();
    return 0;
}

