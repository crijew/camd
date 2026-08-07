// Minimal SuiteSparse_config runtime required by CAMD.
// SPDX-License-Identifier: BSD-3-Clause

#include "SuiteSparse_config.h"

static int (*camd_printf_func)(const char *, ...) = printf;

void *SuiteSparse_malloc(size_t nitems, size_t size_of_item)
{
    if (nitems < 1) nitems = 1;
    if (size_of_item < 1) size_of_item = 1;
    if (nitems > SIZE_MAX / size_of_item) return NULL;
    return malloc(nitems * size_of_item);
}

void *SuiteSparse_free(void *p)
{
    free(p);
    return NULL;
}

int (*SuiteSparse_config_printf_func_get(void))(const char *, ...)
{
    return camd_printf_func;
}

void SuiteSparse_config_printf_func_set(int (*printf_func)(const char *, ...))
{
    camd_printf_func = printf_func;
}
