all: qcksh
qcksh: main.c parser.c builtin.c lexer.c include/builtin.h include/lexer.h include/parser.h
	gcc -ansi -pedantic -Wall -o qcksh main.c parser.c builtin.c lexer.c
