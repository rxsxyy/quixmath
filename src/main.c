
#include "!TYPES.h"

#include "calculate.h"
#include "error.h"
#include "flags.h"
#include "parse.h"
#include "stdio.h"

i32 main(i32 argc, char **argv) {
	OptionFlag flags = parse_flags(argc, argv);

	if (flags.msg != MK_NONE) {
		run_flags(&flags);
		return 0;
	}

	if (!flags.equation) {
		run_flags(&flags);
		return 0;
	}

	Node *tree = parse_eq(flags.equation);
	if (!tree) {
		err_set(ERR_PARSE, "invalid expression: %s", flags.equation);
		err_print();
		return 1;
	}

	f64 result = eval_eq(tree);
	free_node(tree);

	if (err_has()) {
		return 1;
	}

	printf("%g\n\n", result);
	return 0;
}
