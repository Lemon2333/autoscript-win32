#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    TokenArray* tokens;
    int pos;
} Parser;

static Token* peek(Parser* p) {
    return &p->tokens->items[p->pos];
}

static Token* peek_n(Parser* p, int n) {
    return &p->tokens->items[p->pos + n];
}

static int match(Parser* p, TokenType type) {
    if (peek(p)->type == type) {
        p->pos++;
        return 1;
    }
    return 0;
}

static Token* expect(Parser* p, TokenType type, const char* msg) {
    if (peek(p)->type != type) {
        printf("Parse error: %s, got %s\n", msg, peek(p)->text);
        exit(1);
    }
    return &p->tokens->items[p->pos++];
}

static Expr* parse_expression(Parser* p);

static Expr** parse_arg_list(Parser* p, int* out_count) {
    Expr** args = NULL;
    int count = 0;
    int cap = 0;

    if (peek(p)->type != TOK_RPAREN) {
        while (1) {
            Expr* arg = parse_expression(p);

            if (count >= cap) {
                cap = cap ? cap * 2 : 4;
                args = (Expr**)realloc(args, sizeof(Expr*) * cap);
            }
            args[count++] = arg;

            if (!match(p, TOK_COMMA)) break;
        }
    }

    *out_count = count;
    return args;
}

static Expr* parse_primary(Parser* p) {
    Token* t = peek(p);

    if (match(p, TOK_NUMBER)) return new_literal_expr(value_int(t->number));
    if (match(p, TOK_STRING)) return new_literal_expr(value_string(t->text));

    if (match(p, TOK_IDENT)) {
        char* name = t->text;

        if (strcmp(name, "Plugin") == 0 && peek(p)->type == TOK_DOT) {
            match(p, TOK_DOT);
            Token* mod = expect(p, TOK_IDENT, "expected plugin module");
            expect(p, TOK_DOT, "expected '.'");
            Token* fn = expect(p, TOK_IDENT, "expected plugin function");
            expect(p, TOK_LPAREN, "expected '('");
            int argc = 0;
            Expr** args = parse_arg_list(p, &argc);
            expect(p, TOK_RPAREN, "expected ')'");
            return new_plugin_call_expr(mod->text, fn->text, args, argc);
        }

        if (peek(p)->type == TOK_LPAREN) {
            match(p, TOK_LPAREN);
            int argc = 0;
            Expr** args = parse_arg_list(p, &argc);
            expect(p, TOK_RPAREN, "expected ')'");
            return new_call_expr(name, args, argc);
        }

        return new_var_expr(name);
    }

    printf("Parse error: unexpected token %s\n", t->text);
    exit(1);
}

static Expr* parse_equality(Parser* p) {
    Expr* left = parse_primary(p);

    while (peek(p)->type == TOK_ASSIGN || peek(p)->type == TOK_NEQ) {
        Token* op = peek(p);
        p->pos++;
        Expr* right = parse_primary(p);
        left = new_binary_expr(op->text, left, right);
    }

    return left;
}

static Expr* parse_expression(Parser* p) {
    return parse_equality(p);
}

static Stmt* parse_statement(Parser* p);

static Stmt* append_stmt(Stmt* head, Stmt* node) {
    if (!head) return node;
    Stmt* cur = head;
    while (cur->next) cur = cur->next;
    cur->next = node;
    return head;
}

static Stmt* parse_block(Parser* p, int stop_on_else, int stop_on_end) {
    Stmt* head = NULL;

    while (peek(p)->type != TOK_EOF) {
        if (stop_on_else && peek(p)->type == TOK_ELSE) break;
        if (stop_on_end && peek(p)->type == TOK_END) break;
        head = append_stmt(head, parse_statement(p));
    }

    return head;
}

static Stmt* parse_if(Parser* p) {
    expect(p, TOK_IF, "expected If");
    Expr* cond = parse_expression(p);
    expect(p, TOK_THEN, "expected Then");

    Stmt* then_branch = parse_block(p, 1, 1);
    Stmt* else_branch = NULL;

    if (match(p, TOK_ELSE)) {
        else_branch = parse_block(p, 0, 1);
    }

    expect(p, TOK_END, "expected End");
    expect(p, TOK_IF, "expected If after End");

    return new_if_stmt(cond, then_branch, else_branch);
}

static Stmt* parse_statement(Parser* p) {
    if (peek(p)->type == TOK_IF) {
        return parse_if(p);
    }

    if (peek(p)->type == TOK_IDENT && peek_n(p, 1)->type == TOK_ASSIGN) {
        Token* name = expect(p, TOK_IDENT, "expected identifier");
        expect(p, TOK_ASSIGN, "expected '='");
        Expr* value = parse_expression(p);
        return new_assign_stmt(name->text, value);
    }

    Expr* expr = parse_expression(p);
    return new_expr_stmt(expr);
}

Stmt* parse_program(TokenArray* tokens) {
    Parser p;
    p.tokens = tokens;
    p.pos = 0;
    return parse_block(&p, 0, 0);
}