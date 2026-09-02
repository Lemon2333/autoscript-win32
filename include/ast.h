#ifndef AST_H
#define AST_H

#include "value.h"

typedef enum {
    EXPR_LITERAL,
    EXPR_VAR,
    EXPR_BINARY,
    EXPR_CALL,
    EXPR_PLUGIN_CALL
} ExprType;

typedef enum {
    STMT_ASSIGN,
    STMT_EXPR,
    STMT_IF
} StmtType;

typedef struct Expr Expr;
typedef struct Stmt Stmt;

struct Expr {
    ExprType type;
    union {
        Value literal;

        struct {
            char* name;
        } var;

        struct {
            char* op;
            Expr* left;
            Expr* right;
        } binary;

        struct {
            char* name;
            Expr** args;
            int arg_count;
        } call;

        struct {
            char* module_name;
            char* func_name;
            Expr** args;
            int arg_count;
        } plugin_call;
    } as;
};

struct Stmt {
    StmtType type;
    struct Stmt* next;

    union {
        struct {
            char* name;
            Expr* value;
        } assign;

        struct {
            Expr* expr;
        } expr_stmt;

        struct {
            Expr* condition;
            Stmt* then_branch;
            Stmt* else_branch;
        } if_stmt;
    } as;
};

Expr* new_literal_expr(Value v);
Expr* new_var_expr(const char* name);
Expr* new_binary_expr(const char* op, Expr* left, Expr* right);
Expr* new_call_expr(const char* name, Expr** args, int arg_count);
Expr* new_plugin_call_expr(const char* module_name, const char* func_name, Expr** args, int arg_count);

Stmt* new_assign_stmt(const char* name, Expr* value);
Stmt* new_expr_stmt(Expr* expr);
Stmt* new_if_stmt(Expr* cond, Stmt* then_branch, Stmt* else_branch);

void free_expr(Expr* expr);
void free_stmt_list(Stmt* stmt);

#endif