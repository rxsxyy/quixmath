
#include "calculate.h"

#include "error.h"
#include "math.h"

f64 eval_eq(const Node *node) {
	if (!node) {
		return 0.0;
	}

	if (node->kind == NODE_NUM) {
		return node->value;
	}

	f64 lhs = eval_eq(node->lhs);
	f64 rhs = eval_eq(node->rhs);

	switch (node->op) {
	case OP_ADD:
		return lhs + rhs;
	case OP_SUB:
		return lhs - rhs;
	case OP_MUL:
		return lhs * rhs;
	case OP_DIV:
		if (rhs == 0.0) {
			err_set(ERR_PARSE, "division by zero");
			err_print();
			return 0.0;
		}
		return lhs / rhs;
	case OP_EXP:
		return pow(lhs, rhs);
	}

	return 0.0;
}
