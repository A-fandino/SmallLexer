CC = gcc
CFLAGS = -Wall -Wextra

lexer: lexer.c
	$(CC) $(CFLAGS) -o lexer.o lexer.c

.PHONY: clean
clean:
	rm -f lexer.o
