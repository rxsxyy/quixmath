
#ifndef VARS_H
#define VARS_H

#include "!TYPES.h"
#include "mpfr.h"

#define VAR_CAP 64
#define VAR_NAME_MAX 32

void vars_init(void);
void vars_free(void);
bool var_set(const char *name, const char *val_str);
bool var_get(const char *name, mpfr_t out);
bool var_exists(const char *name);
void vars_scan(const char *expr, void (*cb)(const char *name));

#endif
