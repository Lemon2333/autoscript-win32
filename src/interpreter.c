#include "interpreter.h"
#include "builtins.h"
#include "plugin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Value eval_expr(Expr* expr, Env* env) {
    switch (expr->type) {
    case EXPR_LITERAL:
        return value_copy(expr->as.literal);

    case EXPR_VAR:
        return env_get(env, expr->as.var.name);

    case EXPR_BINARY: {
        Value l = eval_expr(expr->as.binary.left, env);
        Value r = eval_expr(expr->as.binary.right, env);

        Value result = value_null();

        if (strcmp(expr->as.binary.op, "=") == 0) {
            if (l.type == VAL_INT && r.type == VAL_INT) {
                result = value_bool(l.as.i == r.as.i);
            }
            else if (l.type == VAL_STRING && r.type == VAL_STRING) {
                result = value_bool(strcmp(l.as.s, r.as.s) == 0);
            }
            else {
                result = value_bool(0);
            }
        }
        else if (strcmp(expr->as.binary.op, "<>") == 0) {
            if (l.type == VAL_INT && r.type == VAL_INT) {
                result = value_bool(l.as.i != r.as.i);
            }
            else if (l.type == VAL_STRING && r.type == VAL_STRING) {
                result = value_bool(strcmp(l.as.s, r.as.s) != 0);
            }
            else {
                result = value_bool(1);
            }
        }

        value_free(l);
        value_free(r);
        return result;
    }

    case EXPR_CALL: {
        int argc = expr->as.call.arg_count;
        Value* argv = (Value*)calloc(argc, sizeof(Value));
        for (int i = 0; i < argc; i++) {
            argv[i] = eval_expr(expr->as.call.args[i], env);
        }

        Value ret = builtin_call(expr->as.call.name, argc, argv);

        for (int i = 0; i < argc; i++) value_free(argv[i]);
        free(argv);
        return ret;
    }

    case EXPR_PLUGIN_CALL: {
        int argc = expr->as.plugin_call.arg_count;
        Value* argv = (Value*)calloc(argc, sizeof(Value));
        for (int i = 0; i < argc; i++) {
            argv[i] = eval_expr(expr->as.plugin_call.args[i], env);
        }

        Value ret = plugin_call(
            expr->as.plugin_call.module_name,
            expr->as.plugin_call.func_name,
            argc,
            argv
        );

        for (int i = 0; i < argc; i++) value_free(argv[i]);
        free(argv);
        return ret;
    }
    }

    return value_null();
}

static void exec_stmt(Stmt* stmt, Env* env) {
    switch (stmt->type) {
    case STMT_ASSIGN: {
        Value v = eval_expr(stmt->as.assign.value, env);
        env_set(env, stmt->as.assign.name, v);
        value_free(v);
        break;
    }

    case STMT_EXPR: {
        Value v = eval_expr(stmt->as.expr_stmt.expr, env);
        value_free(v);
        break;
    }

    case STMT_IF: {
        Value cond = eval_expr(stmt->as.if_stmt.condition, env);
        Stmt* branch = value_is_truthy(cond)
            ? stmt->as.if_stmt.then_branch
            : stmt->as.if_stmt.else_branch;
        value_free(cond);

        for (Stmt* p = branch; p; p = p->next) {
            exec_stmt(p, env);
        }
        break;
    }
    }
}

void interpret(Stmt* program, Env* env) {
    for (Stmt* p = program; p; p = p->next) {
        exec_stmt(p, env);
    }
}