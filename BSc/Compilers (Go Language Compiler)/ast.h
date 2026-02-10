/*
Eduardo Marques 2022231584
João Cardoso 2022222301
*/

#ifndef AST_H
#define AST_H

typedef struct node node;
typedef struct list list;

struct node {
    char *category;
    char *token;
    list *children;
    list *sibling;
};

struct list {
    node *node;
    list *next;
};

node *new_node(char *category, char *token);
void add_childs(node *parent,int nargs, ...);
void add_sibling(node *sibling, node *new_sibling);
void organize_childs(node *parent, node *child);
int count_siblings(node *node);
void print_tree(struct node *node, int depth);
void string_format(char *destination, char *string);
void free_tree(node *node);

#endif