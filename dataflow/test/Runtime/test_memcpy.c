// RUN: clang -c -g -fsanitize=dataflow -std=c11 -Xclang -load -Xclang %buildir/dataflow/libLoadStorePass.so -I%runtimedir -c %s -o %t.o > %t.log 2>&1
// RUN: clang -fsanitize=dataflow %buildir/runtime.c.o %t.o -o %t.out >> %t.log 2>&1
// RUN: %t.out > %t1.log 2>&1
// RUN: FileCheck -input-file=%t1.log %s
#include <string.h>
#include "runtime.h"

#define MAXSIZE 256
int main()
{
    char src[MAXSIZE] = {1};
    char dst[MAXSIZE] = {0};
    df_init(src, MAXSIZE);

    // CHECK: test_memcpy.c:[[@LINE+1]]: tainted load 128 byte(s)
    memcpy(dst, src, MAXSIZE/2);

    char temp;
    // CHECK: test_memcpy.c:[[@LINE+1]]: tainted load 1 byte(s)
    temp = dst[0];
    // CHECK: test_memcpy.c:[[@LINE+1]]: tainted load 1 byte(s)
    temp = dst[MAXSIZE/2 - 1];
    // CHECK: test_memcpy.c:[[@LINE+1]]: clean load 1 byte(s)
    temp = dst[MAXSIZE/2];

    df_stat();
    return 0;
}
