#include <string.h>
#include "runtime.h"

#define MAXSIZE 256
int main()
{
    char src[MAXSIZE] = {1};
    char dst[MAXSIZE] = {0};
    df_init(src, MAXSIZE);

    memcpy(dst, src, MAXSIZE/2);
    char temp;
    temp = dst[0];
    temp = dst[MAXSIZE/2 - 1];
    temp = dst[MAXSIZE/2];

    df_stat();
    return 0;
}
