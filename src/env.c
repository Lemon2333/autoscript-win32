#include "env.h"
#include <stdlib.h>
#include <string.h>

static char* dupstr3(const char* s) {
    size_t n = strlen(s);
    char* p = (char*)malloc(n + 1);
    memcpy(p, s, n + 1);
    return p;
}

void env_init(Env* env) {
    env->head = NULL;
}

void env_free(Env* env) {
    Var* p = env->head;
    while (p) {
        Var* n = p->next;
        free(p->name);
        value_free(p->value);
        free(p);
        p = n;
    }
    env->head = NULL;
}

void env_set(Env* env, const char* name, Value v) {
    for (Var* p = env->head; p; p = p->next) {
        if (strcmp(p->name, name) == 0) {
            value_free(p->value);
            p->value = value_copy(v);
            return;
        }
    }

    Var* node = (Var*)calloc(1, sizeof(Var));
    node->name = dupstr3(name);
    node->value = value_copy(v);
    node->next = env->head;
    env->head = node;
}

Value env_get(Env* env, const char* name) {
    for (Var* p = env->head; p; p = p->next) {
        if (strcmp(p->name, name) == 0) {
            return value_copy(p->value);
        }
    }
    return value_null();
}