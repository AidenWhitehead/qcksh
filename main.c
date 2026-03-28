#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/executor.h"

#define BUF_SIZE 256

void print_ast(struct astnode *node, int level);

int main (void)
{
  char buffer[BUF_SIZE];
  token_t **args;
  struct astnode *root;

  while (1) {
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    
    if (getuid() == 0)
      printf("# ");
    else printf("$ ");
    
    fgets(buffer, sizeof(buffer), stdin);
    args = lexer(buffer, sizeof(buffer));

    if (args == NULL || args[0] == NULL) continue;

    root = parse(args);

    execute(root);

    freelex(args);
    freeast(root);
  }
  
  return 0;
}

void print_ast(struct astnode *node, int level) {
  int i;
  if (node == NULL) return;
  
  for (i = 0; i < level; i++) printf("  ");

  switch (node->type) {
  case NODE_PIPE:
    printf("PIPE (|)\n");
    break;
  case NODE_REDIR_RIGHT:
    printf("REDIR (>)\n");
    break;
  case NODE_REDIR_LEFT:
    printf("REDIR (<)\n");
    break;
  case NODE_SEPARATOR:
    printf("SEPARATOR (;)\n");
    break;
  case NODE_CMD:
    printf("CMD: ");
    if (node->args) {
      for (i = 0; node->args[i] != NULL; i++) {
	printf("%s ", node->args[i]);
      }
    }
    printf("\n");
    break;
  }

    print_ast(node->left, level + 1);
    print_ast(node->right, level + 1);
}
