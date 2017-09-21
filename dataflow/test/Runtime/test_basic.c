// RUN: clang -c -g -fsanitize=dataflow -std=c11 -Xclang -load -Xclang %buildir/dataflow/libLoadStorePass.so -I%runtimedir -c %s -o %t.o > %t.log 2>&1
// RUN: clang -fsanitize=dataflow %buildir/runtime.c.o %t.o -o %t.out >> %t.log 2>&1
// RUN: %t.out > %t1.log 2>&1
// RUN: FileCheck -input-file=%t1.log %s
#include "runtime.h"

#define MAXSIZE 256
int main()
{
    int i, j, k;
    df_init(&i, sizeof(int));

    // CHECK: test_basic.c:[[@LINE+1]]: tainted store 4 byte(s)
    j = i;
    // CHECK: test_basic.c:[[@LINE+1]]: clean store 4 byte(s)
    i = 0;
    // CHECK: test_basic.c:[[@LINE+1]]: clean store 4 byte(s)
    k = i;

    df_stat();
    return 0;
}


