#ifndef VALUE_H
#define VALUE_H

typedef enum {
    VAL_NULL,
    VAL_INT,
    VAL_BOOL,
    VAL_STRING
} ValueType;

typedef struct {
    ValueType type;
    union {
        long long i;
        int b;
        char* s;
    } as;
} Value;

Value value_null(void);
Value value_int(long long x);
Value value_bool(int b);
Value value_string(const char* s);

Value value_copy(Value v);
void value_free(Value v);
void value_print(Value v);

int value_is_truthy(Value v);
long long value_as_int(Value v);
const char* value_as_string(Value v);

#endif