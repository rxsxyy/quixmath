
#ifndef QM_H
#define QM_H

#include "args.h"
#include "parse.h"

#define VERSION 1
#define NAME "quixmath"

/* Maps the Base enum to the integer base used by parse_eq. */
i32 base_to_int(Base base);

/* Recursively evaluates an AST produced by parse_eq. */
f64 eval(const Node *node);

/* Prints the result in the format matching the given base:
 *   B_BINARY      — integer representation in base 2 (e.g. 1010)
 *   B_DECIMAL     — floating-point representation    (e.g. 42.5)
 *   B_HEXADECIMAL — integer representation in base 16 (e.g. 2a)
 */
void print_result(f64 result, Base base);

/* Prints program help. */
void print_help(void);

/* Prints program version. */
void print_ver(void);

#endif
