
#ifndef ARGS_H
#define ARGS_H

#include "types.h"

typedef struct {
	char *out_file;
	char *equation;
	Bool show_help;
	Bool show_ver;
	void *empty;
} Flags;

typedef enum {
	O_INT,
	O_STR,
	O_BOOL,
} OptionKind;

typedef struct {
	const char *name;
	OptionKind kind;
	usize offset;
	i32 value;
} Option;

extern const Option FLAG_TABLE[];
extern const usize FLAG_TABLE_LEN;

/* Parses flags sent as arguments. */
Flags parse_flags(i32 argc, char **argv);

#endif
