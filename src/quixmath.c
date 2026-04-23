#include "!INFO.h"
#include "!TYPES.h"

#include "calculate.h"
#include "error.h"
#include "flags.h"
#include "parse.h"
#include "prompt.h"
#include "stdio.h"
#include "string.h"
#include "vars.h"

#define MAX_UNDEF VAR_CAP

// Collected during vars_scan; populated by collect_undef callback.
static char s_undef[MAX_UNDEF][VAR_NAME_MAX];
static usize s_undef_count = 0;

static void collect_undef(const char *name) {
    if (var_exists(name)) {
        return;
    }
    for (usize i = 0; i < s_undef_count; i++) {
        if (strcmp(s_undef[i], name) == 0) {
            return;
        }
    }
    if (s_undef_count < MAX_UNDEF) {
        strncpy(s_undef[s_undef_count], name, VAR_NAME_MAX - 1);
        s_undef[s_undef_count][VAR_NAME_MAX - 1] = '\0';
        s_undef_count++;
    }
}

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

    // Find variables in the expression that have no inline assignment.
    vars_scan(flags.equation, collect_undef);

    // Prompt for each undefined variable.
    for (usize i = 0; i < s_undef_count; i++) {
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
            char name[VAR_NAME_MAX];
            if (nlen >= VAR_NAME_MAX) {
                continue;
            }
            memcpy(name, buf, nlen);
            name[nlen] = '\0';

            if (strcmp(name, s_undef[i]) != 0) {
                continue;
            }

            if (!var_set(name, eq_pos + 1)) {
                continue;
            }

            break;
        }
    }

    Node *tree = parse_eq(flags.equation);
    if (!tree) {
        if (!err_has()) {
            err_set(ERR_PARSE, "invalid expression: %s", flags.equation);
        }
        err_print();
        vars_free();
        return 1;
    }

    mpfr_t result;
    mpfr_init2(result, QM_PRECISION);

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
