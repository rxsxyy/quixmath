
#ifndef PARSE_H
#define PARSE_H

#include "!TYPES.h"

// Operator types used in NODE_OP nodes, corresponding to +, -, *, /, ^.
typedef enum {
	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV,
	OP_EXP,
} OperationKind;

typedef enum {
	NODE_NUM,
	NODE_OP,
} NodeKind;

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

// Entry point for parsing.
Node *parse_eq(const char *eq);

// Frees node at the end of node calculation.
void free_node(Node *node);

#endif
