#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"

typedef enum {
  NODE_CMD, /* Just a normal command. */
  NODE_PIPE, /* Infixed by '|' */
  NODE_REDIR_RIGHT, /* Infixed by '>' */
  NODE_REDIR_LEFT, /* Infixed by '<' */
  NODE_SEPARATOR /* Infixed by ';' */
} nodetype;

struct astnode {
  nodetype type;
  char *value;
  struct astnode *left; 
  struct astnode *right; /* For the sides of infix operators */
  char **args;
};

void freeast (struct astnode *node);
struct astnode *parse (token_t **args);

#endif
