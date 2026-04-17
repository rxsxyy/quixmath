
#include "error.h"

#include "stdarg.h"
#include "stdio.h"

#define ERR_MSG_MAX 256

static const char *ERR_LABELS[] = {
    [ERR_NONE] = "none",
    [ERR_PARSE] = "parse",
    [ERR_ALLOC] = "alloc",
    [ERR_FLAG] = "flag",
    [ERR_IO] = "io",
};

static struct {
    ErrorKind kind;
    char msg[ERR_MSG_MAX];
} g_err = {ERR_NONE, {0}};

void err_set(ErrorKind kind, const char *fmt, ...) {
    g_err.kind = kind;
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(g_err.msg, ERR_MSG_MAX, fmt, ap);
    va_end(ap);
}

void err_print(void) {
    if (g_err.kind == ERR_NONE) {
        return;
    }
    fprintf(stderr, "error[%s]: %s\n\n", ERR_LABELS[g_err.kind], g_err.msg);
}

bool err_has(void) {
    return g_err.kind != ERR_NONE;
}

ErrorKind err_kind(void) {
    return g_err.kind;
}

void err_clear(void) {
    g_err.kind = ERR_NONE;
    g_err.msg[0] = '\0';
}
