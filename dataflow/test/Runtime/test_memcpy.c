// RUN: clang -c -g -fsanitize=dataflow -std=c11 -Xclang -load -Xclang %buildir/dataflow/libLoadStorePass.so -I%runtimedir -c %s -o %t.o > %t.log 2>&1
// RUN: clang -c -g -fsanitize=dataflow -std=c11 %runtimedir/runtime.c -I%runtimedir -o %t.runtime.o >> %t.log 2>&1
// RUN: clang -fsanitize=dataflow %t.runtime.o %t.o -o %t.out >> %t.log 2>&1
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
    temp = dst[0];
    temp = dst[MAXSIZE/2 - 1];
    temp = dst[MAXSIZE - 1];

    df_stat();
    return 0;
}
