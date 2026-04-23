#include "quixmath.h"

static const char *ERR_LABELS[] = {
    [ERR_NONE] = "none",
    [ERR_PARSE] = "parse",
    [ERR_ALLOC] = "alloc",
    [ERR_FLAG] = "flag",
    [ERR_IO] = "io",
};

static struct {
    ErrorKind kind;
    char msg[ERROR_SIZE_MAX];
} Error = {ERR_NONE, {0}};

void err_set(ErrorKind kind, const char *fmt, ...) {
    Error.kind = kind;
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(Error.msg, ERROR_SIZE_MAX, fmt, ap);
    va_end(ap);
}

void err_print(void) {
    if (Error.kind == ERR_NONE) {
        return;
    }
    fprintf(stderr, "error[%s]: %s\n\n", ERR_LABELS[Error.kind], Error.msg);
}

bool err_has(void) {
    return Error.kind != ERR_NONE;
}

ErrorKind err_kind(void) {
    return Error.kind;
}

void err_clear(void) {
    Error.kind = ERR_NONE;
    Error.msg[0] = '\0';
}
