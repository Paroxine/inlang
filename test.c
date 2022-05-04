#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// var PRECEDENCE = {
//     "=": 1,
//     "||": 2,
//     "&&": 3,
//     "<": 7, ">": 7, "<=": 7, ">=": 7, "==": 7, "!=": 7,
//     "+": 10, "-": 10,
//     "*": 20, "/": 20, "%": 20,
// };

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

void push_item(void* item, list* liste){
    if (!liste->items) liste->items = calloc(1,liste->item_size);
    else liste->items = realloc(liste->items,liste->item_size*(liste->size+1));
    liste->items[liste->size]=item;
    liste->size += 1;
}
int main(){
   list* liste = init_list(sizeof(char*));
   push_item("chien1",liste);
   push_item("chien2",liste);
   push_item("chien3",liste);
   push_item("chien4",liste);
   push_item("chien5",liste);
   printf("%s", (char *) liste->items[0]);
}