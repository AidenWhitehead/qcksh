#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "include/lexer.h"

#define BUF_SIZE 256
#define IN 1
#define OUT 0

void truncsp (char buffer[]);
int numtokens (char buffer[]);

/* Returns a lexed form of buffer as an array of character pointers.
   Uses all space characters as separator characters.  Leaves quoted
   text untouched. */
token_t **lexer (char buffer[], int size)
{
  int i, tokens, len;
  char *current, *start, *end;
  token_t **lexed;

  tokens = numtokens(buffer);
  
  lexed = malloc((tokens + 1) * sizeof(token_t *));
  if (lexed == NULL) return NULL;

  i = 0;
  current = buffer;
  while (*current != '\0') {
    /* Strip the leading whitespace, and break if the entire command is
       just whitespace. */
    while (*current != '\0' && isspace(*current))
      current++;
    if (*current == '\0') break;

    start = current;
    if (*current == '\'') {
      start++;
      current++;
      while (*current != '\0' && *current != '\'')
	current++;
      end = current;
      if (*current == '\'') current++; /* Don't get stuck on closing. */
      if (*current == '\0') {
	fprintf(stderr, "Unmatched \'!");
	break;
      }
    } else {
      while (*current && !isspace(*current))
	current++;
      end = current;
    }
    lexed[i] = malloc(sizeof(token_t));
    if (lexed[i] == NULL) return NULL;
    len = end - start;
    lexed[i]->value = malloc(len + 1);
    memcpy(lexed[i]->value, start, len);
    lexed[i]->value[len] = '\0';
    i++;
  }

  lexed[i] = NULL; /* Insert NULL at the end of args */

  return lexed;
}

int numtokens (char buffer[])
{
  int count = 0;
  char *c = buffer;
  while (*c) {
    while (*c && isspace(*c)) c++;
    if (*c == '\0') break;
    count++; 
    if (*c == '\'') {
      c++; 
      while (*c && *c != '\'') c++; 
      if (*c == '\'') c++; 
    } else {
      while (*c && !isspace(*c)) c++;
    }
  }
  return count;
}

char *strdup (const char *str)
{
  char *dup = malloc(strlen(str) + 1);
  if (dup == NULL) return NULL;
  strcpy(dup, str);
  return dup;
}

void freelex (token_t **lexed)
{
  int i;
  
  for (i = 0; lexed[i] != NULL; i++) {
    free(lexed[i]->value);
    free(lexed[i]);
  }
  free(lexed);
}
