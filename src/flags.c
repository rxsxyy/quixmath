
#include "flags.h"

#include "!INFO.h"
#include "error.h"
#include "stddef.h"
#include "stdio.h"
#include "string.h"

const Option OPTION_TABLE[] = {
    // help options
    {       "-h",   "prints this help message;", O_INT, offsetof(OptionFlag, msg), MK_HELP},
    {   "--help",                            "", O_INT, offsetof(OptionFlag, msg), MK_HELP},
    // version options
    {       "-v", "prints version information;", O_INT, offsetof(OptionFlag, msg),  MK_VER},
    {"--version",                            "", O_INT, offsetof(OptionFlag, msg),  MK_VER},
};

const usize OPTION_TABLE_LEN = sizeof(OPTION_TABLE) / sizeof(OPTION_TABLE[0]);

OptionFlag parse_flags(i32 argc, char **argv) {
    OptionFlag flags = {NULL, NULL, NULL, MK_NONE, 0};

    for (i32 i = 1; i < argc; i++) {
        bool matched = false;

        for (usize j = 0; j < OPTION_TABLE_LEN; j++) {
            if (strcmp(argv[i], OPTION_TABLE[j].name) != 0)
                continue;

            char *base = (char *)&flags;

            switch (OPTION_TABLE[j].kind) {
            case O_BOOL:
                *(bool *)(base + OPTION_TABLE[j].offset) = (bool)OPTION_TABLE[j].value;
                break;
            case O_INT:
                if (OPTION_TABLE[j].offset == offsetof(OptionFlag, msg) && flags.msg != MK_NONE) {
                    err_set(ERR_FLAG, "unknown flag or extra argument: %s", argv[i]);
                    err_print();
                    matched = true;
                    goto next; // don't be scared now
                }
                *(i32 *)(base + OPTION_TABLE[j].offset) = OPTION_TABLE[j].value;
                break;
            case O_STR:
                if (i + 1 < argc)
                    *(char **)(base + OPTION_TABLE[j].offset) = argv[++i];
                break;
            case O_NONE:
                break;
            }

            matched = true;
            break;
        }
    next: // boo!

        if (!matched) {
            if (argv[i][0] != '-' && flags.equation == NULL) {
                flags.equation = argv[i];
            } else {
                err_set(ERR_FLAG, "unknown flag or extra argument: %s", argv[i]);
                err_print();
            }
        }
    }

    return flags;
}

void print_help() {
    printf("usage: qm [options] <expression>\n\n");
    printf("options:\n");
    for (usize i = 0; i < OPTION_TABLE_LEN; i++) {
        printf("  %-14s %s\n", OPTION_TABLE[i].name, OPTION_TABLE[i].desc);
    }
}

void print_ver() {
    printf("%s %s \n", PROGRAM, VERSION);
}

void run_flags(const OptionFlag *flags) {
    switch (flags->msg) {
    case MK_ERR:
        break;
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
