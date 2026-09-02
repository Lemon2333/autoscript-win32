#ifndef ENV_H
#define ENV_H

#include "value.h"

typedef struct Var {
    char* name;
    Value value;
    struct Var* next;
} Var;

typedef struct {
    Var* head;
} Env;

void env_init(Env* env);
void env_free(Env* env);
void env_set(Env* env, const char* name, Value v);
Value env_get(Env* env, const char* name);

#endif