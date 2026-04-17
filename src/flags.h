
#ifndef FLAGS_H
#define FLAGS_H

#include "!TYPES.h"

// message kind enumerator, check `run_flags()`
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

OptionFlag parse_flags(i32 argc, char **argv);
void run_flags(const OptionFlag *flags);

#endif
