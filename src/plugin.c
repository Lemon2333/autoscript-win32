#include <string.h>
#include "plugin.h"
#include "platform_win32.h"
#include "value.h"

/*
    這個小工具函式用來比較兩個字串是否完全相同。
    例如：
    "FindWindow" 和 "FindWindow" -> 相同 -> 回傳 1
    "FindWindow" 和 "ActivateWindow" -> 不同 -> 回傳 0
*/
static int streq(const char* a, const char* b) {
    if (a == NULL || b == NULL) {
        return 0;
    }
    return strcmp(a, b) == 0;
}

/*
    判斷這個名稱是不是「找視窗」函式。
    我們同時支援兩種名字：

    1. Plugin.Window.Find
    2. FindWindow
*/
static int is_find_name(const char* name) {
    if (streq(name, "Plugin.Window.Find")) {
        return 1;
    }

    if (streq(name, "FindWindow")) {
        return 1;
    }

    return 0;
}

/*
    判斷這個名稱是不是「啟動視窗」函式。
    同樣支援兩種名字：

    1. Plugin.Window.Activate
    2. ActivateWindow
*/
static int is_activate_name(const char* name) {
    if (streq(name, "Plugin.Window.Activate")) {
        return 1;
    }

    if (streq(name, "ActivateWindow")) {
        return 1;
    }

    return 0;
}

/*
    interpreter 會先問：
    「這個函式名稱你 plugin 會不會處理？」
*/
int plugin_can_handle(const char* name) {
    if (name == NULL) {
        return 0;
    }

    if (is_find_name(name)) {
        return 1;
    }

    if (is_activate_name(name)) {
        return 1;
    }

    return 0;
}

/*
    真正執行 plugin 呼叫。

    參數說明：
    - name: 函式名稱，例如 "FindWindow"
    - args: 參數陣列
    - argc: 參數數量
*/
Value plugin_call(const char* name, Value* args, int argc) {
    /*
        --------------------------------------------------
        處理 FindWindow / Plugin.Window.Find
        --------------------------------------------------

        用法：
            hwnd = FindWindow(0, "Untitled - Notepad")
        或：
            hwnd = Plugin.Window.Find(0, "Untitled - Notepad")

        參數：
        - 第 1 個參數：class name
            可以是字串，例如 "Notepad"
            也可以是 0，表示不指定 class
        - 第 2 個參數：title
            可以是字串，例如 "Untitled - Notepad"
            也可以是 0，表示不指定 title
    */
    if (is_find_name(name)) {
        const char* class_name = NULL;
        const char* title = NULL;
        long long hwnd_value = 0;

        /*
            讀取第 1 個參數：class_name
        */
        if (argc >= 1) {
            if (args[0].type == VALUE_STRING) {
                class_name = args[0].as.string;
            }
            else if (args[0].type == VALUE_NUMBER && args[0].as.number == 0) {
                class_name = NULL;
            }
        }

        /*
            讀取第 2 個參數：title
        */
        if (argc >= 2) {
            if (args[1].type == VALUE_STRING) {
                title = args[1].as.string;
            }
            else if (args[1].type == VALUE_NUMBER && args[1].as.number == 0) {
                title = NULL;
            }
        }

        /*
            呼叫 Win32 平台層
        */
        hwnd_value = platform_window_find(class_name, title);

        /*
            把結果包成腳本可理解的數字 Value 回傳
        */
        return value_number((double)hwnd_value);
    }

    /*
        --------------------------------------------------
        處理 ActivateWindow / Plugin.Window.Activate
        --------------------------------------------------

        用法：
            ActivateWindow(hwnd)
        或：
            Plugin.Window.Activate(hwnd)

        參數：
        - 第 1 個參數：hwnd（數字）
    */
    if (is_activate_name(name)) {
        long long hwnd_value = 0;
        int ok = 0;

        /*
            讀取第 1 個參數
        */
        if (argc >= 1) {
            if (args[0].type == VALUE_NUMBER) {
                hwnd_value = (long long)args[0].as.number;
            }
        }

        /*
            呼叫 Win32 平台層
        */
        ok = platform_window_activate(hwnd_value);

        /*
            回傳 1 表示成功，0 表示失敗
        */
        return value_number((double)ok);
    }

    /*
        如果不是 plugin 能處理的函式，就回傳 null
    */
    return value_null();
}