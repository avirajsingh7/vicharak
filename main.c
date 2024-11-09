#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"

void printAST(ASTNode *node, int depth) {
    if (node == NULL) return;

    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    switch (node->type) {
        case NODE_PROGRAM:
            printf("Program\n");
            break;
        case NODE_VAR_DECL:
            printf("Variable Declaration: %s\n", node->name);
            break;
        case NODE_ASSIGN:
            printf("Assignment to %s\n", node->name);
            break;
        case NODE_BINARY_OP:
            printf("Binary Operation: %s\n", node->binary_op.left->name);
            break;
        case NODE_IF:
            printf("If Statement\n");
            break;
        case NODE_LITERAL:
            printf("Literal: %d\n", node->value);
            break;
        case NODE_IDENTIFIER:
            printf("Identifier: %s\n", node->name);
            break;
    }

    printAST(node->binary_op.left, depth + 1);
    printAST(node->binary_op.right, depth + 1);

    printAST(node->next, depth);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source-file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Failed to open file");
        return 1;
    }

    initLexer(file);

    ASTNode *program = parse_program();
    if (program == NULL) {
        fprintf(stderr, "Parsing failed.\n");
        fclose(file);
        return 1;
    }

    printf("Abstract Syntax Tree:\n");
    printAST(program, 0);

    free_ast(program);
    fclose(file);

    return 0;
}
