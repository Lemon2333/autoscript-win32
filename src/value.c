#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "value.h"

/*
    因為有些編譯器對 strdup 支援不一致，
    我們自己做一個簡單版本。
*/
static char* value_strdup(const char* s) {
    char* copy;
    size_t len;

    if (s == NULL) {
        return NULL;
    }

    len = strlen(s);
    copy = (char*)malloc(len + 1);
    if (copy == NULL) {
        return NULL;
    }

    memcpy(copy, s, len + 1);
    return copy;
}

Value value_null(void) {
    Value v;
    v.type = VALUE_NULL;
    v.as.number = 0;
    return v;
}

Value value_number(double x) {
    Value v;
    v.type = VALUE_NUMBER;
    v.as.number = x;
    return v;
}

Value value_string(const char* s) {
    Value v;
    v.type = VALUE_STRING;
    v.as.string = value_strdup(s);
    return v;
}

Value value_bool(int b) {
    Value v;
    v.type = VALUE_BOOL;
    v.as.boolean = (b != 0) ? 1 : 0;
    return v;
}

Value value_copy(Value v) {
    switch (v.type) {
    case VALUE_STRING:
        return value_string(v.as.string);
    case VALUE_NUMBER:
        return value_number(v.as.number);
    case VALUE_BOOL:
        return value_bool(v.as.boolean);
    case VALUE_NULL:
    default:
        return value_null();
    }
}

int value_is_truthy(Value v) {
    switch (v.type) {
    case VALUE_NULL:
        return 0;
    case VALUE_BOOL:
        return v.as.boolean != 0;
    case VALUE_NUMBER:
        return v.as.number != 0;
    case VALUE_STRING:
        return (v.as.string != NULL && v.as.string[0] != '\0');
    default:
        return 0;
    }
}

double value_as_number(Value v) {
    switch (v.type) {
    case VALUE_NUMBER:
        return v.as.number;
    case VALUE_BOOL:
        return v.as.boolean ? 1.0 : 0.0;
    case VALUE_NULL:
        return 0.0;
    case VALUE_STRING:
    default:
        return 0.0;
    }
}

void value_print(Value v) {
    switch (v.type) {
    case VALUE_NULL:
        printf("null");
        break;
    case VALUE_NUMBER:
        printf("%g", v.as.number);
        break;
    case VALUE_STRING:
        printf("%s", v.as.string ? v.as.string : "");
        break;
    case VALUE_BOOL:
        printf("%s", v.as.boolean ? "true" : "false");
        break;
    default:
        printf("null");
        break;
    }
}

void value_free(Value v) {
    if (v.type == VALUE_STRING) {
        free(v.as.string);
    }
}