#ifndef PARSE_H
#define PARSE_H
#include "lexer.h"


enum p_type {BLOCK,NUMBER,STRING,BOOLEEN,IDENTIFIER,FUNCTION, CALL,ASSIGN, BINARY };

typedef struct AST_T{
    enum p_type type;
    char* name;
    char* value;
    struct AST_T** children;
    

    
} AST_T;



AST_T* init_ast(int type);

AST_T* ast_assign();

AST_T* ast_compound();
#endif
