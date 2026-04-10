
#ifndef QM_H
#define QM_H

#include "args.h"
#include "parse.h"

#define VERSION 1
#define NAME "quixmath"

/* Recursively evaluates an AST produced by parse_eq. */
f64 eval(const Node *node);

/* Prints the result, stripping unnecessary trailing zeroes for whole numbers. */
void print_result(f64 result);

/* Prints program help. */
void print_help(void);

/* Prints program version. */
void print_ver(void);

#endif
