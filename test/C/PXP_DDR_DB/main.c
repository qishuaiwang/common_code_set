#include <stdio.h>
#include <stdlib.h>
#include "args_analysis.h"

int main(int argc, char** argv) {
    // Instantiate a new ArgParser instance.
    ArgParser* parser = ap_new();
    if (!parser) {
        exit(1);
    }

    // Register the program's helptext and version number.
    ap_set_helptext(parser, "Usage: example...");
    ap_set_version(parser, "1.0");

    // Register a flag and a string-valued and a integer-valued option.
    ap_flag(parser, "intlv l");
    ap_str_opt(parser, "input_file i", "ddrc.bin");
    ap_long_opt(parser, "address a", 0x3000000000);

    // Parse the command line arguments.
    if (!ap_parse(parser, argc, argv)) {
        exit(1);
    }

    // This debugging function prints the parser's state.
    ap_print(parser);

    printf("Interleave is %d\n", ap_found(parser, "l"));
    printf("Input_file is %s\n", ap_str_value(parser, "input_file"));
    printf("Load address is %#lx\n", ap_long_value(parser, "a"));
    // Free the parser's memory.
    ap_free(parser);
}