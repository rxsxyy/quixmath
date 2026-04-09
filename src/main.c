
#include "quixmath.h"
#include "args.h"

i32 main(i32 argc, char **argv) {
	Flags flags = parse_flags(argc, argv);

	if (flags.show_help || argc < 2) {
		print_help();
		return 0;
	}

	if (flags.show_ver) {
		print_ver();
		return 0;
	}

	return 0;
}
