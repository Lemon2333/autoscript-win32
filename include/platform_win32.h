#ifndef PLATFORM_WIN32_H
#define PLATFORM_WIN32_H

#ifdef __cplusplus
extern "C" {
#endif

    /*
        找視窗
        - class_name: 視窗類別名稱，可傳 NULL
        - title: 視窗標題，可傳 NULL

        回傳：
        - 找到時回傳 hwnd（轉成 long long）
        - 找不到時回傳 0
    */
    long long platform_window_find(const char* class_name, const char* title);

    /*
        啟動/切換到某個視窗
        - hwnd_value: 由 platform_window_find 回傳的數值

        回傳：
        - 成功 1
        - 失敗 0
    */
    int platform_window_activate(long long hwnd_value);

#ifdef __cplusplus
}
#endif

#endif