
#ifndef PARSE_H
#define PARSE_H

#include "types.h"

/* Operator types used in NODE_OP nodes, corresponding to +, -, *, /, ^.
 */
typedef enum {
	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV,
	OP_EXP,
} OP_KIND;

/* Discriminator for Node: either a numeric leaf (NODE_NUM) or an operator
 * node (NODE_OP) with left and right children.
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

/* Entry point for parsing. Sets the global ptr to eq and runs parse_expr.
 * Returns NULL if eq is NULL or if any characters remain after parsing,
 * indicating a malformed expression.
 */
Node *parse_eq(const char *eq);

void free_node(Node *node);

#endif
