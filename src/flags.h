
#ifndef FLAGS_H
#define FLAGS_H

#include "values.h"

typedef struct {
        i32 base;
        i32 input_base;
        char *out_file;
        i32 show_ver;
        i32 show_help;
} Flags;

typedef enum {
        OPT_INT,
        OPT_STR,
        OPT_BOOL,
} OptKind;

typedef struct {
        const char *name;
        OptKind kind;
        usize offset;
        i32 value;
} OptDesc;

/* The canonical flag table. Defined in flags.c, exposed here so that
 * callers (e.g. a --help printer) can iterate it without duplication.
 */
extern const OptDesc FLAG_TABLE[];
extern const usize FLAG_TABLE_LEN;

/* Scans argv for recognized flags and populates *flags.
 * On return, *eq_index holds the argv index of the equation string.
 * Returns 0 on success, -1 on error (unknown flag, missing argument, etc.).
 */
i32 parse_flags(i32 argc, char *argv[], Flags *flags, i32 *eq_index);

/* Writes the result to the file/stream described by flags.
 * Opens and closes the output file when flags->out_file is set.
 * Returns 0 on success, -1 on I/O error.
 */
i32 print_result(f64 result, const Flags *flags);

void print_help(const char *program);

void print_ver(i32 ver);

void free_flags(Flags *flags);

#endif
