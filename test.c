#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum type { PUNC, INT, STR , KW, OP, ID};

typedef struct token{
    enum type type;
    char* value;
} token;

struct token* new_token(enum type type, char* value){
    struct token* token = malloc(sizeof(struct token));
    token->type = type;
    token->value = value;
    return token;

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

void push_item(list* liste, void* item){
    if (!liste->items) liste->items = calloc(1,liste->item_size);
    else liste->items = realloc(liste->items,liste->item_size*(liste->size+1));
    liste->items[liste->size]=item;
    liste->size += 1;
}

int main(){
    list* liste = init_list(sizeof(token*));
    push_item(liste ,new_token(PUNC,"."));
    push_item(liste ,new_token(OP,"="));
    token* token = (struct token*) liste->items[1];
    printf("%s",token->value);

}