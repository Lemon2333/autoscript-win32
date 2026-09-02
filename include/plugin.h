#ifndef PLUGIN_H
#define PLUGIN_H

#include "value.h"

Value plugin_call(const char* module_name, const char* func_name, int argc, Value* argv);

#endif