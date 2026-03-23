#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

char *builtins[] = {"exit", "cd", "export", "setenv" "unset"};

void export(char **args);
void echo(char **args);

/* If it is a builtin, handle the execution and return 0.  Otherwise,
   return 1. */
int ckbuiltins (char **args)
{
  if (strcmp(args[0], "exit") == 0) {
    exit(0);
    return 0;
  } else if (strcmp(args[0], "cd") == 0) {
    if (chdir(args[1]) != 0) {
      perror("cd");
    }
    return 0;
  } else if (strcmp(args[0], "export") == 0) {
    export(args);
    return 0;
  } else if (strcmp(args[0], "echo") == 0) {
    echo(args);
    return 0;
  }
  return 1;
}

void export(char **args)
{
  char *name = strtok(args[1], "=");
  char *value = strtok(NULL, "=");

  if (name && value) {
    setenv(name, value, 1);
  } else {
    fprintf(stderr, "Usage: export NAME=VALUE\n");
  }
}

void echo(char **args)
{
  char *var;
  if (*(args[1]) == '$') {
    var = strtok(args[1], "$");
    printf("%s\n", getenv(var));
  } else
    printf("%s\n", args[1]);
}
	   
