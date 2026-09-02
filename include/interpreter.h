#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"
#include "env.h"

void interpret(Stmt* program, Env* env);

#endif