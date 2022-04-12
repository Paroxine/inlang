#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//enum breaker { END = 0, SPACE = 32, BRAO = 124, BRAC = 125, PARO = 40, PARC = 41, SEMI = 59, COMMA = 44, QUOTE = 34 };
enum breaker { END, SPACE, BRAO, BRAC, PARO, PARC, SEMI, COMMA, QUOTE };

struct token_stack {
    enum breaker breaker;
    char* token;
    struct token_stack* next;
};

struct token_stack* add_token(char* token, enum breaker breaker, struct token_stack* stack) {
    struct token_stack *new = malloc(sizeof(struct token_stack));
    char* tokencp = malloc(sizeof(token));
    strcpy(tokencp, token);
    new->next = stack;
    new->token = tokencp;
    new->breaker = breaker;
    return new;
}

void show_tokens(struct token_stack* tokens) {
    printf("%s :: %d\n", tokens->token, (int) tokens->breaker);
    if (tokens->token != NULL) show_tokens(tokens->next);
}

struct token_stack* lexer(char* in) {
    int ci = 0;
    struct token_stack* tokens = malloc(sizeof(struct token_stack));
    int len = strlen(in);
    char* buf = malloc(sizeof(char) * len);
    while (in[ci] != '\0') {
        if (in[ci] == 0 || in[ci] == 32 || in[ci] == 34 || in[ci] == 40 || in[ci] == in[41] || in[ci] == 44 || in[ci] == 59 || in[ci] == 124 || in[ci] == 125) {
            enum breaker breaker;
            switch (in[ci]) {
                case 0:
                    breaker = END;
                    break;
                case 32:
                    breaker = SPACE;
                    break;
                case 40:
                    breaker = PARO;
                    break;
                case 41:
                    breaker = PARC;
                    break;
                case 44:
                    breaker = COMMA;
                    break;
                case 59:
                    breaker = SEMI;
                    break;
                case 124:
                    breaker = BRAO;
                    break;
                case 125:
                    breaker = BRAC;
                    break;
            }
            tokens = add_token(buf, breaker, tokens);
            free(buf);
            buf = malloc(sizeof(char) * len);
        } else {
            strncat(buf,&in[ci],1);
        }
        ci++;
    }
    tokens = add_token(buf, END, tokens);
    free(buf);
    return tokens;
}

int main(int argc, char* argv[]) {
    if (argc < 2) return 0;

    struct token_stack* tokens = lexer(argv[1]);
    show_tokens(tokens);

    return 0;
}