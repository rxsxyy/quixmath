
#include "flags.h"

#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const OptDesc FLAG_TABLE[] = {
    {     "fb",  OPT_INT, offsetof(Flags,       base),  2},
    {     "fo",  OPT_INT, offsetof(Flags,       base),  8},
    {     "fd",  OPT_INT, offsetof(Flags,       base), 10},
    {     "fx",  OPT_INT, offsetof(Flags,       base), 16},
    {     "ib",  OPT_INT, offsetof(Flags, input_base),  2},
    {     "io",  OPT_INT, offsetof(Flags, input_base),  8},
    {     "id",  OPT_INT, offsetof(Flags, input_base), 10},
    {     "ix",  OPT_INT, offsetof(Flags, input_base), 16},
    {      "o",  OPT_STR, offsetof(Flags,   out_file),  0},
    {      "h", OPT_BOOL, offsetof(Flags,  show_help),  1},
    {   "help", OPT_BOOL, offsetof(Flags,  show_help),  1},
    {      "?", OPT_BOOL, offsetof(Flags,  show_help),  1},
    {      "v", OPT_BOOL, offsetof(Flags,   show_ver),  1},
    {"version", OPT_BOOL, offsetof(Flags,   show_ver),  1},
};

const usize FLAG_TABLE_LEN = sizeof(FLAG_TABLE) / sizeof(FLAG_TABLE[0]);

void print_help(const char *program_name) {
        printf("usage: %s [options] \"<equation>\"\n", program_name);
        printf("\noptions:\n");
        printf("  h, help, ?          show this help message\n");
        printf("  v, version          show program version\n");
        printf("  fb                  output result in binary (base 2)\n");
        printf("  fo                  output result in octal (base 8)\n");
        printf("  fd                  output result in decimal (base 10, default)\n");
        printf("  fx                  output result in hexadecimal (base 16)\n");
        printf("  ib                  parse equation numbers as binary\n");
        printf("  io                  parse equation numbers as octal\n");
        printf("  id                  parse equation numbers as decimal (default)\n");
        printf("  ix                  parse equation numbers as hexadecimal\n");
        printf("  o <file>            write output to file instead of stdout\n");
        printf("\nsupported operators: +, -, *, /, ^ (exponentiation)\n");
        printf("parentheses are supported for grouping.\n");
        printf("\nexamples:\n");
        printf("  %s \"2+3*4\"\n", program_name);
        printf("  %s ib \"101+10\"\n", program_name);
        printf("  %s ix \"1+1F\"\n", program_name);
        printf("  %s fb ib \"101+10\"\n", program_name);
        printf("  %s fx \"16+32\"\n", program_name);
        printf("  %s o \"result.txt\" \"(2+3)^4\"\n", program_name);
}

void print_ver(i32 ver) {
        printf("Quixmath v%d\n", ver);
}

/* Prints a signed integer in binary with two's complement representation
 * for negative numbers (shows full bit width of the actual value)
 */
static void fprint_binary(FILE *f, i64 v) {
        if (v == 0) {
                fputc('0', f);
                return;
        }

        u64 uv;
        if (v < 0) {
                uv = (u64)v;
        } else {
                uv = (u64)v;
        }

        i32 top = 0;
        for (i32 i = 62; i >= 0; i--) {
                if ((uv >> i) & 1ULL) {
                        top = i;
                        break;
                }
        }

        for (i32 i = top; i >= 0; i--) {
                fputc(((uv >> i) & 1ULL) ? '1' : '0', f);
        }
}

static void fprint_octal(FILE *f, i64 v) {
        if (v == 0) {
                fputc('0', f);
                return;
        }

        if (v < 0) {
                u64 uv = (u64)v;
                fprintf(f, "%lo", uv);
        } else {
                fprintf(f, "%lo", v);
        }
}

static void fprint_result(FILE *f, f64 result, i32 base) {
        i64 iv = (i64)result;
        switch (base) {
        case 2:
                fprint_binary(f, iv);
                fputc('\n', f);
                break;
        case 8:
                fprint_octal(f, iv);
                fputc('\n', f);
                break;
        case 16:
                if (iv < 0) {
                        fprintf(f, "%lx\n", (u64)iv);
                } else {
                        fprintf(f, "%lx\n", iv);
                }
                break;
        default:
                fprintf(f, "%g\n", result);
                break;
        }
}

i32 parse_flags(i32 argc, char *argv[], Flags *flags, i32 *eq_index) {
        flags->base = 10;
        flags->input_base = 10;
        flags->out_file = NULL;
        flags->show_help = 0;
        flags->show_ver = 0;
        *eq_index = -1;

        for (i32 i = 1; i < argc; i++) {
                const OptDesc *match = NULL;
                for (usize j = 0; j < FLAG_TABLE_LEN; j++) {
                        if (strcmp(argv[i], FLAG_TABLE[j].name) == 0) {
                                match = &FLAG_TABLE[j];
                                break;
                        }
                }

                if (!match) {
                        if (*eq_index != -1) {
                                fprintf(stderr, "error: unexpected argument '%s'\n", argv[i]);
                                return -1;
                        }
                        *eq_index = i;
                        continue;
                }

                void *field = (char *)flags + match->offset;
                switch (match->kind) {
                case OPT_INT:
                        *(i32 *)field = match->value;
                        break;
                case OPT_BOOL:
                        *(i32 *)field = match->value;
                        break;
                case OPT_STR:
                        if (i + 1 >= argc) {
                                fprintf(stderr, "error: %s requires an argument\n", match->name);
                                return -1;
                        }
                        free(*(char **)field);
                        *(char **)field = strdup(argv[++i]);
                        if (!*(char **)field) {
                                fprintf(stderr, "error: out of memory\n");
                                return -1;
                        }
                        break;
                }
        }

        if (*eq_index == -1 && !flags->show_help && !flags->show_ver) {
                fprintf(stderr, "error: no equation provided\n");
                return -1;
        }

        return 0;
}

i32 print_result(f64 result, const Flags *flags) {
        if (!flags->out_file) {
                fprint_result(stdout, result, flags->base);
                return 0;
        }

        FILE *f = fopen(flags->out_file, "w");
        if (!f) {
                fprintf(stderr, "error: could not open '%s' for writing\n", flags->out_file);
                return -1;
        }

        fprint_result(f, result, flags->base);
        fclose(f);
        return 0;
}

void free_flags(Flags *flags) {
        free(flags->out_file);
        flags->out_file = NULL;
}
