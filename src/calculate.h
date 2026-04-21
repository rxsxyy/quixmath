
#ifndef CALCULATE_H
#define CALCULATE_H

#include "!TYPES.h"

#include "parse.h"

// Evaluates the AST rooted at node, writing the result into the
// caller-initialized mpfr_t `result`. Division by zero is reported
// via err_set() and writes 0 into result.
void eval_eq(mpfr_t result, const Node *node);

#endif
