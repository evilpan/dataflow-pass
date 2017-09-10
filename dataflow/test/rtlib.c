#include <stdio.h>
#include <sanitizer/dfsan_interface.h>
#define TRUE 1
#define FALSE 0
dfsan_label src_label;
int initialized = FALSE;
void init_sanitizer(void *ptr, size_t size)
{
    src_label = dfsan_create_label("src", 0);
    dfsan_set_label(src_label, ptr, size);
    initialized = TRUE;
    printf("label initialized \n");
}
void __loadcheck(unsigned char *ptr, size_t size,
        const char* file, size_t line) {
    dfsan_label temp = dfsan_read_label(ptr, size);
    int tainted = initialized ? dfsan_has_label(temp, src_label) : 0;
    printf("[%d] runtime load check: %p:%zu in %s:%zu\n",
            tainted, ptr, size,
            file, line);
    return;
}
void __storecheck(unsigned char *ptr, size_t size,
        const char *file, size_t line) {
    dfsan_label temp = dfsan_read_label(ptr, size);
    int tainted = initialized ? dfsan_has_label(temp, src_label) : 0;
    printf("[%d] runtime store check: %p:%zu in %s:%zu\n",
            tainted, ptr, size,
            file, line);
    return;
}
