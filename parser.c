#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "lexer.h"

Token current_token;
FILE *source_file;

void error(const char *msg) {
    fprintf(stderr, "Parser error: %s\n", msg);
    exit(1);
}

void next_token() {
    getNextToken(source_file, &current_token);
}

ASTNode *create_node(NodeType type) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node) error("Out of memory");
    node->type = type;
    node->next = NULL;
    return node;
}

// Forward declarations for parsing functions
ASTNode *parse_statement();
ASTNode *parse_expression();
ASTNode *parse_term();
ASTNode *parse_factor();

ASTNode *parse_program() {
    ASTNode *program = create_node(NODE_PROGRAM);
    ASTNode *current = program;

    while (current_token.type != TOKEN_EOF) {
        ASTNode *stmt = parse_statement();
        current->next = stmt;
        current = stmt;
    }

    return program;
}

ASTNode *parse_statement() {
    if (current_token.type == TOKEN_INT) {
        next_token(); // Skip 'int'
        if (current_token.type != TOKEN_IDENTIFIER) error("Expected identifier after 'int'");
        
        ASTNode *node = create_node(NODE_VAR_DECL);
        strcpy(node->name, current_token.text);
        next_token(); // Skip identifier

        if (current_token.type != TOKEN_SEMICOLON) error("Expected ';' after variable declaration");
        next_token(); // Skip ';'
        return node;
    } 
    else if (current_token.type == TOKEN_IDENTIFIER) {
        ASTNode *node = create_node(NODE_ASSIGN);
        strcpy(node->name, current_token.text);
        
        next_token(); // Skip identifier
        if (current_token.type != TOKEN_ASSIGN) error("Expected '=' in assignment");
        
        next_token(); // Skip '='
        node->binary_op.left = parse_expression();

        if (current_token.type != TOKEN_SEMICOLON) error("Expected ';' after assignment");
        next_token(); // Skip ';'
        return node;
    } 
    else if (current_token.type == TOKEN_IF) {
        ASTNode *node = create_node(NODE_IF);
        
        next_token(); // Skip 'if'
        if (current_token.type != TOKEN_LBRACE) error("Expected '{' after 'if'");
        
        next_token(); // Skip '{'
        node->binary_op.left = parse_expression();
        
        if (current_token.type != TOKEN_RBRACE) error("Expected '}' after condition");
        next_token(); // Skip '}'

        node->binary_op.right = parse_statement();
        return node;
    } 
    else {
        error("Unexpected token in statement");
    }
    return NULL;
}

ASTNode *parse_expression() {
    ASTNode *node = parse_term();

    while (current_token.type == TOKEN_PLUS || current_token.type == TOKEN_MINUS) {
        ASTNode *op_node = create_node(NODE_BINARY_OP);
        op_node->binary_op.left = node;
        
        if (current_token.type == TOKEN_PLUS) {
            next_token();
            op_node->binary_op.right = parse_term();
        } else if (current_token.type == TOKEN_MINUS) {
            next_token();
            op_node->binary_op.right = parse_term();
        }
        
        node = op_node;
    }

    return node;
}

ASTNode *parse_term() {
    return parse_factor();
}

ASTNode *parse_factor() {
    ASTNode *node;
    if (current_token.type == TOKEN_NUMBER) {
        node = create_node(NODE_LITERAL);
        node->value = atoi(current_token.text);
        next_token(); // Consume number
    } 
    else if (current_token.type == TOKEN_IDENTIFIER) {
        node = create_node(NODE_IDENTIFIER);
        strcpy(node->name, current_token.text);
        next_token(); // Consume identifier
    } 
    else {
        error("Unexpected token in factor");
    }
    return node;
}

void free_ast(ASTNode *node) {
    if (node == NULL) return;
    
    if (node->type == NODE_BINARY_OP) {
        free_ast(node->binary_op.left);
        free_ast(node->binary_op.right);
    }

    free_ast(node->next);
    free(node);
}
