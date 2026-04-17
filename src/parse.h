
#ifndef PARSE_H
#define PARSE_H

#include "!TYPES.h"

/* Operator types used in NODE_OP nodes, corresponding to +, -, *, /, ^. */
typedef enum {
	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV,
	OP_EXP,
} OperationKind;

/* Discriminator for Node: either a numeric leaf (NODE_NUM) or an operator node
 * (NODE_OP) with left and right children.
 */
typedef enum {
	NODE_NUM,
	NODE_OP,
} NodeKind;

/* AST node. NODE_NUM nodes carry a double value. NODE_OP nodes carry an
 * operator and pointers to left and right child nodes.
 */
typedef struct Node {
	NodeKind kind;

	union {
		f64 value;

		struct {
			OperationKind op;
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

/* Frees node at the end of node calculation. Used at the end of main() to free
 * the equation in total. 
 */
void free_node(Node *node);

#endif
