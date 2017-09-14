#include "runtime.h"
#include <stdio.h>
#include <stdatomic.h>
#include <sanitizer/dfsan_interface.h>
#define TRUE 1
#define FALSE 0
dfsan_label src_label;
static atomic_int initialized = FALSE;
static atomic_int total_load, tainted_load, clean_load;
static atomic_int total_store, tainted_store, clean_store;

#define DF_LOG(format, args...) fprintf(stderr, "DF_RUNTIME: " format"\n", ##args)
#define INCREASE(number) atomic_fetch_add_explicit(&number, 1, memory_order_relaxed)
void df_init(void *ptr, size_t size)
{
    src_label = dfsan_create_label("src", 0);
    dfsan_set_label(src_label, ptr, size);
    initialized = TRUE;
    DF_LOG("label initialized");
    total_load = tainted_load = clean_load = 0;
    total_store = tainted_store = clean_store = 0;
}
void df_stat()
{
    DF_LOG("total %d load, %d tainted, %d clean",
            total_load, tainted_load, clean_load);
    DF_LOG("total %d store, %d tainted, %d clean",
            total_store, tainted_store, clean_store);
}
void __loadcheck(unsigned char *ptr, size_t size,
        const char* file, size_t line) {
    dfsan_label temp = dfsan_read_label(ptr, size);
    int tainted = initialized ? dfsan_has_label(temp, src_label) : 0;
    if (tainted) {
        INCREASE(tainted_load);
    } else {
        INCREASE(clean_load);
    }
    INCREASE(total_load);
    DF_LOG("%s:%zu: %s load %zu byte(s)",
            file, line,
            tainted ? "tainted" : "clean",
            size);
    return;
}
void __storecheck(unsigned char *ptr, size_t size,
        const char *file, size_t line) {
    dfsan_label temp = dfsan_read_label(ptr, size);
    int tainted = initialized ? dfsan_has_label(temp, src_label) : 0;
    if (tainted) {
        INCREASE(tainted_store);
    } else {
        INCREASE(clean_store);
    }
    INCREASE(total_store);
    DF_LOG("%s:%zu: %s store %zu byte(s)",
            file, line,
            tainted ? "tainted" : "clean",
            size);
    return;
}
