
#include "args.h"
#include "stddef.h"
#include "stdio.h"
#include "string.h"

/* Table of available flags in current state. */
const Option FLAG_TABLE[] = {
    {"-h",        O_BOOL, offsetof(Flags, show_help), 1},
    {"--help",    O_BOOL, offsetof(Flags, show_help), 1},
    {"-?",        O_BOOL, offsetof(Flags, show_help), 1},
    {"-v",        O_BOOL, offsetof(Flags, show_ver),  1},
    {"--version", O_BOOL, offsetof(Flags, show_ver),  1},
};

const usize FLAG_TABLE_LEN = sizeof(FLAG_TABLE) / sizeof(FLAG_TABLE[0]);

Flags parse_flags(i32 argc, char **argv) {
	Flags flags = {B_DECIMAL, NULL, false, false};

	for (i32 i = 1; i < argc; i++) {
		Bool matched = false;

		for (usize j = 0; j < FLAG_TABLE_LEN; j++) {
			if (strcmp(argv[i], FLAG_TABLE[j].name) != 0)
				continue;

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

		if (!matched)
			fprintf(stderr, "unknown flag: %s\n", argv[i]);
	}

	return flags;
}
