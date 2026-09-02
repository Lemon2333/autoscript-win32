#include "lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char* substr(const char* s, int start, int len) {
    char* p = (char*)malloc(len + 1);
    memcpy(p, s + start, len);
    p[len] = '\0';
    return p;
}

static void push_token(TokenArray* arr, Token t) {
    if (arr->count >= arr->capacity) {
        arr->capacity = arr->capacity ? arr->capacity * 2 : 64;
        arr->items = (Token*)realloc(arr->items, sizeof(Token) * arr->capacity);
    }
    arr->items[arr->count++] = t;
}

static int ieq(const char* a, const char* b) {
    while (*a && *b) {
        char ca = (char)tolower((unsigned char)*a);
        char cb = (char)tolower((unsigned char)*b);
        if (ca != cb) return 0;
        a++; b++;
    }
    return *a == 0 && *b == 0;
}

TokenArray lex_source(const char* src) {
    TokenArray arr = { 0 };
    int i = 0;

    while (src[i]) {
        char c = src[i];

        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            i++;
            continue;
        }

        if (c == '\'') {
            while (src[i] && src[i] != '\n') i++;
            continue;
        }

        if (isalpha((unsigned char)c) || c == '_') {
            int start = i;
            while (isalnum((unsigned char)src[i]) || src[i] == '_') i++;
            char* txt = substr(src, start, i - start);

            Token t = { 0 };
            t.text = txt;

            if (ieq(txt, "If")) t.type = TOK_IF;
            else if (ieq(txt, "Then")) t.type = TOK_THEN;
            else if (ieq(txt, "Else")) t.type = TOK_ELSE;
            else if (ieq(txt, "End")) t.type = TOK_END;
            else t.type = TOK_IDENT;

            push_token(&arr, t);
            continue;
        }

        if (isdigit((unsigned char)c)) {
            int start = i;
            while (isdigit((unsigned char)src[i])) i++;
            char* txt = substr(src, start, i - start);

            Token t = { 0 };
            t.type = TOK_NUMBER;
            t.text = txt;
            t.number = atoll(txt);
            push_token(&arr, t);
            continue;
        }

        if (c == '"') {
            i++;
            int start = i;
            while (src[i] && src[i] != '"') i++;
            char* txt = substr(src, start, i - start);

            Token t = { 0 };
            t.type = TOK_STRING;
            t.text = txt;
            push_token(&arr, t);

            if (src[i] == '"') i++;
            continue;
        }

        if (c == '<' && src[i + 1] == '>') {
            Token t = { 0 };
            t.type = TOK_NEQ;
            t.text = substr(src, i, 2);
            push_token(&arr, t);
            i += 2;
            continue;
        }

        {
            Token t = { 0 };
            t.text = substr(src, i, 1);

            switch (c) {
            case '=': t.type = TOK_ASSIGN; break;
            case '.': t.type = TOK_DOT; break;
            case ',': t.type = TOK_COMMA; break;
            case '(': t.type = TOK_LPAREN; break;
            case ')': t.type = TOK_RPAREN; break;
            default:
                printf("Unknown char: %c\n", c);
                exit(1);
            }

            push_token(&arr, t);
            i++;
        }
    }

    Token eof = { 0 };
    eof.type = TOK_EOF;
    eof.text = substr("", 0, 0);
    push_token(&arr, eof);

    return arr;
}

void free_tokens(TokenArray* arr) {
    for (int i = 0; i < arr->count; i++) {
        free(arr->items[i].text);
    }
    free(arr->items);
    arr->items = NULL;
    arr->count = 0;
    arr->capacity = 0;
}