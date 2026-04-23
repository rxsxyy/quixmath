#include "quixmath.h"

i32 main(i32 argc, char **argv) {
    vars_init();

    OptionFlag flags = parse_flags(argc, argv);

    if (flags.msg != MK_NONE) {
        run_flags(&flags);
        vars_free();
        return 0;
    }

    if (!flags.equation) {
        run_flags(&flags);
        vars_free();
        return 0;
    }
    
    char undefined_var[VARIABLE_MAX][VARIABLE_NAME_MAX];
    usize undefined_var_count = vars_collect(flags.equation, undefined_var, MAX_UNDEF);

    // Prompt for each undefined variable.
    for (usize i = 0; i < undefined_var_count; i++) {
        char buf[256];

        while (1) {
            printf("> ");
            fflush(stdout);

            if (prompt_read(buf, sizeof(buf)) != 0) {
                vars_free();
                return 0;
            }

            if (buf[0] == '\0') {
                continue;
            }

            char *eq_pos = strchr(buf, '=');
            if (!eq_pos || eq_pos == buf || *(eq_pos + 1) == '\0') {
                continue;
            }

            usize nlen = (usize)(eq_pos - buf);
            char name[VARIABLE_NAME_MAX];
            if (nlen >= VARIABLE_NAME_MAX) {
                continue;
            }
            memcpy(name, buf, nlen);
            name[nlen] = '\0';

            if (strcmp(name, undefined_var[i]) != 0) {
                continue;
            }

            if (!var_set(name, eq_pos + 1)) {
                continue;
            }

            break;
        }
    }

    Node *tree = parse_equation(flags.equation);
    if (!tree) {
        if (!err_has()) {
            err_set(ERR_PARSE, "invalid expression: %s", flags.equation);
        }
        err_print();
        vars_free();
        return 1;
    }

    mpfr_t result;
    mpfr_init2(result, PRECISION);

    eval_equation(result, tree);
    free_node(tree);

    if (err_has()) {
        mpfr_clear(result);
        vars_free();
        return 1;
    }

    if (mpfr_integer_p(result)) {
        mpfr_printf("%.0Rf\n\n", result);
    } else {
        mpfr_printf("%.25Rg\n\n", result);
    }

    mpfr_clear(result);
    vars_free();
    return 0;
}
