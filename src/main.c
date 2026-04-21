
#include "!INFO.h"
#include "!TYPES.h"

#include "calculate.h"
#include "error.h"
#include "flags.h"
#include "parse.h"

i32 main(i32 argc, char **argv) {
    OptionFlag flags = parse_flags(argc, argv);

    if (flags.msg != MK_NONE) {
        run_flags(&flags);
        return 0;
    }

    if (!flags.equation) {
        run_flags(&flags);
        return 0;
    }

    Node *tree = parse_eq(flags.equation);
    if (!tree) {
        err_set(ERR_PARSE, "invalid expression: %s", flags.equation);
        err_print();
        return 1;
    }

    mpfr_t result;
    mpfr_init2(result, QM_PRECISION);

    eval_eq(result, tree);
    free_node(tree);

    if (err_has()) {
        mpfr_clear(result);
        return 1;
    }

    if (mpfr_integer_p(result)) {
        mpfr_printf("%.0Rf\n", result);
    } else {
        mpfr_printf("%.25Rg\n", result);
    }
    mpfr_clear(result);
    return 0;
}
