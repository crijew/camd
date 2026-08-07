// Minimal SuiteSparse_config compatibility surface required by CAMD.
// SPDX-License-Identifier: BSD-3-Clause

#ifndef SUITESPARSE_CONFIG_H
#define SUITESPARSE_CONFIG_H

#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "SuiteSparse_config_version.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SuiteSparse_malloc camd_suitesparse_malloc
#define SuiteSparse_free camd_suitesparse_free
#define SuiteSparse_config_printf_func_get \
    camd_suitesparse_config_printf_func_get
#define SuiteSparse_config_printf_func_set \
    camd_suitesparse_config_printf_func_set

void *SuiteSparse_malloc(size_t nitems, size_t size_of_item);
void *SuiteSparse_free(void *p);

int (*SuiteSparse_config_printf_func_get(void))(const char *, ...);
void SuiteSparse_config_printf_func_set(int (*printf_func)(const char *, ...));

#define SUITESPARSE_PRINTF(params)                         \
    do {                                                   \
        int (*printf_func)(const char *, ...) =            \
            SuiteSparse_config_printf_func_get();          \
        if (printf_func != NULL) {                         \
            (void)(printf_func params);                    \
        }                                                  \
    } while (0)

#define SUITESPARSE_VER_CODE(main, sub) ((main) * 1000 + (sub))
#define SUITESPARSE_VERSION \
    SUITESPARSE_VER_CODE(SUITESPARSE_MAIN_VERSION, SUITESPARSE_SUB_VERSION)
#define SUITESPARSE__VERCODE(main, sub, patch) \
    (((main) * 1000ULL + (sub)) * 1000ULL + (patch))
#define SUITESPARSE__VERSION \
    SUITESPARSE__VERCODE(SUITESPARSE_MAIN_VERSION, \
        SUITESPARSE_SUB_VERSION, SUITESPARSE_SUBSUB_VERSION)

#ifdef __cplusplus
}
#endif

#endif
