/* TODO: This is terrible and I am bad with ASTs. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "include/parser.h"
#include "include/lexer.h"

struct astnode *newnode (nodetype type);
void expandenvs (token_t **args);
void cutquotes (token_t **args);

/* Parses the tokens in lexed args and returns a pointer to the root of
   the AST, to be sent to the executor. Returns null if there was an
   error. */
struct astnode *parse (token_t **args)
{
  int i, j;
  char **commands;
  struct astnode *node = NULL; /* Init an empty root node for AST. */

  expandenvs(args);

  for (i = 0; args[i] != NULL; i++) {
    if (strcmp(args[i]->value, "|") == 0) {
      node = newnode(NODE_PIPE);
      args[i] = NULL;
      node->left = parse(args);
      node->right = parse(&args[i + 1]);
      return node;
    } else if (strcmp(args[i]->value, ">") == 0) {
      node = newnode(NODE_REDIR_RIGHT);
      args[i] = NULL;
      node->left = parse(args);
      node->right = parse(&args[i + 1]);
      return node;
    } else if (strcmp(args[i]->value, "<") == 0) {
      node = newnode(NODE_REDIR_LEFT);
      args[i] = NULL;
      node->left = parse(args);
      node->right = parse(&args[i + 1]);
      return node;
    } else if (strcmp(args[i]->value, ";") == 0) {
      node = newnode(NODE_SEPARATOR);
      args[i] = NULL;
      node->left = parse(args);
      node->right = parse(&args[i + 1]);
      return node;
    }
  }
  /* This all feels very poorly designed and I don't really like it,
     but it's all I can think of. */
  for (i = 0; args[i] != NULL; i++);
  commands = malloc((i + 1) * sizeof(char *));
  for (j = 0; j < i; j++) {
    commands[j] = strdup(args[j]->value);
  }
  commands[i] = NULL;
  node = newnode(NODE_CMD);
  node->args = commands;
  return node;
}

struct astnode *newnode (nodetype type)
{
  struct astnode *node = malloc(sizeof(struct astnode));
  node->type = type;
  node->value = NULL;
  node->left = node->right = NULL;
  node->args = NULL;
  return node;
}

/* Free given node and its children. */
void freeast (struct astnode *node)
{
  if (node == NULL) return;
  freeast(node->left);
  freeast(node->right); /* free its children */
  free(node);
}

/* Expand any env variables in args to their values. */
void expandenvs (token_t **args)
{
  char *name, *value, *buffer;
  int i;

  for (i = 0; args[i] != NULL; i++) {
    if (*(args[i]->value) == '$') { /* If first character of args[i] is '$' */
      name = args[i]->value + 1; /* Cut off the '$'. */
      value = getenv(name);
      
      if (value == NULL) continue; /* check that we got something */
      
      buffer = malloc(strlen(value) + 1); /* REMEMBER THE END NULL!!! */
      if (buffer != NULL) {
	strcpy(buffer, value);
	args[i]->value = buffer;
      } else {
	fprintf(stderr, "Memory allocation failed!\n");
	return;
      }
    }
  }
}
