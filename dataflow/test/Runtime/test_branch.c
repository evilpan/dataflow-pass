// RUN: clang -c -g -fsanitize=dataflow -std=c11 -Xclang -load -Xclang %buildir/dataflow/libLoadStorePass.so -I%runtimedir -c %s -o %t.o > %t.log 2>&1
// RUN: clang -fsanitize=dataflow %buildir/runtime.c.o %t.o -o %t.out >> %t.log 2>&1
// RUN: %t.out > %t1.log 2>&1
// RUN: FileCheck -input-file=%t1.log %s
#include <string.h>
#include "runtime.h"

int main()
{
    char src = 0x00;
    int condition = 1;
    df_init(&src, sizeof(src));

    if (condition) {
        // CHECK: test_branch.c:[[@LINE+1]]: tainted load 1 byte(s)
        src += 1;
    } else {
        // CHECK-NOT: test_branch.c:[[@LINE+1]]: tainted load 1 byte(s)
        src += 1;
    }

    df_stat();
    return 0;
}

