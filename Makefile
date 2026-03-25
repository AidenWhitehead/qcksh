all: qcksh
qcksh: main.c parser.c builtin.c lexer.c parser.c executor.c include/builtin.h include/lexer.h include/parser.h include/executor.h
	gcc -ansi -pedantic -Wall -o qcksh main.c parser.c builtin.c lexer.c executor.c
