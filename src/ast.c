#include "ast.h"
#include <stdlib.h>
#include <string.h>

static char* dupstr2(const char* s) {
    size_t n = strlen(s);
    char* p = (char*)malloc(n + 1);
    memcpy(p, s, n + 1);
    return p;
}

Expr* new_literal_expr(Value v) {
    Expr* e = (Expr*)calloc(1, sizeof(Expr));
    e->type = EXPR_LITERAL;
    e->as.literal = value_copy(v);
    return e;
}

Expr* new_var_expr(const char* name) {
    Expr* e = (Expr*)calloc(1, sizeof(Expr));
    e->type = EXPR_VAR;
    e->as.var.name = dupstr2(name);
    return e;
}

Expr* new_binary_expr(const char* op, Expr* left, Expr* right) {
    Expr* e = (Expr*)calloc(1, sizeof(Expr));
    e->type = EXPR_BINARY;
    e->as.binary.op = dupstr2(op);
    e->as.binary.left = left;
    e->as.binary.right = right;
    return e;
}

Expr* new_call_expr(const char* name, Expr** args, int arg_count) {
    Expr* e = (Expr*)calloc(1, sizeof(Expr));
    e->type = EXPR_CALL;
    e->as.call.name = dupstr2(name);
    e->as.call.args = args;
    e->as.call.arg_count = arg_count;
    return e;
}

Expr* new_plugin_call_expr(const char* module_name, const char* func_name, Expr** args, int arg_count) {
    Expr* e = (Expr*)calloc(1, sizeof(Expr));
    e->type = EXPR_PLUGIN_CALL;
    e->as.plugin_call.module_name = dupstr2(module_name);
    e->as.plugin_call.func_name = dupstr2(func_name);
    e->as.plugin_call.args = args;
    e->as.plugin_call.arg_count = arg_count;
    return e;
}

Stmt* new_assign_stmt(const char* name, Expr* value) {
    Stmt* s = (Stmt*)calloc(1, sizeof(Stmt));
    s->type = STMT_ASSIGN;
    s->as.assign.name = dupstr2(name);
    s->as.assign.value = value;
    return s;
}

Stmt* new_expr_stmt(Expr* expr) {
    Stmt* s = (Stmt*)calloc(1, sizeof(Stmt));
    s->type = STMT_EXPR;
    s->as.expr_stmt.expr = expr;
    return s;
}

Stmt* new_if_stmt(Expr* cond, Stmt* then_branch, Stmt* else_branch) {
    Stmt* s = (Stmt*)calloc(1, sizeof(Stmt));
    s->type = STMT_IF;
    s->as.if_stmt.condition = cond;
    s->as.if_stmt.then_branch = then_branch;
    s->as.if_stmt.else_branch = else_branch;
    return s;
}

void free_expr(Expr* expr) {
    if (!expr) return;

    switch (expr->type) {
    case EXPR_LITERAL:
        value_free(expr->as.literal);
        break;
    case EXPR_VAR:
        free(expr->as.var.name);
        break;
    case EXPR_BINARY:
        free(expr->as.binary.op);
        free_expr(expr->as.binary.left);
        free_expr(expr->as.binary.right);
        break;
    case EXPR_CALL:
        free(expr->as.call.name);
        for (int i = 0; i < expr->as.call.arg_count; i++) free_expr(expr->as.call.args[i]);
        free(expr->as.call.args);
        break;
    case EXPR_PLUGIN_CALL:
        free(expr->as.plugin_call.module_name);
        free(expr->as.plugin_call.func_name);
        for (int i = 0; i < expr->as.plugin_call.arg_count; i++) free_expr(expr->as.plugin_call.args[i]);
        free(expr->as.plugin_call.args);
        break;
    }

    free(expr);
}

void free_stmt_list(Stmt* stmt) {
    while (stmt) {
        Stmt* next = stmt->next;

        switch (stmt->type) {
        case STMT_ASSIGN:
            free(stmt->as.assign.name);
            free_expr(stmt->as.assign.value);
            break;
        case STMT_EXPR:
            free_expr(stmt->as.expr_stmt.expr);
            break;
        case STMT_IF:
            free_expr(stmt->as.if_stmt.condition);
            free_stmt_list(stmt->as.if_stmt.then_branch);
            free_stmt_list(stmt->as.if_stmt.else_branch);
            break;
        }

        free(stmt);
        stmt = next;
    }
}