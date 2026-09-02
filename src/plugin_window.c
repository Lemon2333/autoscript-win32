#include <windows.h>
#include <stdint.h>
#include "plugin_window.h"
#include "plugin_manager.h"
#include "value.h"

static Value window_find(int argc, Value* argv) {
    HWND hwnd;
    const char* title;

    if (argc != 2) {
        return make_int(0);
    }

    if (argv[0].type != VALUE_INT) {
        return make_int(0);
    }

    if (argv[1].type != VALUE_STRING || argv[1].string_value == 0) {
        return make_int(0);
    }

    title = argv[1].string_value;
    hwnd = FindWindowA(0, title);

    return make_int((intptr_t)hwnd);
}

void register_window_plugin(void) {
    plugin_manager_register("Plugin.Window.Find", window_find);
}