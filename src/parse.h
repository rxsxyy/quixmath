
#ifndef PARSE_H
#define PARSE_H

#include "types.h"

#include "ctype.h"
#include "stdlib.h"

/* Operator types used in NODE_OP nodes, corresponding to +, -, *, /, ^. */
typedef enum {
	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV,
	OP_EXP,
} OP_KIND;

/* Discriminator for Node: either a numeric leaf (NODE_NUM) or an operator node
 * (NODE_OP) with left and right children.
 */
typedef enum {
	NODE_NUM,
	NODE_OP,
} NODE_KIND;

/* AST node. NODE_NUM nodes carry a double value. NODE_OP nodes carry an
 * operator and pointers to left and right child nodes.
 */
typedef struct Node {
	NODE_KIND kind;

	union {
		f64 value;

		struct {
			OP_KIND op;
			struct Node *lhs;
			struct Node *rhs;
		};
	};
} Node;

/* Parses a primary expression: either a parenthesized sub-expression or a
 * numeric literal. Returns NULL if neither is found at the current position.
 */
static Node *parse_primary(void);

/* Parses a unary expression. Handles leading '-' by constructing a
 * (0 - operand) subtraction node. Supports chained negation (e.g. --5). Falls
 * through to parse_primary if no unary operator is present.
 */
static Node *parse_unary(void);

/* Parses an exponentiation expression. '^' is right-associative, so the
 * exponent is parsed recursively (e.g. 2^3^4 evaluates as 2^(3^4)).
 */
static Node *parse_power(void);

/* Parses a multiplicative expression. Handles '*' and '/' left-associatively,
 * consuming as many factors as are present.
 */
static Node *parse_term(void);

/* Parses an additive expression. Handles '+' and '-' left-associatively,
 * consuming as many terms as are present. Entry point of the precedence chain.
 */
static Node *parse_expr(void);

/* Entry point for parsing. Sets the global ptr to eq and runs parse_expr.
 * Returns NULL if eq is NULL or if any characters remain after parsing,
 * indicating a malformed expression.
 */
Node *parse_equation(const char *eq);

/* Frees node at the end of node calculation. Used at the end of main() to free
 * the equation in total. 
 */
void free_node(Node *node);

#endif
