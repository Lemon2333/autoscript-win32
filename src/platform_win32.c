#include <stdint.h>
#include "platform_win32.h"

#ifdef _WIN32
#include <windows.h>

/*
    內部小工具：
    class_name 或 title 如果是空字串，就當成 NULL
*/
static HWND find_window_simple(const char* class_name, const char* title) {
    LPCSTR cls = NULL;
    LPCSTR ttl = NULL;

    if (class_name != NULL && class_name[0] != '\0') {
        cls = class_name;
    }

    if (title != NULL && title[0] != '\0') {
        ttl = title;
    }

    return FindWindowA(cls, ttl);
}

/*
    找視窗
*/
long long platform_window_find(const char* class_name, const char* title) {
    HWND hwnd = find_window_simple(class_name, title);
    return (long long)(intptr_t)hwnd;
}

/*
    啟動視窗
*/
int platform_window_activate(long long hwnd_value) {
    HWND hwnd = (HWND)(intptr_t)hwnd_value;

    if (hwnd == NULL) {
        return 0;
    }

    if (!IsWindow(hwnd)) {
        return 0;
    }

    /*
        若視窗最小化，先還原
    */
    ShowWindow(hwnd, SW_RESTORE);

    /*
        嘗試切到前景
    */
    SetForegroundWindow(hwnd);
    SetActiveWindow(hwnd);

    return 1;
}

#else

/*
    非 Windows 平台時，提供空實作
*/

long long platform_window_find(const char* class_name, const char* title) {
    (void)class_name;
    (void)title;
    return 0;
}

int platform_window_activate(long long hwnd_value) {
    (void)hwnd_value;
    return 0;
}

#endif