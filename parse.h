#ifndef PARSE_H
#define PARSE_H
#include "lexer.h"



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
    enum type type;
    char* name;
    char* value;

    //block
    list* body;

    //function
    list* args;
    
    // if then else statement
    struct AST_T* cond; 
    struct AST_T* then;
    struct AST_T* elsee;

    // Binary 
    char* operator;
    struct AST_T* left; 
    struct AST_T* right;

} AST_T;

AST_T* init_ast();

AST_T* parse_block(token_list* liste);
#endif
