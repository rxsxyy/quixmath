
#include "args.h"

/* Table of available flags in current state. */
const Option OPTION_TABLE[] = {
        /* PLANNED: Option for file input.  */
        {       "-f",  O_STR, offsetof(Flags,     empty), 0},
        /* Option for file output, will store the result of the equation.  */
        {       "-o",  O_STR, offsetof(Flags,  out_file), 0},
        /* PLANNED: Options for graphical view. Opens window showing 2d/3d graph */
        {       "-g", O_BOOL, offsetof(Flags,     empty), 0},
        {  "--graph", O_BOOL, offsetof(Flags,     empty), 0},
        /* Help options. */
        {       "-h", O_BOOL, offsetof(Flags, show_help), 1},
        {   "--help", O_BOOL, offsetof(Flags, show_help), 1},
        {       "-?", O_BOOL, offsetof(Flags, show_help), 1},
        /* Version options. */
        {       "-v", O_BOOL, offsetof(Flags,  show_ver), 1},
        {"--version", O_BOOL, offsetof(Flags,  show_ver), 1},
};

/* Length of table of available flags */
const usize OPTION_TABLE_LEN = sizeof(OPTION_TABLE) / sizeof(OPTION_TABLE[0]);

/* Parses flags sent as arguments. */
Flags parse_flags(i32 argc, char **argv) {
	Flags flags = {NULL, NULL, false, false, 0};

	for (i32 i = 1; i < argc; i++) {
		Bool matched = false;

		for (usize j = 0; j < OPTION_TABLE_LEN; j++) {
			if (strcmp(argv[i], OPTION_TABLE[j].name) != 0) {
				continue;
			}

			char *base = (char *)&flags;

			switch (OPTION_TABLE[j].kind) {
			case O_BOOL:
				*(Bool *)(base + OPTION_TABLE[j].offset) = (Bool)OPTION_TABLE[j].value;
				break;
			case O_INT:
				*(i32 *)(base + OPTION_TABLE[j].offset) = OPTION_TABLE[j].value;
				break;
			case O_STR:
				if (i + 1 < argc) {
					*(char **)(base + OPTION_TABLE[j].offset) = argv[++i];
				}
				break;
			}

			matched = true;
			break;
		}

		if (!matched) {
			/* First non-flag argument is treated as the equation. */
			if (!flags.equation) {
				flags.equation = argv[i];
			} else {
				fprintf(stderr, "unknown flag: %s\n", argv[i]);
			}
		}
	}

	return flags;
}
