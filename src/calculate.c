#include "quixmath.h"

void eval_equation(mpfr_t result, const Node *node) {
    if (!node) {
        mpfr_set_d(result, 0.0, MPFR_RNDN);
        return;
    }

    if (node->kind == NODE_NUM) {
        mpfr_set(result, node->value, MPFR_RNDN);
        return;
    }

    mpfr_t left, right;
    mpfr_init2(left, QM_PRECISION);
    mpfr_init2(right, QM_PRECISION);

    eval_equation(left, node->left);
    eval_equation(right, node->right);

    switch (node->op) {
    case OP_ADD:
        mpfr_add(result, left, right, MPFR_RNDN);
        break;
    case OP_SUB:
        mpfr_sub(result, left, right, MPFR_RNDN);
        break;
    case OP_MUL:
        mpfr_mul(result, left, right, MPFR_RNDN);
        break;
    case OP_DIV:
        if (mpfr_zero_p(right)) {
            err_set(ERR_PARSE, "division by zero");
            err_print();
            mpfr_set_d(result, 0.0, MPFR_RNDN);
        } else {
            mpfr_div(result, left, right, MPFR_RNDN);
        }
        break;
    case OP_EXP:
        mpfr_pow(result, left, right, MPFR_RNDN);
        break;
    }

    mpfr_clear(left);
    mpfr_clear(right);
}
