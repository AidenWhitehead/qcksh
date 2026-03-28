#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include "include/parser.h"
#include "include/builtin.h"

void execute (struct astnode *node);

void execute (struct astnode *node)
{
  int fds[2], fd;
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
    fd = creat(node->right->args[0], 0644);
    
    if (fd < 0)
      perror("creat");
    
    if (fork() == 0) {
      dup2(fd, STDOUT_FILENO);
      close(fd);
      execute(node->left);
      exit(0);
    }

    close(fd);
    wait(NULL);
  } else if (node->type == NODE_REDIR_LEFT) {
    fd = creat(node->left->args[0], 0644);
    
    if (fd < 0)
      perror("creat");
    
    if (fork() == 0) {
      dup2(fd, STDOUT_FILENO);
      close(fd);
      execute(node->right);
      exit(0);
    }
    
    close(fd);
    wait(NULL);
  } else if (node->type == NODE_SEPARATOR) {
    execute(node->left);
    execute(node->right);
  } else if (node->type == NODE_CMD) {
    if (builtin(node->args) == 0)
      return;

    pid = fork();
    signal(SIGINT, SIG_IGN);
    
    if (pid < 0) {
      perror("fork");
    } else if (pid == 0) {
      execvp(node->args[0], node->args);
      perror("execvp");
    } else wait(NULL);
  }
}
