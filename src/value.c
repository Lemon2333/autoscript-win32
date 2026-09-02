#include "value.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* dupstr(const char* s) {
    size_t n = strlen(s);
    char* p = (char*)malloc(n + 1);
    memcpy(p, s, n + 1);
    return p;
}

Value value_null(void) {
    Value v;
    v.type = VAL_NULL;
    return v;
}

Value value_int(long long x) {
    Value v;
    v.type = VAL_INT;
    v.as.i = x;
    return v;
}

Value value_bool(int b) {
    Value v;
    v.type = VAL_BOOL;
    v.as.b = b ? 1 : 0;
    return v;
}

Value value_string(const char* s) {
    Value v;
    v.type = VAL_STRING;
    v.as.s = dupstr(s ? s : "");
    return v;
}

Value value_copy(Value v) {
    if (v.type == VAL_STRING) return value_string(v.as.s);
    return v;
}

void value_free(Value v) {
    if (v.type == VAL_STRING && v.as.s) free(v.as.s);
}

void value_print(Value v) {
    switch (v.type) {
    case VAL_NULL:   printf("null"); break;
    case VAL_INT:    printf("%lld", v.as.i); break;
    case VAL_BOOL:   printf("%s", v.as.b ? "True" : "False"); break;
    case VAL_STRING: printf("%s", v.as.s); break;
    }
}

int value_is_truthy(Value v) {
    switch (v.type) {
    case VAL_NULL: return 0;
    case VAL_BOOL: return v.as.b;
    case VAL_INT: return v.as.i != 0;
    case VAL_STRING: return v.as.s && v.as.s[0] != '\0';
    }
    return 0;
}

long long value_as_int(Value v) {
    if (v.type == VAL_INT) return v.as.i;
    if (v.type == VAL_BOOL) return v.as.b;
    return 0;
}

const char* value_as_string(Value v) {
    if (v.type == VAL_STRING) return v.as.s;
    return "";
}