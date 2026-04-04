
#include "flags.h"
#include "parse.h"
#include "values.h"

#include <math.h>
#include <stdio.h>

// DON'T FORGET to change this
#define VERSION 1

// static const char *op_str[] = {"+", "-", "*", "/", "^"};

/* Prints the equation node along side the depth of each element.
 *
 * Example:
 * $ qm 1+(2+3)
 * +
 *  2
 *  2
 * This roughly translates to AST as:
 *                +
 *               / \
 *              1   +
 *                 / \
 *                2   3
 *
 * This function goes unused, but can be activated manually by editing the
 * source code.
 * I don't want to make a flag for this.
 */
// static void print_tree(const Node *node, i32 depth) {
//         if (!node)
//                 return;
//         for (i32 i = 0; i < depth; i++)
//                 printf(" ");
//         if (node->kind == NODE_NUM) {
//                 printf("%g\n", node->value);
//         } else {
//                 printf("%s\n", op_str[node->op]);
//                 print_tree(node->lhs, depth + 1);
//                 print_tree(node->rhs, depth + 1);
//         }
// }

/* Calculates the final result of the equation node, following the depth system
 * from the Node struct.
 *
 * Recursively traverses the AST, evaluating left and right children before
 * applying the parent operator. Leaf nodes (NODE_NUM) return their value
 * directly. Returns 0.0 for NULL nodes or unrecognized operators.
 */
f64 calculate(const Node *node) {
        if (!node) {
                return 0.0;
        }
        if (node->kind == NODE_NUM) {
                return node->value;
        }
        f64 l = calculate(node->lhs);
        f64 r = calculate(node->rhs);
        switch (node->op) {
        case OP_ADD:
                return l + r;
        case OP_SUB:
                return l - r;
        case OP_MUL:
                return l * r;
        case OP_DIV:
                return l / r;
        case OP_EXP:
                return pow(l, r);
        }
        return 0.0;
}

/* Detects an optional input-base prefix on the equation string.
 * Recognises 'b', 'o', and 'x' as the first character when the rest of
 * the string is a valid equation (i.e. starts with a digit, '(', '-', or
 * whitespace). Sets *input_base and advances *eq past the prefix character.
 *
 * b"..." -> base  2 (binary)
 * o"..." -> base  8 (octal)
 * x"..." -> base 16 (hexadecimal)
 * 
 * UNUSED, REPLACED BY REGULAR FLAGS, THIS WAS SUCH A STUPID ADDITION.
 */
// static void detect_input_base(const char **eq, i32 *input_base) {
//         char prefix = (*eq)[0];
//         char next = (*eq)[1];

//         if (next == '\0')
//                 return;

//         if (prefix == 'b') {
//                 *input_base = 2;
//                 (*eq)++;
//         } else if (prefix == 'o') {
//                 *input_base = 8;
//                 (*eq)++;
//         } else if (prefix == 'x') {
//                 *input_base = 16;
//                 (*eq)++;
//         }
// }

i32 main(i32 argc, char *argv[]) {
        if (argc < 2) {
                fprintf(stderr, "usage: %s [options] \"<equation>\"\n", argv[0]);
                return 1;
        }

        Flags flags;
        i32 eq_index;
        if (parse_flags(argc, argv, &flags, &eq_index) != 0) {
                return 1;
        }

        if (flags.show_help) {
                print_help(argv[0]);
                free_flags(&flags);
                return 0;
        }
        
        if (flags.show_ver) {
                print_ver(VERSION);
                free_flags(&flags);
                return 0;
        }

        const char *eq_str = argv[eq_index];

        Node *root = parse_eq(eq_str, flags.input_base);
        if (!root) {
                fprintf(stderr, "failed to parse equation: %s\n", argv[eq_index]);
                free_flags(&flags);
                return 1;
        }

        f64 result = calculate(root);
        free_node(root);

        i32 ret = print_result(result, &flags);
        free_flags(&flags);
        return ret != 0 ? 1 : 0;
}
