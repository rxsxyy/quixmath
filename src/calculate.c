
#include "calculate.h"

#include "!INFO.h"
#include "error.h"
#include "parse.h"

void eval_eq(mpfr_t result, const Node *node) {
    if (!node) {
        mpfr_set_d(result, 0.0, MPFR_RNDN);
        return;
    }

    if (node->kind == NODE_NUM) {
        mpfr_set(result, node->value, MPFR_RNDN);
        return;
    }

    mpfr_t lhs, rhs;
    mpfr_init2(lhs, QM_PRECISION);
    mpfr_init2(rhs, QM_PRECISION);

    eval_eq(lhs, node->lhs);
    eval_eq(rhs, node->rhs);

    switch (node->op) {
    case OP_ADD:
        mpfr_add(result, lhs, rhs, MPFR_RNDN);
        break;
    case OP_SUB:
        mpfr_sub(result, lhs, rhs, MPFR_RNDN);
        break;
    case OP_MUL:
        mpfr_mul(result, lhs, rhs, MPFR_RNDN);
        break;
    case OP_DIV:
        if (mpfr_zero_p(rhs)) {
            err_set(ERR_PARSE, "division by zero");
            err_print();
            mpfr_set_d(result, 0.0, MPFR_RNDN);
        } else {
            mpfr_div(result, lhs, rhs, MPFR_RNDN);
        }
        break;
    case OP_EXP:
        mpfr_pow(result, lhs, rhs, MPFR_RNDN);
        break;
    }

    mpfr_clear(lhs);
    mpfr_clear(rhs);
}
