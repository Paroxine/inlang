#ifndef PARSE_H
#define PARSE_H
#include "lex.h"

char* int_to_char(int type){ //debug
    if (type==PUNC){return "PUNC";};
    if (type==KW ){return "KW";};
    if (type==OP ){return "OP";};
    if (type==BLOCK ){return "BLOCK";};
    if (type==INT ){return "INT";};
    if (type==STR ){return "STR";};
    if (type==BOOL ){return "BOOL";};
    if (type==ID ){return "ID";};
    if (type==FUNC ){return "FUNC";};
    if (type==CALL  ){return "CALL";};
    if (type==ASSIGN ){return "ASSIGN";};
    if (type==BINARY ){return "BINARY";};
    if (type==IF ){return "IF";};
    if (type==WHILE){return "WHILE";};
};

typedef struct list{
    void** items;
    size_t size;
    size_t item_size;
}list;


list* init_list(size_t item_size){
    list* liste = calloc(1,sizeof(struct list));
    liste->items = 0 ;
    liste->size = 0;
    liste->item_size = item_size;
    return liste;
}


// for (int i = 0; i < liste->size; i++)
// {
//     AST_T* ast = liste[i];
//     printf("%s",int_to_char(ast->type));

// }

void push_item(list* liste, void* item){
    if (!liste->items) liste->items = calloc(1,liste->item_size);
    else liste->items = realloc(liste->items,liste->item_size*(liste->size+1));
    liste->items[liste->size]=item;
    liste->size += 1;
}

typedef struct AST_T{
    enum type type;
    char* name;
    char* value;

    //assign
    struct AST_T* assign_value;

    //block
    list* body;

    //function
    list* args;

     // if then else statement
    list* body_if;
    list* body_else;
    struct AST_T* cond; 
    
    //while statement 
    list* body_while;
    // Binary 
    char* operator;
    struct AST_T* left; 
    struct AST_T* right;

} AST_T;

bool can_add(token_list* liste){
    return (liste->curseur!=liste->size);
}

char* eat(int type,char* value,token_list* liste);

AST_T* ast_init();

AST_T* parse_all(token_list* liste);

AST_T* parse_call(token_list* liste);

AST_T* parse_function(token_list* liste);

AST_T* parse_bool(token_list* liste);

AST_T* parse_block(token_list* liste);

AST_T* parse_atom(token_list* liste);

AST_T* parse_expression(token_list* liste);

AST_T* parse_parentheses(token_list* liste);

AST_T* parse_while(token_list* liste);

AST_T* maybe_binary(AST_T* left, int old_precedence,token_list* liste);

#endif
