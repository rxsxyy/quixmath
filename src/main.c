
#include "!TYPES.h"

#include "flags.h"

i32 main(i32 argc, char **argv) {
    // check for flags, parse and run if there are any.
    OptionFlag flags = parse_flags(argc, argv);
    run_flags(&flags);

    // find equation, parse and evaluate.
    // Node equation = parse_eq(flags.equation);
    // f64 result = eval_eq(equation);
    // free_node(equation);
    
    // print result
    // print_result(result);

    return 0;
}
