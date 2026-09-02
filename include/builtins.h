#ifndef BUILTINS_H
#define BUILTINS_H

#include "value.h"

Value builtin_call(const char* name, int argc, Value* argv);

#endif