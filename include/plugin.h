#ifndef PLUGIN_H
#define PLUGIN_H

#include "value.h"

#ifdef __cplusplus
extern "C" {
#endif

	/* 判斷某個函式名稱是否由 plugin 系統處理 */
	int plugin_can_handle(const char* name);

	/* 執行 plugin 函式呼叫 */
	Value plugin_call(const char* name, Value* args, int argc);

#ifdef __cplusplus
}
#endif

#endif