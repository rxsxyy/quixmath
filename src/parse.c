
#include "parse.h"

#include <ctype.h>
#include <stdlib.h>

static const char *ptr;

static void skip_spaces(void) {
        while (isspace((u8)*ptr)) {
                ptr++;
        }
}

/* Allocates and returns a NODE_NUM node with the given value.
 * Returns NULL on allocation failure.
 */
static Node *make_num(f64 v) {
        Node *n = malloc(sizeof(Node));
        if (!n) {
                return NULL;
        }
        n->kind = NODE_NUM;
        n->value = v;
        return n;
}

/* Allocates and returns a NODE_OP node with the given operator and children.
 * Returns NULL on allocation failure.
 */
static Node *make_op(OP_KIND op, Node *left, Node *right) {
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

static Node *parse_primary(void) {
        skip_spaces();

        if (*ptr == '(') {
                ptr++;
                Node *inner = parse_expr();
                skip_spaces();
                if (*ptr == ')')
                        ptr++;
                return inner;
        }

        char *end;
        f64 v = strtod(ptr, &end);
        if (end == ptr)
                return NULL;
        ptr = end;
        return make_num(v);
}

static Node *parse_unary(void) {
        skip_spaces();
        if (*ptr == '-') {
                ptr++;
                Node *operand = parse_unary();
                if (!operand)
                        return NULL;
                Node *zero = make_num(0.0);
                if (!zero) {
                        free_node(operand);
                        return NULL;
                }
                return make_op(OP_SUB, zero, operand);
        }
        return parse_primary();
}

static Node *parse_power(void) {
        Node *base = parse_unary();
        if (!base)
                return NULL;
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
        if (!left)
                return NULL;
        skip_spaces();
        while (*ptr == '*' || *ptr == '/') {
                OP_KIND op = (*ptr == '*') ? OP_MUL : OP_DIV;
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
        if (!left)
                return NULL;
        skip_spaces();
        while (*ptr == '+' || *ptr == '-') {
                OP_KIND op = (*ptr == '+') ? OP_ADD : OP_SUB;
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
        }
        free(node);
}
