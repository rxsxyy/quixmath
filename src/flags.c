#include "quixmath.h"

static const struct option LONG_OPTS[] = {
    {"help",    no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'v'},
    {NULL, 0, NULL, 0},
};

void print_help(void) {
    printf("usage: qm [options] <expression>\n\n");
    printf("options:\n");
    printf("  -h, --help       prints this help message;\n");
    printf("  -v, --version    prints version information;\n\n");
}

void print_ver(void) {
    printf("%s %s\n\n", PROGRAM, VERSION);
}

OptionFlag parse_flags(i32 argc, char **argv) {
    OptionFlag flags = {NULL, NULL, NULL, MK_NONE, 0};

    int opt;
    while ((opt = getopt_long(argc, argv, "hv", LONG_OPTS, NULL)) != -1) {
        switch (opt) {
        case 'h':
            if (flags.msg != MK_NONE) {
                err_set(ERR_FLAG, "conflicting flags");
                err_print();
                break;
            }
            flags.msg = MK_HELP;
            break;
        case 'v':
            if (flags.msg != MK_NONE) {
                err_set(ERR_FLAG, "conflicting flags");
                err_print();
                break;
            }
            flags.msg = MK_VER;
            break;
        case '?':
            break;
        }
    }

    for (i32 i = optind; i < argc; i++) {
        char *eq_pos = strchr(argv[i], '=');
        if (eq_pos != NULL) {
            usize nlen = (usize)(eq_pos - argv[i]);
            if (nlen == 0 || nlen >= VARIABLE_NAME_MAX) {
                err_set(ERR_FLAG, "invalid variable assignment: %s", argv[i]);
                err_print();
                continue;
            }
            char name[VARIABLE_NAME_MAX];
            strncpy(name, argv[i], nlen);
            name[nlen] = '\0';
            if (!var_set(name, eq_pos + 1)) {
                err_set(ERR_FLAG, "invalid value in assignment: %s", argv[i]);
                err_print();
            }
        } else if (flags.equation == NULL) {
            flags.equation = argv[i];
        } else {
            err_set(ERR_FLAG, "unknown flag or extra argument: %s", argv[i]);
            err_print();
        }
    }

    return flags;
}

void run_flags(const OptionFlag *flags) {
    switch (flags->msg) {
    case MK_VER:
        print_ver();
        break;
    case MK_HELP:
        print_help();
        break;
    case MK_NONE:
        print_help();
        break;
    }
}
