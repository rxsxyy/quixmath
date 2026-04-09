
#include "quixmath.h"

#include "math.h"
#include "stdint.h"
#include "stdio.h"

i32 base_to_int(Base base) {
	static const i32 TABLE[] = {2, 10, 16};
	return TABLE[base];
}

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

/* Prints v in base 2, no leading zeroes (minimum one digit). */
static void print_binary(i64 v) {
	if (v < 0) {
		putchar('-');
		v = -v;
	}

	/* Find the highest set bit. */
	i32 top = 0;
	for (i32 b = 62; b >= 0; b--) {
		if ((v >> b) & 1) {
			top = b;
			break;
		}
	}

	for (i32 b = top; b >= 0; b--)
		putchar(((v >> b) & 1) ? '1' : '0');

	putchar('\n');
}

void print_result(f64 result, Base base) {
	switch (base) {
	case B_BINARY:
		print_binary((i64)result);
		break;
	case B_HEXADECIMAL:
		printf("%llX\n", (long long)(i64)result);
		break;
	default:
		/* Strip unnecessary trailing zeroes for whole numbers. */
		if (result == (i64)result) {
			printf("%lld\n", (long long)(i64)result);
		} else {
			printf("%g\n", result);
		}
		break;
	}
}

void print_help(void) {
	printf("usage: %s [options] \"<equation>\"\n", NAME);
}

void print_ver(void) {
	printf("%s v%d\n", NAME, VERSION);
}
