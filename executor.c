#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "include/parser.h"

void execute (struct astnode *node);

void execute (struct astnode *node)
{
  int fds[2];
  pid_t pid;

  if (node->type == NODE_PIPE) {
    pipe(fds);
    if (fork() == 0) { /* Start child proc #1 */
      dup2(fds[1], STDOUT_FILENO); /* Set as output */
      close(fds[0]);
      close(fds[1]);
      execute(node->left);
      exit(0);
    }
    if (fork() == 0) { /* Start child proc #2 */
      dup2(fds[0], STDIN_FILENO); /* Set as input */
      close(fds[0]);
      close(fds[1]);
      execute(node->right);
      exit(0);
    }
    close(fds[0]);
    close(fds[1]);
    wait(NULL);
    wait(NULL);
  } else if (node->type == NODE_REDIR_RIGHT) {
    pipe(fds);
    if (fork() == 0) {
      dup2(fds[1], STDOUT_FILENO);
      close(fds[0]);
      close(fds[1]);
      execute(node->left);
      exit(0);
    }
    close(fds[0]);
    close(fds[1]);
    wait(NULL);
  } else if (node->type == NODE_SEPARATOR) {
    execute(node->left);
    execute(node->right);
  } else if (node->type == NODE_CMD) {
    pid = fork();
    if (pid < 0) {
      perror("fork");
    } else if (pid == 0) {
      execvp(node->args[0], node->args);
      perror("execvp");
    } else wait(NULL);
  }
}
