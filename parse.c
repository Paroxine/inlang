#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"
#include "parse.h"

char* int_to_char(int type){
    if (type==PUNC){return "PUNC";};
    if (type==OP){return "OP";};
    if (type==STR){return "STR";};
    if (type==KW){return "KW";};
    if (type==ID){return "ID";};
    if (type==INT){return "INT";};
};

char* eat(int type,char* value,token* token,int* curseur){
    if (token->type!=type){
        printf("Wrong token type. Expected %s, received %s\n",int_to_char(type),int_to_char(token->type));
        printf("Wrong token value. Expected %s, received %s\n",value,token->value);

        exit(0);
    }
    else{
        if(token->type==PUNC||token->type==KW||strcmp(value,"=")==0){
            if (strcmp(value,token->value)!=0){
                printf("Wrong token value. Expected %s, received %s",value,token->value);
                exit(1);
            }
        }
        add(curseur) ;
        return token->value;

    }
};

AST_T* ast_init(int type){
    AST_T* ast = malloc(sizeof(struct AST_T));
    ast->type = type;

    if (type==BLOCK){
        ast->body = init_list(sizeof(struct AST_T));
    }
}
    
AST_T* token_to_AST(token* token){
    AST_T* ast = init_ast(token->type);
    ast->value = token->value;
    return ast;
}

AST_T* parse_expression(int* curseur, token* token_list){

}

AST_T* parse_if(int* curseur, token* token_list){

}

AST_T* parse_all(char* nom){
    token_list* liste_token = lexer(nom);
    int taille = liste_token->size;
    int curseur =0;
    while (curseur<taille)
    {
        list* prog = init_list(sizeof(struct AST_T));
        parse_expression(&curseur,liste_token);
    }
    
    

}

int main(){
    parser("test1.txt");
    
}