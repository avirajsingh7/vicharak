// parser.h
#include "lexer.h"

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
        int value;                // For literals
        char name[MAX_TOKEN_LEN]; // For identifiers
        struct {
            struct ASTNode *left;
            struct ASTNode *right;
        } binary_op;              // For binary operations
    };
    struct ASTNode *next;         // For statements in a program
} ASTNode;

// Parser functions
ASTNode *parse_program();
void free_ast(ASTNode *node);
