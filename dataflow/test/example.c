#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <strings.h>
#define MAXSIZE 2048
extern void init_sanitizer(void *ptr, size_t size);

void foo(char *buf, int size) {
    char a = buf[0];
    char b = buf[9];
    buf[size] = a+b;
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
    init_sanitizer(buf, rdsize);

    if (rdsize > 10)
        foo(buf, rdsize);
    else
        bar(buf, rdsize);
    return 0;
}
