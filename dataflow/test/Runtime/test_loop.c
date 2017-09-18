// RUN: clang -c -g -fsanitize=dataflow -std=c11 -Xclang -load -Xclang %buildir/dataflow/libLoadStorePass.so -I%runtimedir -c %s -o %t.o > %t.log 2>&1
// RUN: clang -fsanitize=dataflow %buildir/runtime.c.o %t.o -o %t.out >> %t.log 2>&1
// RUN: %t.out > %t1.log 2>&1
// RUN: FileCheck -input-file=%t1.log %s
#include <string.h>
#include "runtime.h"

#define MAXSIZE 16
int main()
{
    char src[MAXSIZE] = {1};
    char dst[MAXSIZE] = {0};
    df_init(src, MAXSIZE/2);

    for (int i=0; i<MAXSIZE; i++) {
        // CHECK: test_loop.c:[[@LINE+4]]: tainted load 1 byte(s)
        // CHECK: test_loop.c:[[@LINE+3]]: tainted store 1 byte(s)
        // CHECK: test_loop.c:[[@LINE+2]]: clean load 1 byte(s)
        // CHECK: test_loop.c:[[@LINE+1]]: clean store 1 byte(s)
        dst[i] = src[i];
    }

    df_stat();
    return 0;
}
