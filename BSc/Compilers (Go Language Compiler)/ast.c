/*
Eduardo Marques 2022231584
João Cardoso 2022222301
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h> 
#include "ast.h"
#include "y.tab.h"

node *new_node(char* category, char *token) {
    struct node *new_node = (struct node *)malloc(sizeof(struct node));
    new_node->category = strdup(category);
    if (token) {
        new_node->token = strdup(token);
    } else {
        new_node->token = NULL;
    }
    new_node->children = NULL;
    new_node->sibling = NULL;
    return new_node;
}

void add_childs(node *parent, int nargs, ...) {
    va_list args;                          
    va_start(args, nargs);                 
    for (int i = 0; i < nargs; i++) {
        node *child = va_arg(args, node *);
        list *list_nodes = (list *)malloc(sizeof(list));
        list_nodes->node = child;
        list_nodes->next = NULL;
        if (parent->children == NULL) {
            parent->children = list_nodes;
        } else {
            list *current = parent->children;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = list_nodes;
        }
    }
    va_end(args);  
}

void add_sibling(node *sibling, node *new_sibling) {
    if (sibling == NULL || new_sibling == NULL) return;
    list *list_siblings = sibling->sibling;
    if (list_siblings == NULL) {
        sibling->sibling = (list *)malloc(sizeof(list));
        sibling->sibling->node = new_sibling;
        sibling->sibling->next = NULL;
    } else {
        while (list_siblings->next != NULL) {
            list_siblings = list_siblings->next;
        }
        list_siblings->next = (list *)malloc(sizeof(list));
        list_siblings->next->node = new_sibling;
        list_siblings->next->next = NULL;
    }
}

void organize_childs(node *parent, node *child) {
    if (parent == NULL || child == NULL) return;
    list *list_nodes = (list *)malloc(sizeof(list));
    list_nodes->node = child;
    list_nodes->next = parent->children; 
    parent->children = list_nodes;

    list *list_siblings = parent->sibling;
    while (list_siblings != NULL) {
        node *sibling_node = list_siblings->node;
        organize_childs(sibling_node, child);
        list_siblings = list_siblings->next;
    }
}

void print_tree(node *node, int depth) {
    if (node == NULL) return;
    for (int i = 0; i < depth; i++) {
        printf("..");
    }
    if (node->token != NULL) {
        printf("%s(%s)\n", node->category, node->token);
    } else {
        printf("%s\n", node->category);
    }
    list *current_child = node->children;
    while (current_child != NULL) {
        print_tree(current_child->node, depth + 1);
        current_child = current_child->next;
    }
    current_child = node->sibling;
    while (current_child != NULL) {
        print_tree(current_child->node, depth);
        current_child = current_child->next;
    }
}

int count_siblings(node *node){
    int count = 1;
    list *list_siblings = node->sibling;
    while (list_siblings != NULL){
        count++;
        list_siblings = list_siblings->next;
    }
    return count;
}

void free_tree(node *node) {
    if (node == NULL) return; 
    list *current_child = node->children;
    while (current_child != NULL) {
        list *next_child = current_child->next; 
        free_tree(current_child->node); 
        free(current_child);
        current_child = next_child; 
    }
    list *current_sibling = node->sibling;
    while (current_sibling != NULL) {
        list *next_sibling = current_sibling->next;
        free(current_sibling);
        current_sibling = next_sibling;
    }
    free(node->category); 
    free(node->token); 
    free(node);
}

void string_format(char *destination, char *string) {
    destination[0] = '\"';
    destination[1] = '\0';
    strcat(destination, string);
    strcat(destination, "\"");
}