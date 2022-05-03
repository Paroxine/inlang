#ifndef PARSE_H
#define PARSE_H
#include "lexer.h"


enum p_type {BLOCK,NUMBER,STRING,BOOLEEN,IDENTIFIER,FUNCTION, CALL,ASSIGN, BINARY };

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

void push_item(list* liste, void* item){
    if (!liste->items) liste->items = calloc(1,liste->item_size);
    else liste->items = realloc(liste->items,liste->item_size*(liste->size+1));
    liste->items[liste->size]=item;
    liste->size += 1;
}

typedef struct AST_T{
    enum p_type type;
    char* name;
    char* value;

    //block
    list* body;
    list* args;
    
    // if then else statement
    AST_T* cond; 
    AST_T* then;
    AST_T* elsee;

    // Binary 
    AST_T* left; 
    AST_T* right;

} AST_T;

AST_T* init_ast(int type);

AST_T* parse_assign();

AST_T* parse_compound();
#endif
