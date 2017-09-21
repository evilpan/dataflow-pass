// RUN: clang -c -g -fsanitize=dataflow -std=c11 -Xclang -load -Xclang %buildir/dataflow/libLoadStorePass.so -I%runtimedir -c %s -o %t.o > %t.log 2>&1
// RUN: clang -fsanitize=dataflow %buildir/runtime.c.o %t.o -o %t.out >> %t.log 2>&1
// RUN: %t.out > %t1.log 2>&1
// RUN: FileCheck -input-file=%t1.log %s
// XFAIL: *
#include <assert.h>
#include "runtime.h"

int main()
{
    unsigned char src = 0x7f;
    unsigned char dst;
    df_init(&src, sizeof(src));

    src = src ^ src;
    assert(src == 0);
    // src should not be considered tainted but DFSan failed to cover this
    // CHECK-NOT: test_xor.c:[[@LINE+2]]: tainted load 1 byte(s)
    // CHECK-NOT: test_xor.c:[[@LINE+1]]: tainted store 1 byte(s)
    dst = src;

    df_stat();
    return 0;
}
