#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ast.h"
#include "table.h"
#include "y.tab.h"

int symbol_exists(symbol_table *table, char *name) {
    symbol_table *current = table;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

symbol_table *new_symbol(char *name, char *type, char *extra_info) {
    symbol_table *symbol = (symbol_table *)malloc(sizeof(symbol_table));
    if (!symbol) exit(EXIT_FAILURE);
    symbol->name = strdup(name);
    symbol->type = strdup(type);
    if (extra_info != NULL && strlen(extra_info) >= 2) {
        symbol->extra_info = strdup(extra_info);
    } else {
        symbol->extra_info = NULL;
    }
    symbol->next = NULL;
    return symbol;
}

void add_symbol(symbol_table **table, char *name, char *type, char *extra_info) {
    if (symbol_exists(*table,name)) return;
    symbol_table *symbol = new_symbol(name, type, extra_info);
    if (*table == NULL) {
        *table = symbol;
    } else {
        symbol_table *current = *table;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = symbol;
    }
}

void print_table(symbol_table *table, char *title) {
    printf("===== %s =====\n", title);
    symbol_table *current = table;
    while (current != NULL) {
        if (current->extra_info == NULL) {
            printf("%s\t\t%s", current->name, current->type);
        }
        else {
            printf("%s\t%s", current->name, current->type);
        }
        if (current->extra_info) {
            printf("\t%s", current->extra_info);
        }
        printf("\n");
        current = current->next;
    }
    printf("\n");
}

void function_print(symbol_table *table, char *title, char *str) {
    char buffer[10000];
    snprintf(buffer, sizeof(buffer), "===== %s =====\n", title);
    strcat(str, buffer);
    symbol_table *current = table;
    while (current != NULL) {
        snprintf(buffer, sizeof(buffer), "%s\t\t%s", current->name, current->type);
        strcat(str, buffer);
        if (current->extra_info) {
            snprintf(buffer, sizeof(buffer), "\t%s", current->extra_info);
            strcat(str, buffer);
        }
        strcat(str, "\n");
        current = current->next;
    }
    strcat(str, "\n");
}

void lower_first_char(char *str) {
    if (str && str[0] >= 'A' && str[0] <= 'Z') {
        str[0] = tolower(str[0]);
    }
}

void process_var_decl(node *root, symbol_table **table, int *var_function) {
    node *var_type = root->children->node;
    node *var_name = root->children->next->node;
    char *aux = strdup(var_type->category);
    lower_first_char(aux);
    add_symbol(table, var_name->token, aux, " ");
    if (*var_function == 1 && root->sibling == NULL){
        *var_function = 0;
    }
}

void process_func_body(node *body_elements, symbol_table **table, int *var_function) {
    if (body_elements != NULL) {
        node *element = body_elements;
        if (strcmp(element->category, "VarDecl") == 0) {
            *var_function = 1;
            process_var_decl(element,table, var_function);
        }
        if (element->children != NULL) {
            list *current_child = element->children;
            while (current_child != NULL) {
                process_func_body(current_child->node, table, var_function);
                current_child = current_child->next;
            }
        }
        list *sibling = element->sibling;
        while (sibling != NULL) {
            process_func_body(sibling->node, table, var_function);
            sibling = sibling->next;
        }
    }
}

void process_func_params(node *param, symbol_table **table, char **param_list_type, char **param_list_name, int *param_count) {
    if (param != NULL) {
        if (strcmp(param->category, "ParamDecl") == 0) {
            node *param_type = param->children->node;
            node *param_name = param->children->next->node;
            char *aux = strdup(param_type->category);
            lower_first_char(aux);
            int flag = 0;
            for (int i = 0; i < *param_count; i++) {
                if (strcmp(param_list_name[i], param_name->token) == 0) {
                    flag = 1;
                    break;
                }
            }
            if (flag == 0) {
                add_symbol(table, param_name->token, aux, "param");
                param_list_type[*param_count] = strdup(aux);
                param_list_name[*param_count] = strdup(param_name->token);
                (*param_count)++;
            }
        }
        if (param->children != NULL) {
            list *current_child = param->children;
            while (current_child != NULL) {
                process_func_params(current_child->node, table, param_list_type, param_list_name,param_count);
                current_child = current_child->next;
            }
        }
        list *sibling = param->sibling;
        while (sibling != NULL) {
            process_func_params(sibling->node, table, param_list_type, param_list_name, param_count);
            sibling = sibling->next;
        }
    }
}

void process_func_decl(node *root, symbol_table **table, char *str, int *var_function) {
    symbol_table *function_table = NULL;
    node *func_header = root->children->node;
    char *func_name = func_header->children->node->token;
    char *func_return_type = func_header->children->next->node->category;
    node *func_params;
    if (strcmp(func_return_type, "FuncParams") == 0) { 
        func_params = func_header->children->next->node; 
        func_return_type = "none";
    }
    else {
        func_params = func_header->children->next->next->node;
    }
    char *aux = strdup(func_return_type);
    lower_first_char(aux);
    add_symbol(&function_table, "return", aux, NULL);
    node *func_body = root->children->next->node;
    if (func_body->children == NULL) *var_function = 0;
    process_func_body(func_body, &function_table, var_function);
    char *param_list_type[10000];
    char *param_list_name[10000];
    int param_count = 0;
    if (strcmp(func_params->category, "FuncParams") == 0) {
        process_func_params(func_params, &function_table, param_list_type, param_list_name, &param_count);
    }
    char title[10000];
    char params[10000] = "(";
    snprintf(title, sizeof(title), "Function %s", func_name);
    for (int i = 0; i < param_count; i++) {
        strcat(params, param_list_type[i]);
        if (i < param_count - 1) {
            strcat(params, ",");
        }
    }
    strcat(params,")");
    strcat(title, params);
    strcat(title, " Symbol Table");
    char *aux1 = strdup(func_return_type);
    lower_first_char(aux1);
    add_symbol(table, func_name, params, aux1);
    function_print(function_table,title,str);
}

void generate_table(node *root, symbol_table **table, char *str, int *var_function) {
    if (root == NULL) return;
    if (strcmp(root->category, "FuncDecl") == 0) {
        node *func_header = root->children->node;
        char *func_name = func_header->children->node->token;
        if (!symbol_exists(*table, func_name)) {
            *var_function = 1;
            process_func_decl(root, table, str, var_function);
        }
    }
    if (strcmp(root->category, "Return") == 0) *var_function = 0;
    if (strcmp(root->category, "VarDecl") == 0 && *var_function == 0) {
        process_var_decl(root, table, var_function);
    }
    if (root->children != NULL) {
        list *current_child = root->children;
        while (current_child != NULL) {
            generate_table(current_child->node, table, str, var_function);
            current_child = current_child->next;
        }
    }
    list *sibling = root->sibling;
    while (sibling != NULL) {
        generate_table(sibling->node, table, str, var_function);
        sibling = sibling->next;
    }
}

void generate_table_main(node *root) {
    int var_function = 0;
    char functions[100000] = "";
    symbol_table *global_table = NULL;
    generate_table(root, &global_table, functions, &var_function);
    print_table(global_table, "Global Symbol Table");
    printf("%s",functions);
}