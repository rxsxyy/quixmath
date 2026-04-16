
#include "!TYPES.h"

#include "flags.h"

i32 main(i32 argc, char **argv) {
        // check for flags, parse and run if there are any.
        OptionFlag flags = parse_flags(argc, argv);
        run_flags(&flags);
}
