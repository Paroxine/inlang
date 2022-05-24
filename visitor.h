#ifndef VISITOR_H
#define VISITOR_H
#include "parse.h"

float apply_op(char* op, char* right_char, char* left_char);

bool char_to_bool(char* valeur);

bool in_environnement(char* name, list* environnement,int* indice);

void* evaluate_all(list* liste ,list* environnement);

AST_T* evaluate_assign(AST_T* node, list* environnement);

AST_T* evaluate_func(AST_T* node, list* environnement);

AST_T* evaluate_call(AST_T* node, list* environnement);

AST_T* evaluate(AST_T* node , list* environenemenet);

AST_T* evaluate_ID(AST_T* node, list* environnement);

AST_T* evaluate_IF(AST_T* node, list* environnement);

AST_T* evaluate_while(AST_T* node, list* environnement);

AST_T* evaluate_block(list* node, list* environnement);

char* get_value(AST_T* node, list* environnement);

list* function_environnement(list* literal_arg, list* actual_arg, list* environnement);

#endif