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
            if (l.type == VALUE_NUMBER && r.type == VALUE_NUMBER) {
                result = value_bool(l.as.number == r.as.number);
            }
            else if (l.type == VALUE_STRING && r.type == VALUE_STRING) {
                if (l.as.string != NULL && r.as.string != NULL) {
                    result = value_bool(strcmp(l.as.string, r.as.string) == 0);
                }
                else {
                    result = value_bool(l.as.string == r.as.string);
                }
            }
            else if (l.type == VALUE_BOOL && r.type == VALUE_BOOL) {
                result = value_bool(l.as.boolean == r.as.boolean);
            }
            else if (l.type == VALUE_NULL && r.type == VALUE_NULL) {
                result = value_bool(1);
            }
            else {
                result = value_bool(0);
            }
        }
        else if (strcmp(expr->as.binary.op, "<>") == 0) {
            if (l.type == VALUE_NUMBER && r.type == VALUE_NUMBER) {
                result = value_bool(l.as.number != r.as.number);
            }
            else if (l.type == VALUE_STRING && r.type == VALUE_STRING) {
                if (l.as.string != NULL && r.as.string != NULL) {
                    result = value_bool(strcmp(l.as.string, r.as.string) != 0);
                }
                else {
                    result = value_bool(l.as.string != r.as.string);
                }
            }
            else if (l.type == VALUE_BOOL && r.type == VALUE_BOOL) {
                result = value_bool(l.as.boolean != r.as.boolean);
            }
            else if (l.type == VALUE_NULL && r.type == VALUE_NULL) {
                result = value_bool(0);
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
        Value* argv = (Value*)calloc((size_t)argc, sizeof(Value));
        if (argv == NULL && argc > 0) {
            return value_null();
        }

        for (int i = 0; i < argc; i++) {
            argv[i] = eval_expr(expr->as.call.args[i], env);
        }

        Value ret = builtin_call(expr->as.call.name, argc, argv);

        for (int i = 0; i < argc; i++) {
            value_free(argv[i]);
        }
        free(argv);
        return ret;
    }

    case EXPR_PLUGIN_CALL: {
        int argc = expr->as.plugin_call.arg_count;
        Value* argv = (Value*)calloc((size_t)argc, sizeof(Value));
        if (argv == NULL && argc > 0) {
            return value_null();
        }

        for (int i = 0; i < argc; i++) {
            argv[i] = eval_expr(expr->as.plugin_call.args[i], env);
        }

        /*
            你的 plugin.h 現在是：
            Value plugin_call(const char* name, Value* args, int argc);

            所以把 module + func 組成：
            "Plugin.Window.Find"
            "Plugin.Window.Activate"
        */
        {
            char full_name[256];
            full_name[0] = '\0';

#ifdef _MSC_VER
            _snprintf_s(
                full_name,
                sizeof(full_name),
                _TRUNCATE,
                "Plugin.%s.%s",
                expr->as.plugin_call.module_name,
                expr->as.plugin_call.func_name
            );
#else
            snprintf(
                full_name,
                sizeof(full_name),
                "Plugin.%s.%s",
                expr->as.plugin_call.module_name,
                expr->as.plugin_call.func_name
            );
#endif

            Value ret = plugin_call(full_name, argv, argc);

            for (int i = 0; i < argc; i++) {
                value_free(argv[i]);
            }
            free(argv);
            return ret;
        }
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