#ifndef libfmemopen_windows
#define libfmemopen_windows

#include <cstdio>

/* https://github.com/Arryboom/fmemopen_windows  */
FILE *fmemopen(void *buf, size_t len, const char *type);

#endif