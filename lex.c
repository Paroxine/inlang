#include "lex.h"

struct token *new_token(enum type type, char *value)
{
    struct token *token = malloc(sizeof(struct token));
    token->type = type;
    token->value = value;
    token->precedence = 0;
    return token;
};

token_list *init_token_list()
{
    token_list *liste = calloc(1, sizeof(struct token_list));
    liste->items = 0;
    liste->size = 0;
    liste->curseur = 0;
    return liste;
}

void add_token(token *item, token_list *liste)
{
    if (!liste->items)
        liste->items = calloc(1, sizeof(token *));
    else
        liste->items = realloc(liste->items, sizeof(token *) * (liste->size + 1));
    liste->items[liste->size] = item;
    liste->size += 1;
}

void show_token(struct token *token)
{
    if (token->type == PUNC)
    {
        printf("Punctuation : %s ", token->value);
    };
    if (token->type == OP)
    {
        printf("Operation : %s ", token->value);
    };
    if (token->type == STR)
    {
        printf("String : %s ", token->value);
    };
    if (token->type == KW)
    {
        printf("Keyword : %s ", token->value);
    };
    if (token->type == ID)
    {
        printf("Identifier : %s ", token->value);
    };
    if (token->type == INT)
    {
        printf("Int : %s ", token->value);
    };
    printf("\n");
    // printf(" %d\n",token->precedence);
};

char *char_to_string(char c)
{
    char *mot = malloc(sizeof(char) * 2);
    mot[0] = c;
    mot[1] = '\0';
    return mot;
};

bool is_Punc(char c)
{
    if (c == RPAR || c == LPAR || c == RBRA || c == LBRA || c == SEMIC || c == COMMA || c == LCURL || c == RCURL || c == HTAG || c == LINE || c == POINT || c == 0 || c == 10)
        return true;
    else
        return false;
}

bool is_Op(char c)
{
    if (c == PLUS || c == MINUS || c == PROD || c == DIV || c == PERC || c == EQUAL || c == EXCLA || c == LESS || c == GREATER)
        return true;
    else
        return false;
};

char *concat(char *mot1, char *mot2)
{
    char *conc = malloc(sizeof(char) * (strlen(mot1) + strlen(mot2)));
    strcpy(conc, mot1);
    strcat(conc, mot2);
    return conc;
}

char *read_String(char *texte, int *curseur)
{
    char *mot = "";
    int i = 0;
    char c = texte[i + *curseur];
    while (c != '"')
    {
        mot = concat(mot, char_to_string(c));
        i++;
        c = texte[i + *curseur];
    }
    *curseur = *curseur + strlen(mot);
    return (mot);
};

char *read_Identifier(char *texte, int *curseur)
{
    char *mot = "";
    int i = 0;
    char c = texte[i + *curseur];
    while ((c >= 97 && c <= 122) || (c >= 65 && c <= 90) || (c >= 48 && c <= 57))
    {
        mot = concat(mot, char_to_string(c));
        i++;
        c = texte[i + *curseur];
    }
    *curseur = *curseur + strlen(mot);
    return (mot);
};

void *read_Comment(char *texte, int *curseur)
{
    *curseur += 1;
    while (texte[*curseur] != '@' && *curseur < strlen(texte))
    {
        *curseur += 1;
    }
    *curseur += 1;
}

bool is_Keyword(char *mot)
{
    if (strcmp(mot, "if") == 0 || strcmp(mot, "else") == 0 || strcmp(mot, "while") == 0 || strcmp(mot, "function") == 0 || strcmp(mot, "let") == 0 || strcmp(mot, "true") == 0 || strcmp(mot, "false") == 0)
        return true;
    else
        return false;
};

char *read_Float(char *texte, int *curseur)
{
    char *endPtn;
    int buffer = *curseur;
    float nombre = strtof(texte + *curseur, &endPtn);
    *curseur = strlen(texte) - strlen(endPtn) - 1;
    int len = *curseur - buffer;
    char *mot = malloc(sizeof(char) * (len + 1));
    sprintf(mot, "%f", nombre);
    return mot;
};

void add_precedence(token_list *liste)
{
    for (int i = 0; i < liste->size; i++)
    {
        token *token = liste->items[i];
        if (token->type != OP)
        {
            token->precedence = 0;
        }
        else
        {
            switch (token->value[0])
            {
            case '=':
                token->precedence = 1;
                break;
            case '<':
                token->precedence = 7;
                break;
            case '>':
                token->precedence = 7;
                break;
            case '+':
                token->precedence = 10;
                break;
            case '-':
                token->precedence = 10;
                break;
            case '*':
                token->precedence = 20;
                break;
            case '/':
                token->precedence = 20;
                break;
            case '%':
                token->precedence = 20;
                break;

            default:
                break;
            }
            if (strcmp(token->value, "==") == 0)
                token->precedence = 7;
            else if (strcmp(token->value, "<=") == 0)
                token->precedence = 7;
            else if (strcmp(token->value, ">=") == 0)
                token->precedence = 7;
            else if (strcmp(token->value, "!=") == 0)
                token->precedence = 7;
            else if (strcmp(token->value, "&&") == 3)
                token->precedence = 7;
            else if (strcmp(token->value, "||") == 2)
                token->precedence = 7;
        };
    }
};

token_list *lexer(char *filename)
{
    int compteur = 0;
    token_list *liste = init_token_list();
    FILE *fichier = fopen(filename, "r");
    fseek(fichier, 0, SEEK_END);
    int len = ftell(fichier);
    fseek(fichier, 0, SEEK_SET);
    char *texte = malloc(sizeof(char) * (len + 1));
    fread(texte, 1, len, fichier);
    int curseur = 0;
    while (curseur < len && (texte[curseur] != '\0'))
    {
        char c = texte[curseur];
        char *value = char_to_string(c);
        if (c == '@')
        {
            read_Comment(texte, &curseur);
        }
        else if (c == SPACE || c == 10 || c == 13 || c == '\t')
        {
            curseur++;
            compteur--;
        }
        else if (is_Op(c))
        {
            if (is_Op(texte[curseur + 1]))
            {
                char *value2 = strdup(value);
                free(value);
                char *value = concat(value2, char_to_string(texte[curseur + 1]));
                add_token(new_token(OP, value), liste);
                curseur += 2;
            }
            else
            {
                add_token(new_token(OP, value), liste);
                curseur++;
            }
        }
        else if (is_Punc(c))
        {
            add_token(new_token(PUNC, value), liste);
            curseur++;
        }
        else if (c == QUOTE)
        {
            curseur++;
            add_token(new_token(STR, read_String(texte, &curseur)), liste);
            curseur++;
        }
        else if (isdigit(c))
        {
            add_token(new_token(INT, read_Float(texte, &curseur)), liste);
            curseur++;
        }
        else if ((c >= 97 && c <= 122) || (c >= 65 && c <= 90))
        {
            char *mot = read_Identifier(texte, &curseur);
            if (is_Keyword(mot))
            {
                add_token(new_token(KW, mot), liste);
            }
            else
            {
                add_token(new_token(ID, mot), liste);
            }
        };
        compteur++;
    }
    add_precedence(liste);
    return liste;
};