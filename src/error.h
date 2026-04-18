
#ifndef ERROR_H
#define ERROR_H

#include "!TYPES.h"

typedef enum {
	ERR_NONE = 0,
	ERR_PARSE,
	ERR_ALLOC,
	ERR_FLAG,
	ERR_IO,
} ErrorKind;

void err_set(ErrorKind kind, const char *fmt, ...);
void err_print(void);
bool err_has(void);
ErrorKind err_kind(void);
void err_clear(void);

#endif
