
#include "quixmath.h"

/* Recursively evaluates an AST produced by parse_eq. */
f64 eval(const Node *node) {
	if (!node)
		return 0.0;

	if (node->kind == NODE_NUM)
		return node->value;

	f64 l = eval(node->lhs);
	f64 r = eval(node->rhs);

	switch (node->op) {
	case OP_ADD:
		return l + r;
	case OP_SUB:
		return l - r;
	case OP_MUL:
		return l * r;
	case OP_DIV:
		return l / r;
	case OP_EXP:
		return pow(l, r);
	}

	return 0.0;
}

/* Prints the result, stripping unnecessary trailing zeroes for whole numbers.
 */
void print_result(f64 result) {
	/* Strip unnecessary trailing zeroes for whole numbers. */
	if (result == (i64)result) {
		printf("%lld\n", (long long)(i64)result);
	} else {
		printf("%g\n", result);
	}
}

/* Prints program help. */
void print_help(void) {
	printf("usage: %s [options] \"<equation>\"\n", NAME);
}

/* Prints program version. */
void print_ver(void) {
	printf("%s v%d\n", NAME, VERSION);
}
