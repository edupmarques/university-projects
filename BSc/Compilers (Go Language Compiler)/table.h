#ifndef TABLE_H
#define TABLE_H

typedef struct symbol_table {
    char *name;
    char *type;
    char *extra_info;
    struct symbol_table *next;
} symbol_table;

int symbol_exists(symbol_table *table, char *name);
symbol_table *new_symbol(char *name, char *type, char *extra_info);
void add_symbol(symbol_table **table, char *name, char *type, char *extra_info);
void print_table(symbol_table *table, char *title);
void function_print(symbol_table *table, char *title, char *str);
void process_var_decl(node *root, symbol_table **table, int *var_function);
void process_func_body(node *body_elements, symbol_table **table, int *var_function);
void process_func_params(node *param, symbol_table **table, char **param_list_type, char **param_list_name, int *param_count);
void process_func_decl(node *root, symbol_table **table, char *str, int *var_function);
void generate_table(node *root, symbol_table **table, char *str, int *var_function);
void generate_table_main(node *root);

#endif