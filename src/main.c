#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "env.h"
#include <stdio.h>
#include <stdlib.h>

static char* read_file(const char* path) {
    FILE* fp = fopen(path, "rb");
    if (!fp) {
        printf("Cannot open file: %s\n", path);
        exit(1);
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char* buf = (char*)malloc(size + 1);
    fread(buf, 1, size, fp);
    buf[size] = '\0';

    fclose(fp);
    return buf;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("usage: autoscript <scriptfile>\n");
        return 1;
    }

    char* source = read_file(argv[1]);

    TokenArray tokens = lex_source(source);
    Stmt* program = parse_program(&tokens);

    Env env;
    env_init(&env);

    interpret(program, &env);

    env_free(&env);
    free_stmt_list(program);
    free_tokens(&tokens);
    free(source);

    return 0;
}