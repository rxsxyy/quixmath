
#include "parse.h"

#include "!INFO.h"
#include "ctype.h"
#include "stdlib.h"

static const char *ptr;

static void skip_spaces(void) {
    while (isspace((u8)*ptr)) {
        ptr++;
    }
}

static Node *make_op(OperationKind op, Node *left, Node *right) {
    Node *n = malloc(sizeof(Node));
    if (!n) {
        return NULL;
    }
    n->kind = NODE_OP;
    n->op = op;
    n->lhs = left;
    n->rhs = right;
    return n;
}

// Forward declaration needed because parse_primary calls parse_expr upward.
static Node *parse_expr(void);

static Node *parse_primary(void) {
    skip_spaces();

    if (*ptr == '(') {
        ptr++;
        Node *inner = parse_expr();
        skip_spaces();
        if (*ptr == ')') {
            ptr++;
        }
        return inner;
    }

    Node *n = malloc(sizeof(Node));
    if (!n) {
        return NULL;
    }
    n->kind = NODE_NUM;
    mpfr_init2(n->value, QM_PRECISION);

    char *end;
    mpfr_strtofr(n->value, ptr, &end, 10, MPFR_RNDN);

    if (end == ptr) {
        mpfr_clear(n->value);
        free(n);
        return NULL;
    }

    ptr = end;
    return n;
}

static Node *parse_unary(void) {
    skip_spaces();
    if (*ptr == '-') {
        ptr++;
        Node *operand = parse_unary();
        if (!operand) {
            return NULL;
        }
        Node *zero = malloc(sizeof(Node));
        if (!zero) {
            free_node(operand);
            return NULL;
        }
        zero->kind = NODE_NUM;
        mpfr_init2(zero->value, QM_PRECISION);
        mpfr_set_d(zero->value, 0.0, MPFR_RNDN);
        return make_op(OP_SUB, zero, operand);
    }
    return parse_primary();
}

static Node *parse_power(void) {
    Node *base = parse_unary();
    if (!base) {
        return NULL;
    }
    skip_spaces();
    if (*ptr == '^') {
        ptr++;
        Node *exp = parse_power();
        if (!exp) {
            free_node(base);
            return NULL;
        }
        return make_op(OP_EXP, base, exp);
    }
    return base;
}

static Node *parse_term(void) {
    Node *left = parse_power();
    if (!left) {
        return NULL;
    }
    skip_spaces();
    while (*ptr == '*' || *ptr == '/') {
        OperationKind op = (*ptr == '*') ? OP_MUL : OP_DIV;
        ptr++;
        Node *right = parse_power();
        if (!right) {
            free_node(left);
            return NULL;
        }
        left = make_op(op, left, right);
        skip_spaces();
    }
    return left;
}

static Node *parse_expr(void) {
    Node *left = parse_term();
    if (!left) {
        return NULL;
    }
    skip_spaces();
    while (*ptr == '+' || *ptr == '-') {
        OperationKind op = (*ptr == '+') ? OP_ADD : OP_SUB;
        ptr++;
        Node *right = parse_term();
        if (!right) {
            free_node(left);
            return NULL;
        }
        left = make_op(op, left, right);
        skip_spaces();
    }
    return left;
}

Node *parse_eq(const char *eq) {
    if (!eq) {
        return NULL;
    }
    ptr = eq;
    Node *root = parse_expr();
    skip_spaces();
    if (*ptr != '\0') {
        free_node(root);
        return NULL;
    }
    return root;
}

void free_node(Node *node) {
    if (!node) {
        return;
    }
    if (node->kind == NODE_OP) {
        free_node(node->lhs);
        free_node(node->rhs);
    } else {
        mpfr_clear(node->value);
    }
    free(node);
}
