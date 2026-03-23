#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "lexer.h"
#include "builtin.h"

#define BUF_SIZE 256

int main (void)
{
  char buffer[BUF_SIZE];
  char **args;
  pid_t pid;

  while (1) {
    printf("$ ");
    
    fgets(buffer, sizeof(buffer), stdin);
    args = lexer(buffer, sizeof(buffer));

    if (args == NULL || args[0] == NULL)
      continue;

    if (ckbuiltins(args) != 0) {
      pid = fork();
      if (pid < 0) {
	perror("fork");
	return 1;
      } else if (pid == 0) {
	execvp(args[0], args);
	perror("execvp");
	return 1;
      } else {
	wait(NULL);
      }
    }

    free(args);
  }
  
  return 0;
}
