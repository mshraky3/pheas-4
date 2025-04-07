#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Token types
typedef enum {
    TOKEN_ID,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATION,
    TOKEN_KEYWORD,
    TOKEN_EOF
} TokenType;

// Keywords
typedef enum {
    KW_IF,
    KW_THEN,
    KW_WHILE,
    KW_DO
} KeywordType;

// Token structure
typedef struct {
    TokenType type;
    union {
        char* id;
        int number;
        char op;
        char punct;
        KeywordType keyword;
    } value;
    int line_number;
} Token;

// Function declarations
Token get_next_token(void);
void parse_start(void);
void parse_stmt(void);
void parse_expr(void);
void gen_code(const char* code);

#endif // GLOBAL_H 