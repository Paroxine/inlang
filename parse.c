#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"
#include "parse.h"


bool eat(token* token1,token* token2){
    if (token1->type==token2->type){
        if(token1->value==token2->value){
            return true;
        }
        return false;
    }
    return false;        
};


AST_T* ast_compound(){

};

AST_T* ast_assign(int* curseur,token** liste_token){
    *curseur++;
    AST_T* tree = malloc(sizeof(AST_T*));
    tree->name = liste_token[*curseur]->value;
    
};

int main(){
    
}