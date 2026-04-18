
#ifndef CALCULATE_H
#define CALCULATE_H

#include "!TYPES.h"

#include "parse.h"

// Recursively evaluates the AST rooted at node and returns the result.
// Division by zero is reported via err_set() and returns 0.0.
f64 eval_eq(const Node *node);

#endif
