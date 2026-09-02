#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOK_EOF,
    TOK_IDENT,
    TOK_NUMBER,
    TOK_STRING,

    TOK_ASSIGN,     // =
    TOK_NEQ,        // <>
    TOK_DOT,        // .
    TOK_COMMA,      // ,
    TOK_LPAREN,     // (
    TOK_RPAREN,     // )

    TOK_IF,
    TOK_THEN,
    TOK_ELSE,
    TOK_END
} TokenType;

typedef struct {
    TokenType type;
    char* text;
    long long number;
} Token;

typedef struct {
    Token* items;
    int count;
    int capacity;
} TokenArray;

TokenArray lex_source(const char* src);
void free_tokens(TokenArray* arr);

#endif