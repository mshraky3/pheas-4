#include "../include/global.h"

static Token current_token;

// Helper function to match expected token
static void match(TokenType expected_type) {
    if (current_token.type == expected_type) {
        current_token = get_next_token();
    } else {
        printf("Error: Unexpected token at line %d\n", current_token.line_number);
    }
}

// Helper function to match specific punctuation
static void match_punct(char expected_punct) {
    if (current_token.type == TOKEN_PUNCTUATION && current_token.value.punct == expected_punct) {
        current_token = get_next_token();
    } else {
        printf("Error: Expected '%c' at line %d\n", expected_punct, current_token.line_number);
    }
}

static void match_keyword(KeywordType expected_keyword) {
    if (current_token.type == TOKEN_KEYWORD && current_token.value.keyword == expected_keyword) {
        current_token = get_next_token();
    } else {
        printf("Error: Expected keyword at line %d\n", current_token.line_number);
    }
}

// Parse expression
void parse_expr(void) {
    if (current_token.type == TOKEN_NUMBER) {
        printf("PUSH %d\n", current_token.value.number);
        match(TOKEN_NUMBER);
    } else if (current_token.type == TOKEN_ID) {
        printf("PUSH %s\n", current_token.value.id);
        match(TOKEN_ID);
    } else if (current_token.type == TOKEN_PUNCTUATION && current_token.value.punct == '(') {
        match_punct('(');
        parse_expr();
        match_punct(')');
    }

    if (current_token.type == TOKEN_OPERATOR) {
        char op = current_token.value.op;
        match(TOKEN_OPERATOR);
        parse_expr();
        
        switch (op) {
            case '+': printf("ADD\n"); break;
            case '-': printf("SUB\n"); break;
            case '*': printf("MULT\n"); break;
            case '/': printf("DIV\n"); break;
            case '%': printf("MOD\n"); break;
        }
    }
}

// Parse statement
void parse_stmt(void) {
    if (current_token.type == TOKEN_ID) {
        char* id = strdup(current_token.value.id);
        match(TOKEN_ID);
        match_punct('=');
        parse_expr();
        printf("POP %s\n", id);
        free(id);
    }
    else if (current_token.type == TOKEN_KEYWORD) {
        switch (current_token.value.keyword) {
            case KW_IF: {
                match_keyword(KW_IF);
                match_punct('(');
                parse_expr();
                printf("CMP\n");
                printf("BE else\n");
                match_punct(')');
                match_keyword(KW_THEN);
                parse_stmt();
                printf("LABEL else\n");
                break;
            }
            case KW_WHILE: {
                printf("LABEL loop\n");
                match_keyword(KW_WHILE);
                match_punct('(');
                parse_expr();
                printf("CMP\n");
                printf("BE end\n");
                match_punct(')');
                match_keyword(KW_DO);
                parse_stmt();
                printf("B loop\n");
                printf("LABEL end\n");
                break;
            }
            default:
                printf("Error: Unexpected keyword at line %d\n", current_token.line_number);
                break;
        }
    }
}

// Start parsing
void parse_start(void) {
    current_token = get_next_token();
    while (current_token.type != TOKEN_EOF) {
        parse_stmt();
        if (current_token.type == TOKEN_PUNCTUATION && current_token.value.punct == ';') {
            match_punct(';');
        }
    }
} 