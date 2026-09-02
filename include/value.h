#ifndef VALUE_H
#define VALUE_H

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum ValueType {
        VALUE_NULL = 0,
        VALUE_NUMBER = 1,
        VALUE_STRING = 2,
        VALUE_BOOL = 3
    } ValueType;

    typedef struct Value {
        ValueType type;
        union {
            double number;
            char* string;
            int boolean;
        } as;
    } Value;

    /* 建立各種 Value */
    Value value_null(void);
    Value value_number(double x);
    Value value_string(const char* s);
    Value value_bool(int b);

    /* 複製 / 判真 / 轉數字 / 輸出 */
    Value value_copy(Value v);
    int value_is_truthy(Value v);
    double value_as_number(Value v);
    void value_print(Value v);

    /* 釋放 Value 內部資源（主要是字串） */
    void value_free(Value v);

#ifdef __cplusplus
}
#endif

#endif