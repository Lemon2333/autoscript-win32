#include <string.h>
#include "plugin_manager.h"

#define MAX_PLUGIN_FUNCTIONS 128

static PluginFunctionEntry g_functions[MAX_PLUGIN_FUNCTIONS];
static int g_function_count = 0;

void plugin_manager_init(void) {
    g_function_count = 0;
}

int plugin_manager_register(const char* name, PluginFunction func) {
    if (g_function_count >= MAX_PLUGIN_FUNCTIONS) {
        return 0;
    }

    g_functions[g_function_count].name = name;
    g_functions[g_function_count].func = func;
    g_function_count++;
    return 1;
}

PluginFunction plugin_manager_find(const char* name) {
    int i;
    for (i = 0; i < g_function_count; i++) {
        if (strcmp(g_functions[i].name, name) == 0) {
            return g_functions[i].func;
        }
    }
    return 0;
}