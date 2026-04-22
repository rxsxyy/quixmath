
#ifndef PARSE_H
#define PARSE_H

#include "!TYPES.h"
#include "mpfr.h"

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
        mpfr_t value;

        struct {
            OperationKind op;
            struct Node *left;
            struct Node *right;
        };
    };
} Node;

// Entry point for parsing.
Node *parse_eq(const char *eq);

// Frees node at the end of node calculation.
void free_node(Node *node);

#endif
