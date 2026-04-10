
#include "quixmath.h"
#include "args.h"
#include "parse.h"

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

	if (!flags.equation) {
		fprintf(stderr, "%s: no equation provided\n", NAME);
		return 1;
	}

	Node *tree = parse_equation(flags.equation);

	if (!tree) {
		fprintf(stderr, "%s: malformed equation: %s\n", NAME, flags.equation);
		return 1;
	}

	f64 result = eval(tree);
	free_node(tree);

	print_result(result);

	return 0;
}
