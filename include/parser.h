#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

Stmt* parse_program(TokenArray* tokens);

#endif