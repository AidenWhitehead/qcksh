#define _POSIX_C_SOURCE 200112L /* Don't know why I need this. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  char *name;
  int (*func)(char **args);
} builtin_t;

int sh_exit (char **args);
int sh_cd (char **args);
int sh_export(char **args);
int sh_echo(char **args);
int ckbuiltins (char **args);

builtin_t builtins[] = {
  {"exit", &sh_exit},
  {"cd", &sh_cd},
  {"export", &sh_export},
  {NULL, NULL}};

/* If it is a builtin, handle the execution and return 0.  Otherwise,
   return 1. */
int builtin (char **args)
{
  int (*ptr)(char **);
  int i;

  for (i = 0; builtins[i].name != NULL; i++) {
    if (strcmp(args[0], builtins[i].name) == 0) {
      ptr = builtins[i].func;
      return ptr(args); /* I think this is ugly, but whatever. */
    }
  }
  return 1;
}

/* Probably a more terse way to implement this one. */
int sh_exit (char **args)
{
  exit(0);
  return 0; 
}

int sh_cd (char **args)
{
  if (chdir(args[1]) != 0)
    perror("cd");
  return 0;
}

int sh_export(char **args)
{
  /* Separate the two sides of the equivalence, i.e., PATH=/bin/ ->
     name="PATH" and value="/bin/". */
  char *name = strtok(args[1], "=");
  char *value = strtok(NULL, "="); 

  if (name && value) {
    setenv(name, value, 1);
  } else {
    fprintf(stderr, "Usage: export NAME=VALUE\n");
  }  
  return 0;
}
