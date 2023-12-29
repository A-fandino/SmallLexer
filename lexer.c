#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    INT,
    IDENTIFIER,
    PUSH,
    POP,
} token_type;

const char* token_names[] = {
    "INT",
    "IDENTIFIER",
    "PUSH",
    "POP",
};

typedef struct {
    token_type type;
    char* value;
} Token;

typedef struct {
    Token* array;
    size_t size;
    size_t used;
} TokenArray;

int is_white_space(char c) {
    return c == ' ' || c == '\n' || c == '\t' || c == '\r' || c==EOF;
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}


void initArray(TokenArray *a, size_t size) {
    a->array = malloc(size * sizeof(Token));
    a->size = size;
    a->used = 0;
}

void pushArray(TokenArray *a, Token element) {
    if (a->used == a->size) {
        a->size *= 2;
        a->array = (Token*)realloc(a->array, a->size * sizeof(Token));
    }
    a->array[a->used++] = element;
}

token_type get_keyword_type(char* token) {
    if (strcmp(token, "push") == 0) return PUSH;
    if (strcmp(token, "pop") == 0) return POP;
    if (is_digit(token[0])) return INT;
    return IDENTIFIER;
}


Token generateToken(char* token) {
    Token t;
    t.type = get_keyword_type(token);
    t.value = malloc(strlen(token) + 1);
    strcpy(t.value, token);
    return t;
}



TokenArray* parse_tokens(FILE* f) {
    TokenArray* tokens = malloc(sizeof(TokenArray));
    initArray(tokens, 8);
    size_t buffer_size = 128;
    char *token;
    token = malloc(buffer_size * sizeof(char));
    int token_length = 0;
    char c;
    while ((c = fgetc(f))) {
        if (is_white_space(c)) {
            if (token_length > 0) {
                token[token_length] = '\0';
                pushArray(tokens, generateToken(token));
                token = malloc(buffer_size * sizeof(char));
                token_length = 0;
            }
            if (c == EOF) break;
            continue;
        }
        if (is_digit(c)) {
            do {
            token[token_length++] = c;
            } while(c = fgetc(f), is_digit(c));
            if (!is_white_space(c)) {
                printf("Invalid character after integer %c\n", c);
                exit(1);
            }
            continue;
        }
      token[token_length++] = c;
    }
    if (ferror(f)) {
        printf("Error reading file\n");
        exit(1);
    }
    return tokens;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: lexer <filename>\n");
        return 1;
    }
    char* filename = argv[1];
    FILE* f = fopen(filename, "r");
    TokenArray* tokens = parse_tokens(f);
    fclose(f);
    for (size_t i = 0; i < tokens->used; i++) {
        Token t = tokens->array[i];
        printf("Token: %s -> %s\n", t.value, token_names[t.type]);
    }
    // Free memory
    for (size_t i = 0; i < tokens->used; i++) {
        free(tokens->array[i].value);
    }

    free(tokens->array);
    free(tokens);
    return 0;
}