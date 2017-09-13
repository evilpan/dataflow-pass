#include "runtime.h"
int get_sign(int x) {
    if (x == 0)
        return 0;

    if (x < 0)
        return -1;
    else
        return 1;
}

int main() {
    int a;
    df_init(&a, sizeof a);
    a = get_sign(a);
    df_stat();
    return a;
}
