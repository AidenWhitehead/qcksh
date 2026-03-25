#ifndef LEXER_H
#define LEXER_H

typedef enum {
  TOKEN_UNQUOTE,
  TOKEN_SINGLE_QUOTE,
  TOKEN_DOUBLE_QUOTE
} token_type_t;

typedef struct {
  char *value;
  token_type_t type;
} token_t;

char *strdup (const char *str);
token_t **lexer (char buffer[], int size);
void freelex (token_t **lexed);

#endif
