#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"

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


char* char_to_string(char c){
    char* mot = malloc(sizeof(char)*2);
    mot[0]=c;
    mot[1]='\0';
    return mot;

};

struct token* new_token(enum type type, char* value){
    struct token* token = malloc(sizeof(struct token));
    token->type = type;
    token->value = value;
    return token;

};

void add_token(token* token ,token_stack* stack) {
    struct token_stack *new = malloc(sizeof(struct token_stack));
    new->token = stack->token;
    new->next = stack->next;
    stack->next = new;
    stack->token = token;
}

struct token_stack* new_token_stack(){
    struct token_stack *new = malloc(sizeof(struct token_stack));
    new->next=NULL;
    return(new);

};

void show_token(struct token* token){
    if (token->type==PUNC){printf("Punctuation : %s\n", token->value);};
    if (token->type==OP){printf("Operation : %s\n", token->value);};
    if (token->type==STR){printf("String : %s\n", token->value);};
    if (token->type==KW){printf("Keyword : %s\n", token->value);};
    if (token->type==ID){printf("Identifier : %s\n", token->value);};
    if (token->type==INT){printf("Int : %s\n", token->value);};
    
};

void show_token_stack(struct token_stack* token_stack){
    struct token_stack* next = token_stack->next;
    if (token_stack->token!=NULL){
        show_token(token_stack->token);
        show_token_stack(token_stack->next);
    };
};

bool is_Punc (char c){
    if (c==RPAR||c==LPAR||c==RBRA||c==LBRA||c==SEMIC||c==COMMA||c==LCURL||c==RCURL||c==HTAG||c==LINE||c==POINT||c==0||c==10) return true;
    else return false;
}

bool is_Op (char c){
    if (c==PLUS||c==MINUS||c==PROD||c==DIV||c==PERC||c==EQUAL||c==EXCLA||c==LESS||c==GREATER) return true;
    else return false;
};

char* concat(char* mot1, char* mot2){
    char* conc= malloc(sizeof(char)*(strlen(mot1)+strlen(mot2)));
    strcpy(conc,mot1);
    strcat(conc,mot2);
    return conc;
}


char* read_String(char* texte, int* curseur){
    char* mot = "";
    int i = 0;
    char c = texte[i+*curseur];
    while ((c >= 97 && c <= 122) || (c >= 65 && c <= 90)){
        mot = concat(mot,char_to_string(c));
        i++;
        c = texte[i+*curseur];
    }
    *curseur = *curseur + strlen(mot);
    return(mot);
    
};

bool is_Keyword(char* mot){
    if (strcmp(mot,"if")==0||strcmp(mot,"else")==0||strcmp(mot,"while")==0||strcmp(mot,"then")==0||strcmp(mot,"function")==0||strcmp(mot,"let")==0||strcmp(mot,"true")==0||strcmp(mot,"false")==0) return true;
    else return false ;
};

char* read_Float(char* texte ,int* curseur){
    char* endPtn;
    int buffer = *curseur;
    float nombre = strtof(texte+*curseur,&endPtn);
    *curseur = strlen(texte)-strlen(endPtn);
    int len = *curseur - buffer;
    char* mot = malloc(sizeof(char)*(len+1));
    sprintf(mot,"%f",nombre);
    return mot;
    
};

token_stack* lexer(char* filename, int* taille){
    int compteur = 0;
    token_stack* stack =  new_token_stack();
    stack->token=NULL;
    FILE* fichier = fopen(filename,"r");
    fseek(fichier,0, SEEK_END);
    int len = ftell(fichier);
    fseek(fichier,0,SEEK_SET);
    char* texte = malloc(sizeof(char) * (len+1));
    fread(texte,1,len,fichier);
    int curseur = 0;
    while (curseur<len&&(texte[curseur]!='\0')){
        char c = texte[curseur];
        char* value = char_to_string(c);
        if (c==SPACE||c==10||c==13||c=='\t') {
            curseur++;
            compteur--;
        }
        else if (is_Op(c)) {
            add_token(new_token(OP,value),stack);
            curseur ++;
        }
        else if (is_Punc(c)) {
            add_token(new_token(PUNC,value),stack);
            curseur++;
        }
        else if (c==QUOTE) {
            curseur++;
            add_token(new_token(STR,read_String(texte, &curseur)),stack);
            curseur++;
        }
        else if (isdigit(c)){
            add_token(new_token(INT,read_Float(texte, &curseur)),stack);
            curseur++;
        }
        else if ((c >= 97 && c <= 122) || (c >= 65 && c <= 90)){
            char* mot = read_String(texte ,&curseur);
            if (is_Keyword(mot)){
                add_token(new_token(KW,mot),stack);
            }
            else {
                add_token(new_token(ID,mot),stack);
            }
        };
        compteur++;
       
    }
    *taille = compteur-1;
    return stack;
};

token** token_list(char* nom, int* taille){
    int i =0 ;
    token_stack* stack = lexer(nom,taille);
    token** tab = malloc(sizeof(token*)*(*taille+1));
    while(stack->token!=NULL){
        tab[*taille-i]=stack->token;
        stack = stack->next;
        i++;
    }
    for (int k = 0; k < *taille; k++)
    {
        show_token(tab[k]);
    }
    free(stack);
    return tab;
};

