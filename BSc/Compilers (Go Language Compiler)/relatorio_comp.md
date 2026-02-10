Relatório Compiladores
2024-2025

(i) Gramática re-escrita

O desenvolvimento do analisador sintático foi abordado de forma faseada, sendo que, a par da sugestão dada no enunciado do projeto, começamos por reescrever para o YACC a gramática pretendida que nos foi dada em notação EBNF de modo a identificar possíveis erros de sintaxe e posteriormente procuramos aprimorar ao máximo a gramática, o que nos deu a possibilidade de construir a árvore de sintaxe abstrata (AST).

Para traduzirmos a gramática fornecida em notação EBNF para o formato aceite pelo YACC seguimos algumas diretrizes. A primeira trata-se das regras de associatividade e precedência, onde definimos os operadores com base nas mesmas, utilizando os comandos %left e %right. Isto simplificou o tratamento de expressões complexas e removeu as ambiguidades presentes na gramática. 

Em seguida, desenvolvemos as produçoes e as ações semânticas da gramática fornecida:

Declaração de Variáveis (VarDeclaration e VarSpec_aux): A produção VarDeclaration define a forma como as variáveis são declaradas e a subprodução VarSpec_aux permite que várias variáveis sejam declaradas (e.g., var x, y, z int).

Declaração de Funções (FuncDeclaration e FuncBody): A produção FuncDeclaration suporta a definição de funções com ou sem parâmetros e com ou sem tipo de retorno e o corpo da função é definido pela produção FuncBody, que faz o processamento de variáveis e declarações no corpo das funções através de VarsAndStatements.

Variáveis e Declarações (VarsAndStatements): Esta produção mistura as declarações de variáveis e instruções, permitindo uma organização flexível no corpo das funções. A gramática permite sequências de VarDeclaration e de Statment.

Instruções (Statement e Statement_aux): A produção Statement define as operações que podem ser realizadas num programa desta linguagem e a subprodução Statement_aux permite que muitas instruções sejam processadas.

Expressões (Expr): A produção Expr define operações matemáticas e comparações. Tal como já foi referido a associatividade e a precedência são garantidas pelas declarações %left e %right.

Por último, focamo-nos na deteção de erros utilizando o error em determinadas produções como em Statment e FuncInvocation, por exemplo, permitindo a identificação desses mesmos erros. 

(ii) Estruturas de dados

- AST

Após validar a gramática, o nosso foco deslocou-se para o desenvolvimento da árvore de sintaxe abstrata. A estrutura de dados node guarda as informações necessárias de cada elemento da árvore como a categoria (e.g., "Identifier"), o token que guarda o nome dos elementos e duas listas, uma para os filhos e outra para os irmãos. Essas listas também são definidas através de uma estrutura de dados chamada list que guarda um conjuntos de nodes. 

Posto isto, desenvolvemos todas as funções em C inerentes à criação da AST desde a função new_node que cria um novo nó à função print_tree que imprime o resultado final da AST. As funções add_childs e add_sibling que recorrem à função new_node adicionam um nó como filho ou como irmão de um nó já existente, respetivamente. A função organize_childs também desempenha um papel preponderante, uma vez que organiza os filhos de um nó adicionando um filho no início da lista dos filhos que, neste caso, foi usada para colocar o Type no início de uma Declaration. Por fim, a função free_tree é responsável por libertar toda a memória alocada ao longo do desenvolvimento da árvore.

- Tabela de símbolos

Na terceira meta, o principal objetivo foi a criação e organização de tabelas de símbolos para variavéis globais e locais, funções e os seus parâmetros e, para isso, implementamos estruturas de dados e funções em C.

A estrutura de dados symbol_table armazena os dados de cada símbolo como o nome, o tipo e informação extra (e.g., "param"). A função new_symbol cria um novo símbolo e a função add_symbol adiciona um símbolo a uma tabela, recorrendo à função symbol_exists que verifica se um símbolo já existe na mesma, evitando duplicação de símbolos.

A nossa estratégia passou por percorrer recursivamente todos os nós presentes na AST e ao analizar a categoria dos nós, dividimos o código em duas partes principais: processamento de variavéis através de nós "VarDecl" e processamento de funções sempre que nós com categoria "FuncDecl" são encontrados. Na declaração de variáveis, foi utilizada a função process_var_decl que adiciana uma variável a uma tabela. O processamento de funções distribui-se em process_func_decl, process_func_params e process_func_body. A primeira gera uma tabela de símbolos específica para a função, faz o processamento do tipo de retorno e dos parametros da funcão através da função process_func_params que adiciona os parâmetros ("FuncParams") à tabela através de recursão, não permitindo repetição dos mesmos e por último lugar, a função process_func_body processa o corpo da função, procurando e adicionando declarações de variáveis locais à tabela.

A função print_table é utilizada para imprimir a tabela global em primeiro lugar e a função function_print tem como objetivo concatenar todas as funcões numa só string ("functions") para mais tarde imprimi-las.

A função principal generate_table_main inicia a análise da árvore de sintaxe abstrata, cria a tabela global, chama a função generate_table que faz todo o processamento dos símbolos e por ultímo imprime todas as tabelas de funções.

Trabalho realizado por:
Eduardo Marques 2022231584
João Cardoso 2022222301