#include "builtins.h"
#include <stdio.h>
#include <string.h>

Value builtin_call(const char* name, int argc, Value* argv) {
    if (strcmp(name, "SayString") == 0) {
        for (int i = 0; i < argc; i++) {
            value_print(argv[i]);
            if (i + 1 < argc) printf(" ");
        }
        printf("\n");
        return value_null();
    }

    printf("[builtin] unknown function: %s\n", name);
    return value_null();
}