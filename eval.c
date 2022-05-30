#include "eval.h"
#include "parse.c"

float apply_op(char* op, char* right_char, char* left_char){
    float right = strtof(right_char,NULL);
    float left = strtof(left_char,NULL);
    if (compare(op,"+")) return right + left;
    else if (compare(op,"-")) return -right + left;
    else if (compare(op,"*"))  return right * left;
    else if (compare(op,"/"))  return left / right;
    else if (compare(op,"<"))  return right > left;
    else if (compare(op,">"))  return right < left;
    else if (compare(op,"<=")) return right >= left;
    else if (compare(op,">=")) return right <= left;
    else if (compare(op,"==")) return right == left;
    else if (compare(op,"!=")) return right != left;
};

bool char_to_bool(char* valeur){
    if (atoi(valeur)==1) return true; 
    else if(atoi(valeur)==0) return false;
    else if (compare(valeur,"true")) return true;
    else if (compare(valeur,"false")) return false;
};

bool in_environnement(char* name, list* environnement,int* indice){
    int size = environnement->size;
    for (int i = 0; i < size; i++)
    {   
        AST_T* element = environnement->items[i];
        if(compare(element->name,name)){
            *indice = i ;
            return true;
        }
    }
    return false;
    
};

list* function_environnement(list* literal_arg, list* actual_arg, list* environnement){
    int l_size = literal_arg->size;
    int a_size = actual_arg->size;
    if (a_size!=l_size){
        printf("ERROR : MISSING ARGUMENTS");
        exit(0);
    }
    int j;
    list* new_environnement = init_list(sizeof(struct AST_T));
    for (int i = 0; i < l_size; i++)
    {
        AST_T* l_arg = literal_arg->items[i];
        AST_T* a_arg = actual_arg->items[i];
        AST_T* new_node = malloc(sizeof(struct AST_T));
        new_node->name = l_arg->value;
        if (in_environnement(a_arg->value, environnement,&j)){
            AST_T* env_arg = environnement->items[j];
            new_node->assign_value = env_arg->assign_value;
        }
        else new_node->assign_value = a_arg;
        push_item(new_environnement, new_node);

    }
    return new_environnement;
    
}; 

void* evaluate_all(list* liste ,list* environnement){
    int size = liste->size;
    for (int i = 0; i < size; i++)
    {   
        AST_T* liste_element = liste->items[i];
        evaluate(liste_element,environnement);
    }
    
}

// EVALUATE ASSIGN 

AST_T* evaluate_assign(AST_T* node, list* environnement){
    int type = node->type;
    char* name = node->name;
    int indice;
    if (in_environnement(name,environnement,&indice)){
        AST_T* e_node = environnement->items[indice];
        e_node->assign_value = evaluate(node->assign_value, environnement);
    }
    else{
        AST_T* node_value = node->assign_value;
        AST_T* new_value = evaluate(node_value, environnement);
        node->assign_value = new_value;
        push_item(environnement, node);
    }
}

AST_T* evaluate_ID(AST_T* node, list* environnement){
    int type = node->type;
    char* name = node->value;
    int indice;
    if (in_environnement(name,environnement,&indice)){
        AST_T* e_node = environnement->items[indice];
        return e_node->assign_value;
    }
    else{
        printf("ERROR : NO VARIABLE WITH THE NAME : %s\n", name );
        exit(0);
    }
}

AST_T* evaluate_binary(AST_T* node, list* environnement){
    
    AST_T* right = evaluate(node->right,environnement);
    AST_T* left = evaluate(node->left,environnement);
    float value = apply_op(node->operator,right->value,left->value);
    char* value_char = malloc(1);
    sprintf(value_char,"%f",value);
    AST_T* new_node = malloc(sizeof(struct AST_T));
    new_node->type = INT;
    new_node->value = value_char;
    return new_node;
    
}

AST_T* evaluate_func(AST_T* node, list* environnement){
    push_item(environnement,node);
}

void* print(list* argument,list* environnement){
    int size = argument->size;
    int indice;
    for (int i = 0; i < size; i++)
    {   
        AST_T* arg = argument->items[i];
        AST_T* ast = evaluate(arg,environnement);
        printf("%s\n",ast->value);
    }
};

char* concate(char* mot1, char* mot2){
    char* conc= malloc(sizeof(char)*(strlen(mot1)+strlen(mot2)));
    strcpy(conc,mot1);
    strcat(conc,mot2);
    return conc;
}

AST_T* fileopen(list* argument, list* environnement){
    if (argument->size!=2){
        printf(" ARGUMENT != 2 IN FILEOPEN");
        exit(0);
    }
    AST_T* arg = argument->items[0];
    AST_T* arg1 = argument->items[1];
    AST_T* ast = evaluate(arg,environnement);
    AST_T* ast1 = evaluate(arg1,environnement);
    FILE* fichier = fopen(ast->value,ast1->value);
    fseek(fichier,0, SEEK_END);
    int len = ftell(fichier);
    fseek(fichier,0,SEEK_SET);
    char* texte = malloc(sizeof(char) * (len+1));
    fread(texte,1,len,fichier);
    AST_T* ast3 = malloc(sizeof(struct AST_T));
    ast3->type = STR;
    ast3->value = texte;
    return ast3; 

};


AST_T* evaluate_block(list* liste, list* environnement){
    for (int i = 0; i < liste->size ; i++)
        {   
            AST_T* line = liste->items[i];
            if (line->type == CALL && compare(line->name,"return")){
                return evaluate(line->args->items[0],environnement);
            }
            evaluate(line,environnement);
        }
            
}

AST_T* evaluate_IF(AST_T* element, list* environnement){
    AST_T* cond = element->cond;
    if(char_to_bool(evaluate(cond,environnement)->value)) {
        return evaluate_block(element->body_if,environnement);
    }
    else{
        return evaluate_block(element->body_else,environnement);    
    }
}

AST_T* evaluate_while(AST_T* element, list* environnement){
    AST_T* cond = element->cond;
    while(char_to_bool(evaluate(cond,environnement)->value) ) {
        return evaluate_block(element->body_while,environnement);
    }
}

AST_T* evaluate_call(AST_T* element,list* environnement){
    list* argument = element->args;
    int size = argument->size;
    int indice;
    if (compare(element->name,"print")){
        print(element->args,environnement);
    }
    else if (compare(element->name,"fileopen")){
        return fileopen(element->args,environnement);
    }
    else if (in_environnement(element->name,environnement,&indice)){
        AST_T* arg_env = environnement->items[indice];
        if ((arg_env->type == FUNC)){
            list* func_env = function_environnement(arg_env->args, element->args, environnement);
            return evaluate_block(arg_env->body,func_env);
        }
        else{
            printf("ERROR : WHAT YOU ARE TRYING TO CALL IS NOT A FUNCTION");
        }
    }
    else{
        printf("ERROR : NO FUNCTION WITH THAT NAME");
    }
};



AST_T* evaluate(AST_T* node , list* environnement){
    int type = node->type;
    switch (type)
    {
    case INT: 
        return node;
        break;
    case STR: 
        return node;
        break;
    case BOOL :
        return node;
        break;
    case BINARY: 
        return evaluate_binary(node,environnement);
        break;
    case ASSIGN: 
        return evaluate_assign(node,environnement);
        break;
    case ID: 
        return evaluate_ID(node,environnement);
        break;
    case IF:
        return evaluate_IF(node,environnement);
        break;
    case WHILE:
        return evaluate_while(node,environnement);
        break;
    case FUNC: 
        return evaluate_func(node,environnement);
        break;
    case CALL: 
        return evaluate_call(node,environnement);
        break;
    default:
        break;
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        token_list*  liste = lexer("test.txt");
    } else {
        token_list*  liste = lexer(argv[1]);
    }
    for (int i = 0; i < liste->size; i++)
    {
        show_token(liste->items[i]);
    }
    
    list* environnement = init_list(sizeof(struct AST_T));
    AST_T* ast = parse_all(liste);
    AST_T* ast2 = ast->body->items[3];
    evaluate_all(ast->body,environnement);
    // AST_T* ast3 = environnement->items[2];
    // printf("%s",ast3->assign_value->value);

}