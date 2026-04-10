
#ifndef ARGS_H
#define ARGS_H

#include "types.h"

#include "stddef.h"
#include "stdio.h"
#include "string.h"

/* Flag struct for different types of option flags. */
typedef struct {
	char *out_file;
	char *equation;
	Bool show_help;
	Bool show_ver;
	void *empty;
} Flags;

/* Option kind. */
typedef enum {
	O_INT,
	O_STR,
	O_BOOL,
} OptionKind;

/* Option struct, used for OPTION_TABLE[] */
typedef struct {
	const char *name;
	OptionKind kind;
	usize offset;
	i32 value;
} Option;

/* Table of available flags in current state. */
extern const Option OPTION_TABLE[];
/* Length of table of available options. */
extern const usize OPTION_TABLE_LEN;

/* Parses flags sent as arguments. */
Flags parse_flags(i32 argc, char **argv);

#endif
