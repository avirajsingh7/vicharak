// parser.h
#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

// AST Node Types
typedef enum {
    NODE_PROGRAM,     
    NODE_VAR_DECL,   
    NODE_ASSIGN,      
    NODE_BINARY_OP,   
    NODE_IF,          
    NODE_LITERAL,     
    NODE_IDENTIFIER   
} NodeType;

typedef struct ASTNode {
    NodeType type;
    union {
        int value;                  
        char name[MAX_TOKEN_LEN];   
        struct {
            struct ASTNode *left;   
            struct ASTNode *right; 
        } binary_op;              
    };
    struct ASTNode *next;           
} ASTNode;

ASTNode *parse_program();           
void free_ast(ASTNode *node);       

#endif 
