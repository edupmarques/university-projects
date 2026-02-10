/*
Eduardo Marques 2022231584
João Cardoso 2022222301
*/

%{
    #include "y.tab.h"
    #include "ast.h"
    #include "table.h"

    #define YY_USER_ACTION { syn_line = linha; syn_column = coluna; }
    #define PASS_TOKEN() yylval.lexeme = strdup(yytext);

    int yyparse(void);
    void yyerror(char *);

    int syn_line = -1;
    int syn_column = -1;

    int linha = 1;
    int coluna = 1;
    int linha_save;
    int coluna_save;
    int verifica;
    char buffer[1024];
    int caracter;
    int semicolon = 0;
    int flagl = 0 ;
    int flagt = 0;
    int eof = 0;
    int error_aux = 0;
    int string = 0;
    int print = 0; 

    extern struct node *program;
%}

%X COMMENT STRING

digit                        [0-9]
hexa                         [0-9a-fA-F]
hexadecimal                  0[xX]{hexa}+
decimal                      ({digit}+("."{digit}*)?|"."{digit}+)([eE][+-]?{digit}+)?
octal                        0[0-7]+
invalid_octal                0([0-7]*[8-9]+[0-7]*)+
letter                       [a-zA-Z]
identifier                   ("_"|{letter})({letter}|{digit}|"_")*
whitespace                   [ \t]

semicolon   ";"
comma       ","
blankid     "_"
assign      "="
star        "*"
div         "/"
minus       "-"
plus        "+"
eq          "=="
ge          ">="
gt          ">"
lbrace      "{"
le          "<="
lpar        "("
lsq         "["
lt          "<"
mod         "%"
ne          "!="
not         "!"
and         "&&"
or          "||"
rbrace      "}"
rpar        ")"
rsq         "]"
package     "package"
return      "return"
else        "else"
for         "for"
if          "if"
var         "var"
int         "int"
float32     "float32"
bool        "bool"
string      "string"
print       "fmt.Println"
parseint    "strconv.Atoi"
func        "func"
cmdargs     "os.Args"
reserved  	"break"|"case"|"chan"|"const"|"continue"|"default"|"defer"|"fallthrough"|"go"|"goto"|"import"|"interface"|"map"|"range"|"select"|"struct"|"switch"|"type"|"++"|"--"

%%
(\r)?"\n"                    { linha++; coluna = 1; if(flagl) { if(semicolon) printf("SEMICOLON\n") ; semicolon = 0 ; }; if( flagt && semicolon) { semicolon = 0 ; PASS_TOKEN(); return SEMICOLON; } ; }
<INITIAL><<EOF>>             { eof = 1 ; linha++; coluna = 1; if(flagl && semicolon) { printf("SEMICOLON\n") ; semicolon = 0 ; }; if( flagt && semicolon) { semicolon = 0 ; PASS_TOKEN(); return SEMICOLON; } ; return 0 ; }

"\""                         { linha_save = linha ; coluna_save = coluna ; BEGIN(STRING); verifica = 1 ; caracter = 0; buffer[0] = '\0'; coluna += yyleng ; semicolon = 0 ; }
<STRING>"\\"                 { printf("Line %d, column %d: invalid escape sequence (\\)\n", linha, coluna); coluna += yyleng ; }
<STRING>\\[fnrt\\\"]         { buffer[caracter++] = '\\' ; buffer[caracter++] = yytext[1]; buffer[caracter] = '\0'; coluna += yyleng ; semicolon = 0 ; }    
<STRING>\\[^fnrt\"\\]        { printf("Line %d, column %d: invalid escape sequence (\\%c)\n", linha, coluna, yytext[1]); caracter = 0 ; buffer[0] = '\0' ; verifica = 0 ;  semicolon = 0 ; coluna += yyleng ; }
<STRING>"\n"                 { printf("Line %d, column %d: unterminated string literal\n", linha_save, coluna_save); linha++; coluna = 1; BEGIN(INITIAL) ; semicolon = 0 ; }
<STRING>"\""                 { string = 0; BEGIN(INITIAL); coluna += yyleng ; if (flagl && verifica) { printf("STRLIT(\"%s\")\n", buffer); semicolon = 1; };  if ( flagt && verifica ){ semicolon = 1; string = 1; PASS_TOKEN(); char *str = (char *) malloc(strlen(buffer) + 3); string_format(str,buffer); yytext = strdup(str) ;yylval.lexeme = strdup(str); return STRLIT;}; }
<STRING>.                    { strcat(buffer, yytext); caracter += yyleng; coluna += yyleng; } 
<STRING><<EOF>>              { error_aux = 1; printf("Line %d, column %d: unterminated string literal\n", linha_save, coluna_save); verifica = 0; BEGIN(INITIAL) ; semicolon = 0 ; }

"/*"                         { linha_save = linha ; coluna_save = coluna ; BEGIN(COMMENT); coluna += yyleng ; }
<COMMENT>"*/"                { BEGIN(INITIAL) ; coluna += yyleng ; }
<COMMENT><<EOF>>             { error_aux = 1; printf("Line %d, column %d: unterminated comment\n", linha_save, coluna_save); BEGIN(INITIAL) ; semicolon = 0 ; return 0; }
<COMMENT>(\r)?"\n"           { if(flagl && semicolon) { printf("SEMICOLON\n"); }; coluna = 1 ; linha++ ; semicolon = 0 ; }
<COMMENT>.                   { coluna += yyleng ; }

"//"[^\n]*                   { if (flagl && semicolon) printf("SEMICOLON\n") ; if (flagt && semicolon) { semicolon = 0; PASS_TOKEN(); return SEMICOLON; } ; semicolon = 0 ; }

{semicolon}                  { if (flagl) printf("SEMICOLON\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return SEMICOLON; }; }
{comma}                      { if (flagl) printf("COMMA\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return COMMA; }; }
{blankid}                    { if (flagl) printf("BLANKID\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return BLANKID; }; }
{assign}                     { if (flagl) printf("ASSIGN\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return ASSIGN; }; }
{star}                       { if (flagl) printf("STAR\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return STAR; }; }
{div}                        { if (flagl) printf("DIV\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return DIV; }; }
{minus}                      { if (flagl) printf("MINUS\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return MINUS; }; }
{plus}                       { if (flagl) printf("PLUS\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return PLUS; }; }
{eq}                         { if (flagl) printf("EQ\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return EQ; }; }
{ge}                         { if (flagl) printf("GE\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return GE; }; }
{gt}	                     { if (flagl) printf("GT\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return GT; }; }
{lbrace}                     { if (flagl) printf("LBRACE\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return LBRACE; }; }
{le}                         { if (flagl) printf("LE\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return LE; }; }
{lpar}                       { if (flagl) printf("LPAR\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return LPAR; }; }
{lsq}                        { if (flagl) printf("LSQ\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return LSQ; }; }
{lt}                         { if (flagl) printf("LT\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return LT; }; }
{mod}                        { if (flagl) printf("MOD\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return MOD; }; }
{ne}                         { if (flagl) printf("NE\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return NE; }; }
{not}                        { if (flagl) printf("NOT\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return NOT; }; }
{and}                        { if (flagl) printf("AND\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return AND; }; }
{or}                         { if (flagl) printf("OR\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return OR; }; }
{rbrace}                     { if (flagl) printf("RBRACE\n"); coluna += yyleng ; semicolon = 1; if ( flagt ) { PASS_TOKEN(); return RBRACE; }; }
{rpar}                       { if (flagl) printf("RPAR\n"); coluna += yyleng ; semicolon = 1; if ( flagt ) { PASS_TOKEN(); return RPAR; }; }
{rsq}                        { if (flagl) printf("RSQ\n"); coluna += yyleng ; semicolon = 1; if ( flagt ) { PASS_TOKEN(); return RSQ; }; }
{package}                    { if (flagl) printf("PACKAGE\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return PACKAGE; }; }
{return}                     { if (flagl) printf("RETURN\n"); coluna += yyleng ; semicolon = 1; if ( flagt ) { PASS_TOKEN(); return RETURN; }; }
{else}                       { if (flagl) printf("ELSE\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return ELSE; }; }
{for}                        { if (flagl) printf("FOR\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return FOR; }; }
{if}                         { if (flagl) printf("IF\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return IF; }; }
{var}                        { if (flagl) printf("VAR\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return VAR; }; }
{int}                        { if (flagl) printf("INT\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return INT; }; }
{float32}                    { if (flagl) printf("FLOAT32\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return FLOAT32; }; }
{bool}                       { if (flagl) printf("BOOL\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return BOOL; }; }
{string}                     { if (flagl) printf("STRING\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return STR; }; }
{print}                      { print = linha; if (flagl) printf("PRINT\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return PRINT; }; }
{parseint}                   { if (flagl) printf("PARSEINT\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return PARSEINT; }; }
{func}                       { if (flagl) printf("FUNC\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return FUNC; }; }
{cmdargs}                    { if (flagl) printf("CMDARGS\n"); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return CMDARGS; }; }

{reserved}                   { if (flagl) printf("RESERVED(%s)\n", yytext); coluna += yyleng ; semicolon = 0; if ( flagt ) { PASS_TOKEN(); return CMDARGS; }; }

{identifier}                 { if (flagl) printf("IDENTIFIER(%s)\n",yytext); coluna += yyleng ; semicolon = 1; if ( flagt ) { PASS_TOKEN(); return IDENTIFIER; }; }
{octal}                      { if (flagl) printf("NATURAL(%s)\n",yytext); coluna += yyleng ; semicolon = 1; if ( flagt ) { PASS_TOKEN(); return NATURAL; }; }
{invalid_octal}              { error_aux = 1; printf("Line %d, column %d: invalid octal constant (%s)\n", linha, coluna, yytext); coluna += yyleng; semicolon = 0 ; }
{digit}+                     { if (flagl) printf("NATURAL(%s)\n",yytext); coluna += yyleng ; semicolon = 1; if ( flagt ) { PASS_TOKEN(); return NATURAL; }; }
{hexadecimal}                { if (flagl) printf("NATURAL(%s)\n",yytext); coluna += yyleng ; semicolon = 1; if ( flagt ) { PASS_TOKEN(); return NATURAL; }; }
{decimal}                    { if (flagl) printf("DECIMAL(%s)\n",yytext); coluna += yyleng ; semicolon = 1; if ( flagt ) { PASS_TOKEN(); return DECIMAL; }; }

{whitespace}                 { coluna += yyleng; }
.                            { printf("Line %d, column %d: illegal character (%c)\n", linha,  coluna, yytext[0]); coluna += yyleng ; }

%%

void yyerror(char *error) {
    error_aux = 1;
    if (eof) { printf("Line %d, column %d: %s: %s\n", linha, coluna, error, yytext); return; }
    if (linha_save == print && string) string = 0;
    if (string){
        syn_column = coluna_save;
        string = 0;
    }
    printf("Line %d, column %d: %s: %s\n", syn_line, syn_column, error, yytext);
}

extern int yylex();
int main(int argc, char *argv[]) {
	if(argc == 2){
		if(strcmp(argv[1],"-l") == 0){
			yylex();
		}
		else if(strcmp(argv[1],"-t") == 0){
			flagt = 1;
			yyparse();
            if (flagt && program && !error_aux) {
                print_tree(program, 0);
		    }
        }
        else if (strcmp(argv[1], "-s") == 0) {
            flagt = 1;
            flagl = 0;
            yyparse();
            if (flagt && program && !error_aux) {
                generate_table_main(program);
            }
            print_tree(program, 0);
        }
	}
	free_tree(program);
    yylex_destroy();
	return 0;
}

int yywrap() {  /* called on EOF, return 1 to terminate */
    return 1;
}