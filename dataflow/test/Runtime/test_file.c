// RUN: clang -c -g -fsanitize=dataflow -std=c11 -Xclang -load -Xclang %buildir/dataflow/libLoadStorePass.so -I%runtimedir -c %s -o %t.o > %t.log 2>&1
// RUN: clang -fsanitize=dataflow %buildir/runtime.c.o %t.o -o %t.out >> %t.log 2>&1
// RUN: %t.out /etc/passwd > %t1.log 2>&1
// RUN: FileCheck -input-file=%t1.log -check-prefix=CHECK-FOO %s
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <strings.h>
#define MAXSIZE 2048
#include "runtime.h"
void foo(char *buf, int size) {
    // CHECK-FOO: test_file.c:[[@LINE+1]]: tainted load 1 byte(s)
    char a = buf[0];
    // CHECK-FOO: test_file.c:[[@LINE+1]]: tainted load 1 byte(s)
    char b = buf[9];
    // CHECK-FOO: test_file.c:[[@LINE+1]]: tainted store 1 byte(s)
    buf[size] = a+b;
    // CHECK-FOO: test_file.c:[[@LINE+1]]: clean store 1 byte(s)
    buf[size] = 1;
}
void bar(char *buf, int size) {
    char a = buf[11];
    char b = buf[12];
    buf[13] = a+b;
}
int main(int argc, char **argv)
{
    if(argc != 2) {
        printf("Usage: %s input-file\n", argv[0]);
        return 1;
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        printf("failed to open %s\n", argv[1]);
        return 1;
    }
    char buf[MAXSIZE+1];
    bzero(buf, MAXSIZE+1);
    int rdsize = read(fd, buf, MAXSIZE);
    printf("read %d from file\n", rdsize);
    df_init(buf, rdsize);

    if (rdsize > 10)
        foo(buf, rdsize);
    else
        bar(buf, rdsize);
    df_stat();
    return 0;
}
