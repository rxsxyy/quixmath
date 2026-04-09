
#include "args.h"
#include "stddef.h"
#include "stdio.h"
#include "string.h"

/* Table of available flags in current state. */
const Option FLAG_TABLE[] = {
        /* Base options, includes Binary, Decimal and heXadecimal. */
        {      "-fb",  O_INT, offsetof(Flags,      base), 0},
        {      "-fd",  O_INT, offsetof(Flags,      base), 1},
        {      "-fx",  O_INT, offsetof(Flags,      base), 2},
        /* PLANNED: Option for file input.  */
        {       "-f",  O_STR, offsetof(Flags,     empty), 0},
        /* Option for file output, will store the result of the equation.  */
        {       "-o",  O_STR, offsetof(Flags,  out_file), 0},
        /* PLANNED: Option for graphical view. Opens window showing 2d/3d graph */
        {       "-g", O_BOOL, offsetof(Flags,     empty), 0},
        /* Help options. */
        {       "-h", O_BOOL, offsetof(Flags, show_help), 1},
        {   "--help", O_BOOL, offsetof(Flags, show_help), 1},
        {       "-?", O_BOOL, offsetof(Flags, show_help), 1},
        /* Version options. */
        {       "-v", O_BOOL, offsetof(Flags,  show_ver), 1},
        {"--version", O_BOOL, offsetof(Flags,  show_ver), 1},
};

const usize FLAG_TABLE_LEN = sizeof(FLAG_TABLE) / sizeof(FLAG_TABLE[0]);

Flags parse_flags(i32 argc, char **argv) {
	Flags flags = {B_DECIMAL, NULL, NULL, false, false, 0};

	for (i32 i = 1; i < argc; i++) {
		Bool matched = false;

		for (usize j = 0; j < FLAG_TABLE_LEN; j++) {
			if (strcmp(argv[i], FLAG_TABLE[j].name) != 0) {
				continue;
			}

			char *base = (char *)&flags;

			switch (FLAG_TABLE[j].kind) {
			case O_BOOL:
				*(Bool *)(base + FLAG_TABLE[j].offset) = (Bool)FLAG_TABLE[j].value;
				break;
			case O_INT:
				*(i32 *)(base + FLAG_TABLE[j].offset) = FLAG_TABLE[j].value;
				break;
			case O_STR:
				if (i + 1 < argc) {
					*(char **)(base + FLAG_TABLE[j].offset) = argv[++i];
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
