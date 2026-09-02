#include "plugin.h"
#include <stdio.h>
#include <string.h>

Value plugin_call(const char* module_name, const char* func_name, int argc, Value* argv) {
    if (strcmp(module_name, "Window") == 0 && strcmp(func_name, "Find") == 0) {
        if (argc == 2) {
            const char* title = value_as_string(argv[1]);
            if (strcmp(title, "Notepad") == 0) {
                return value_int(1001);
            }
        }
        return value_int(0);
    }

    printf("[plugin] unknown call: Plugin.%s.%s\n", module_name, func_name);
    return value_null();
}