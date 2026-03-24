/* TODO: This is terrible and I am bad with ASTs. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef enum {
  NODE_CMD, /* Just a normal command. */
  NODE_VAR, /* Prefixed by '$' */
  NODE_PIPE, /* Infixed by '|' */
  NODE_REDIR /* Infixed by '>' */
} nodetype;

struct astnode {
  nodetype type;
  char *value;
  struct astnode *left; 
  struct astnode *right; /* For the sides of infix operators */
  char **args;
};

/* Parses the token and puts it in a form to be sent to the executor. */
int parse (char **args)
{
  char *name, *value, *buffer;
  int i;
  
  for (i = 0; args[i] != NULL; i++) {
    if (*(args[i]) == '$') { /* If first character of args[i] is '$' */
      name = args[i] + 1;
      value = getenv(name);

      if (value == NULL) continue;
      
      buffer = malloc(strlen(value) + 1);
      if (buffer != NULL) {
	strcpy(buffer, value);
	args[i] = buffer;
      } else {
	fprintf(stderr, "Memory allocation failed!\n");
	return 1;
      }
    }
  }
  return 0;
}
