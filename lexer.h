#ifndef LEXER_H
#define LEXER_H
#include <stdbool.h>
#include <ctype.h>

enum breaker { END = 0, SPACE = 32, EXCLA = 33, QUOTE = 34 , LPAR = 40, RPAR = 41, COMMA = 44,  POINT = 46 ,SEMIC= 59,LESS =60, EQUAL=61 ,GREATER=62,
LBRA = 91, RBRA = 93 , LCURL=123, RCURL = 125 , PERC = 37 , PROD = 42 ,PLUS = 43 ,MINUS = 45 ,DIV = 47 ,HTAG = 35, LINE = 10};

enum type { PUNC, INT, STR , KW, OP, ID};

typedef struct token{
    enum type type;
    char* value;
} token;


typedef struct token_stack {
    struct token* token;
    struct token_stack* next;
}token_stack;

char* char_to_string(char c);

struct token* new_token(enum type, char* value);

struct token_stack* new_token_stack();

void add_token(token* token, struct token_stack* stack);

void show_token(struct token* token);

void show_token_stack(struct token_stack* token_stack);

//Fonction pour le type

bool is_Punc (char c);

bool is_Op (char c);

char* read_String(char* texte ,int* curseur);

bool read_Identifier(int* curseur);

bool is_Keyword(char* mot);

char* read_Float(char* texte ,int* curseur);

token_stack* lexer(char* filename, int* taille);

token** token_list(char* nom, int* taille);


#endif