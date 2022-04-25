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

void add(int* valeur){
    *valeur = *valeur +1;
}

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
        


AST_T* ast_assign(int* curseur,token** liste_token){
    add(curseur);
    AST_T* tree = malloc(sizeof(AST_T*));
    tree->name = eat(ID,"",liste_token[*curseur],curseur);
    eat(OP,"=",liste_token[*curseur],curseur);
    tree->value = eat(STR,"",liste_token[*curseur],curseur);
    eat(PUNC,";",liste_token[*curseur],curseur);
    return tree;
};

AST_T* parser(char* nom){
    token_list* liste_token = lexer(nom);
    int taille = liste_token->size;
    int curseur =0;
    while (curseur<taille)
    {
        ast_compound(&curseur,liste_token);
    }
    
    

}

int main(){
    parser("test1.txt");
    
}