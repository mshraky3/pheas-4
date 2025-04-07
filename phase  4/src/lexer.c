#include "../include/global.h"

static int current_line = 1;
static char input_buffer[1024];
static int buffer_pos = 0;
static int buffer_size = 0;

// Get next character from input
static char get_next_char(void) {
    if (buffer_pos >= buffer_size) {
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
            return EOF;
        }
        buffer_size = strlen(input_buffer);
        buffer_pos = 0;
        current_line++;
    }
    return input_buffer[buffer_pos++];
}

// Put back a character
static void unget_char(void) {
    if (buffer_pos > 0) {
        buffer_pos--;
    }
}

Token get_next_token(void) {
    Token token;
    char ch;
    
    // Skip whitespace
    while ((ch = get_next_char()) != EOF && isspace(ch));

    if (ch == EOF || ch == '\n') {
        token.type = TOKEN_EOF;
        return token;
    }

    // Handle identifiers and keywords
    if (isalpha(ch)) {
        char buffer[256];
        int i = 0;
        buffer[i++] = ch;
        
        while ((ch = get_next_char()) != EOF && (isalnum(ch) || ch == '_')) {
            buffer[i++] = ch;
        }
        unget_char();
        buffer[i] = '\0';
        
        // Check for keywords
        if (strcmp(buffer, "if") == 0) {
            token.type = TOKEN_KEYWORD;
            token.value.keyword = KW_IF;
        } else if (strcmp(buffer, "then") == 0) {
            token.type = TOKEN_KEYWORD;
            token.value.keyword = KW_THEN;
        } else if (strcmp(buffer, "while") == 0) {
            token.type = TOKEN_KEYWORD;
            token.value.keyword = KW_WHILE;
        } else if (strcmp(buffer, "do") == 0) {
            token.type = TOKEN_KEYWORD;
            token.value.keyword = KW_DO;
        } else {
            token.type = TOKEN_ID;
            token.value.id = strdup(buffer);
        }
    }
    // Handle numbers
    else if (isdigit(ch)) {
        int num = 0;
        do {
            num = num * 10 + (ch - '0');
            ch = get_next_char();
        } while (isdigit(ch));
        unget_char();
        
        token.type = TOKEN_NUMBER;
        token.value.number = num;
    }
    // Handle operators
    else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%') {
        token.type = TOKEN_OPERATOR;
        token.value.op = ch;
    }
    // Handle punctuation
    else if (ch == ';' || ch == '=' || ch == '(' || ch == ')') {
        token.type = TOKEN_PUNCTUATION;
        token.value.punct = ch;
    }
    else {
        token.type = TOKEN_EOF;
        printf("Error: Unknown character at line %d\n", current_line);
    }

    token.line_number = current_line;
    return token;
} 