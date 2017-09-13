#ifndef SANITIZER_RUNTIME_H
#define SANITIZER_RUNTIME_H
#include <sys/types.h>
// init tainted source
void df_init(void *ptr, size_t size);
// get load/store statistics
void df_stat();
#endif
