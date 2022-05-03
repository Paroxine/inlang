#ifndef LEXER_H
#define LEXER_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h> 


enum breaker { END = 0, SPACE = 32, EXCLA = 33, QUOTE = 34 , LPAR = 40, RPAR = 41, COMMA = 44,  POINT = 46 ,SEMIC= 59,LESS =60, EQUAL=61 ,GREATER=62,
LBRA = 91, RBRA = 93 , LCURL=123, RCURL = 125 , PERC = 37 , PROD = 42 ,PLUS = 43 ,MINUS = 45 ,DIV = 47 ,HTAG = 35, LINE = 10};

enum type {PUNC, KW, OP ,BLOCK, INT,STR,BOOLEEN,ID,FUNC, CALL, ASSIGN, BINARY };

typedef struct token{
    enum type type;
    char* value;
} token;

typedef struct token_list{
    token** items;
    size_t size;
}token_list;


char* char_to_string(char c);

token_list* init_token_list();

struct token* new_token(enum type, char* value);

struct token_stack* new_token_stack();

void add_token(token* token, token_list* stack);

void show_token(struct token* token);

void show_token_stack(struct token_stack* token_stack);

//Fonction pour le type

bool is_Punc (char c);

bool is_Op (char c);

char* read_String(char* texte ,int* curseur);

bool read_Identifier(int* curseur);

bool is_Keyword(char* mot);

char* read_Float(char* texte ,int* curseur);

token_list* lexer(char* filename);

#endif