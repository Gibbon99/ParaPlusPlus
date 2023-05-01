#pragma once

#include <cstdio>

/* https://github.com/Arryboom/fmemopen_windows  */
FILE *fmemopen(void *buf, size_t len, const char *type);
