#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define BUF_SIZE 256
#define IN 1
#define OUT 0

void truncsp (char buffer[]);
int numtokens (char buffer[]);
char **lexer (char buffer[], int size);

/* Returns an array of character pointers. */
char **lexer (char buffer[], int size)
{
  int i, tokens;
  char **lexed, *token;
  char sep[] = " \t\n\v\f\r";

  truncsp(buffer);
  tokens = numtokens(buffer);
  
  lexed = malloc((tokens + 1) * sizeof(char *));
  if (lexed == NULL) return NULL;
  
  token = strtok(buffer, sep);

  for (i = 0; token != NULL && i < tokens;) {
    lexed[i++] = token;
    token = strtok(NULL, sep);
  }

  lexed[i] = NULL;

  return lexed;
}

/* Remove repeated whitespaces, replace them with single spaces. */
void truncsp (char str[])
{
  int lastsp;
  char *src = str;
  char *dst = str;

  lastsp = 0;
  while (*src != '\0') {
    if (isspace(*src)) {
      if (lastsp == 0) {
	*dst++ = ' ';
	lastsp = 1;
      }	
    } else { 
      *dst++ = *src;
      lastsp = 0;
    }
    src++;
  }
  *dst = '\0';
}

/* Count the number of tokens, return that value. */
int numtokens (char buffer[])
{
  int i, tokens, state;

  state = OUT; /* If start is IN, then first token not counted. */
  for (i = tokens = 0; buffer[i] != '\0'; i++) {
    if (isspace(buffer[i])) state = OUT;
    else if (state == OUT && !isspace(buffer[i])) ++tokens;
  }

  return tokens;
}
