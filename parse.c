#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lex.h"
#include "parse.h"

bool can_add(token_list* liste){
    return (liste->curseur!=liste->size);
}

char* eat(int type,char* value,token_list* liste){
    if (liste->curseur<liste->size){
        token* token = liste->items[liste->curseur];
        if (token->type!=type){
            printf("Wrong token type. Expected %s; received %s\n",int_to_char(type),int_to_char(token->type));
            printf("Wrong token value. Expected %s; received %s\n",value,token->value);

            exit(0);
        }
        else{
            if(token->type==PUNC||token->type==KW||strcmp(value,"=")==0){
                if (strcmp(value,token->value)!=0){
                    printf("Wrong token value. Expected %s; received %s",value,token->value);
                    exit(1);
                }
            }
            if (can_add(liste)) liste->curseur+= 1 ;
            return token->value;

        }
    }
    printf("Exceed token list size");
    exit(2);
};

AST_T* ast_init(){
    AST_T* ast = malloc(sizeof(struct AST_T));
    ast->type = NOOP;
    ast->name = "init";
    ast->value = "init";

    // Binary 
    ast->operator = "noop";
    ast->left= (void*) 0 ; 
    ast->right = (void*) 0 ;

    return ast;
}

bool compare(char* mot1, char* mot2){
    return (strcmp(mot1,mot2)==0);
}

AST_T* token_to_AST(token* token){
    AST_T* ast = malloc(sizeof(struct AST_T));
    ast->type = token->type;
    ast->value = token->value;
    return ast;
}

AST_T* parse_function(token_list* liste){
    AST_T* ast = malloc(sizeof(struct AST_T));
    eat(KW, "function", liste);
    ast->name = eat(ID,"",liste);
    eat(PUNC,"(",liste);
    ast->type=FUNC;
    ast->args = init_list(sizeof(struct AST_T));
    if (liste->items[liste->curseur]->value[0]==RPAR){
        eat(PUNC,")",liste);
        return ast;
    }
    push_item(ast->args, parse_expression(liste));
    while (!compare(")",liste->items[liste->curseur]->value) && can_add(liste)){
        eat(PUNC,",",liste);
        push_item(ast->args, parse_expression(liste));
    }
    eat(PUNC,")",liste);
    return ast;                     
}

AST_T* parse_if(token_list* liste){
    eat(KW,"if",liste);
    eat(PUNC,"(",liste);
    AST_T* ast = malloc(sizeof(struct AST_T));
    ast->type = IF ;
    ast->cond = parse_expression(liste);
    eat(PUNC,")",liste);
    eat(PUNC,"{",liste);
    ast->body_if = init_list(sizeof(struct AST_T));
    while (!compare("}",liste->items[liste->curseur]->value) && can_add(liste)){
        push_item(ast->body_if, parse_expression(liste));
        eat(PUNC,";",liste);
    }
    eat(PUNC,"}",liste);
    return ast;
}

AST_T* parse_bool(token_list* liste){
} 

AST_T* parse_block(token_list* liste){
}

AST_T* parse_atom(token_list* liste){
    
    if (can_add(liste)){
        // easiest case to parse from token to AST type 

        token* token = liste->items[liste->curseur];
        if (token->type == STR || token->type == ID || token->type == INT){
            liste->curseur+=1;
            return token_to_AST(token);
        }

        // Parsing of function and if statement
        if (token->type==KW){
            char* value = token->value;
            if (compare(value,"function")){
                return parse_function(liste); 
            }

            else if(compare(value, "if")){
                return parse_if(liste);
            }
            
            else if(compare(value,"if")) return parse_if(liste);
            else if (compare(value, "true")||compare(value, "false")) return parse_bool(liste);
        }
        if(token->type==OP){
            if (token->value[0]==LPAR) return parse_block(liste);
        }
    }
    AST_T* ast = ast_init();
    return ast ;
}

AST_T* maybe_binary(AST_T* left, int old_precedence,token_list* liste){
    if (can_add(liste)){
        token* token = liste->items[liste->curseur];
        if (token->type == OP){
            if (token->precedence>=old_precedence){
                liste->curseur+=1;
                AST_T* binaire = malloc(sizeof(struct AST_T));
                binaire->operator = token->value;
                AST_T* right =  maybe_binary(parse_atom(liste),token->precedence, liste);
                binaire->left = left; 
                binaire->right = right;
                binaire->type = BINARY;
                return maybe_binary(binaire, old_precedence,liste);
            }
           
        }
    }
    return left;;
};

AST_T* parse_expression(token_list* liste){
    maybe_binary(parse_atom(liste),0,liste);
};

int main(){
    token_list*  liste = lexer("test_func.txt");
    AST_T* ast = parse_if(liste);
    AST_T* fligne = ast->body->items[0];
    printf("%s",fligne->operator);
     
}