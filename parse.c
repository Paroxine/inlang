#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parse.h"

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

bool compare(char* mot1, char* mot2){
    return (strcmp(mot1,mot2)==0);
}

AST_T* token_to_AST(token* token){
    AST_T* ast = malloc(sizeof(struct AST_T));
    ast->type = token->type;
    ast->value = token->value;
    return ast;
}
AST_T* parse_all(token_list* liste){
    AST_T* ast = malloc(sizeof(struct AST_T));
    ast->type = BLOCK;
    ast->body = init_list(sizeof(struct AST_T));
    while(can_add(liste)){
        push_item(ast->body, parse_expression(liste));
        eat(PUNC,";",liste);
    }
    return ast;
};

AST_T* parse_assign(token_list* liste){
    eat(KW,"let",liste);
    AST_T* ast = malloc(sizeof(struct AST_T));
    ast->type = ASSIGN;
    ast->name = eat(ID,"",liste);
    eat(OP,"=",liste);
    ast->assign_value = parse_expression(liste);

    return ast;
    
}

AST_T* parse_function(token_list* liste){
    AST_T* ast = malloc(sizeof(struct AST_T));
    eat(KW, "function", liste);
    ast->name = eat(ID,"",liste);
    eat(PUNC,"(",liste);
    ast->type=FUNC;
    ast->args = init_list(sizeof(struct AST_T));
    if (!(liste->items[liste->curseur]->value[0]==RPAR)) push_item(ast->args, parse_expression(liste));
    while (!compare(")",liste->items[liste->curseur]->value) && can_add(liste)){
        eat(PUNC,",",liste);
        push_item(ast->args, parse_expression(liste));
    }
    eat(PUNC,")",liste);
    eat(PUNC,"{",liste);
    ast->body = init_list(sizeof(struct AST_T));
    
    while (!compare("}",liste->items[liste->curseur]->value) && can_add(liste)){
        push_item(ast->body, parse_expression(liste));
        eat(PUNC,";",liste);
    }
    eat(PUNC,"}",liste);
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
    ast->body_if= init_list(sizeof(struct AST_T));
    while (!compare("}",liste->items[liste->curseur]->value) && can_add(liste)){
        push_item(ast->body_if, parse_expression(liste));
        eat(PUNC,";",liste);
    }
    eat(PUNC,"}",liste);
    if (can_add(liste)){
        if(compare(liste->items[liste->curseur]->value,"else")){
            eat(KW,"else",liste);
            eat(PUNC,"{",liste);
            ast->body_else = init_list(sizeof(struct AST_T));
            
            while (!compare("}",liste->items[liste->curseur]->value) && can_add(liste)){
                push_item(ast->body_else, parse_expression(liste));
                eat(PUNC,";",liste);
            }
            eat(PUNC,"}",liste);
        }
    }
    return ast;
}

AST_T* parse_while(token_list* liste){
    eat(KW,"while",liste);
    eat(PUNC,"(",liste);
    AST_T* ast = malloc(sizeof(struct AST_T));
    ast->type = WHILE ;
    ast->cond = parse_expression(liste);
    eat(PUNC,")",liste);
    eat(PUNC,"{",liste);
    ast->body_while= init_list(sizeof(struct AST_T));
    while (!compare("}",liste->items[liste->curseur]->value) && can_add(liste)){
        push_item(ast->body_while, parse_expression(liste));
        eat(PUNC,";",liste);
    }
    eat(PUNC,"}",liste);
    return ast;
}

AST_T* parse_call(token_list* liste){
    AST_T* call = malloc(sizeof(struct AST_T));
    call->name = eat(ID,"",liste);
    eat(PUNC,"(",liste);
    call->type = CALL;
    call->args = init_list(sizeof(struct AST_T));
    if (!(liste->items[liste->curseur]->value[0]==RPAR)) push_item(call->args, parse_expression(liste));
    while (!compare(")",liste->items[liste->curseur]->value) && can_add(liste)){
        eat(PUNC,",",liste);
        push_item(call->args, parse_expression(liste));
    }
    eat(PUNC,")",liste);
    return call;

}

AST_T* parse_bool(token_list* liste){
    AST_T* ast = token_to_AST(liste->items[liste->curseur]);
    ast->type = BOOL;
    return ast;
}

AST_T* parse_parentheses(token_list* liste){
    eat(PUNC,"(",liste);
    AST_T* ast = parse_expression(liste);
    eat(PUNC,")",liste);
    return ast;

}

AST_T* parse_atom(token_list* liste){
    
    if (can_add(liste)){
        // easiest case to parse from token to AST type 

        token* token = liste->items[liste->curseur];
        if (compare(token->value,"(")) return parse_parentheses(liste);
        else if ((token->type == ID) && (compare(liste->items[liste->curseur+1]->value,"("))){
            return parse_call(liste);
        }   
        else if (token->type == STR || token->type == ID || token->type == INT){
            liste->curseur+=1;
            return token_to_AST(token);
        }

        // Parsing of function and if statement
        else if (token->type==KW){
            char* value = token->value;
            if (compare(value,"function")) return parse_function(liste); 
            else if(compare(value,"if")) return parse_if(liste);
            else if(compare(value,"while")) return parse_while(liste);
            else if (compare(value, "true")||compare(value, "false")){
                liste->curseur+=1;
                return parse_bool(liste);
            }
            else if (compare(value,"let")) return parse_assign(liste);
        }
        else{
            printf("DON'T KNOW HOW TO PARSE");
            exit(0);
        }
    }
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
    AST_T* ast = parse_atom(liste);
    return maybe_binary(ast,0,liste);
};

