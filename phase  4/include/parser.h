#ifndef PARSER_H
#define PARSER_H

#include "global.h"

// Parser helper functions
void match(TokenType expected_type);
void match_punct(char expected_punct);
void match_keyword(KeywordType expected_keyword);

// Parser functions
void parse_start(void);
void parse_stmt(void);
void parse_expr(void);
void parse_cs(void);

#endif // PARSER_H 