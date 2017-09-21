// RUN: clang -c -g -fsanitize=dataflow -std=c11 -Xclang -load -Xclang %buildir/dataflow/libLoadStorePass.so -I%runtimedir -c %s -o %t.o > %t.log 2>&1
// RUN: clang -fsanitize=dataflow %buildir/runtime.c.o %t.o -o %t.out >> %t.log 2>&1
// RUN: %t.out > %t1.log 2>&1
// RUN: FileCheck -input-file=%t1.log %s
#include <assert.h>
#include "runtime.h"

int main()
{
    float src = 1;
    char dst;
    assert(sizeof(src) == 4);
    df_init(&src, sizeof(src));

    // CHECK: test_float.c:[[@LINE+2]]: tainted load 4 byte(s)
    // CHECK: test_float.c:[[@LINE+1]]: tainted store 1 byte(s)
    dst = (char)src;

    df_stat();
    return 0;
}

