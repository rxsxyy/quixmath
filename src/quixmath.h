#include "!INFO.h"
#include "!TYPES.h"
#include "ctype.h"
#include "error.h"
#include "mpfr.h" // Dependency
#include "stdarg.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "termios.h"
#include "unistd.h"

#define VARIABLE_MAX 64
#define VARIABLE_NAME_MAX 64

#define PRECISION 128

typedef enum {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_EXP,
} OperationKind;

typedef enum {
    NODE_NUM,
    NODE_OP,
} NodeKind;

typedef struct Node {
    NodeKind kind;

    union {
        mpfr_t value;

        struct {
            OperationKind op;
            struct Node *left;
            struct Node *right;
        };
    };
} Node;

typedef enum {
    MK_NONE,
    MK_VER,
    MK_HELP,
} MessageKind;

typedef struct {
    char *out_file;
    char *in_file;
    char *equation;
    MessageKind msg;
    void *TODO; // left here for planned future flags
} OptionFlag;

// option kind enumerator.
typedef enum {
    O_NONE,
    O_INT,
    O_STR,
    O_BOOL, // also left here for any future plans
} OptionKind;

typedef struct {
    const char *name;
    const char *desc;
    OptionKind kind;
    usize offset;
    i32 value;
} Option;

extern const Option OPTION_TABLE[];
extern const usize OPTION_TABLE_LEN;

typedef enum {
    ERR_NONE,
    ERR_PARSE,
    ERR_ALLOC,
    ERR_FLAG,
    ERR_IO,
} ErrorKind;
